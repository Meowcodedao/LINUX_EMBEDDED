/*
 * WHY pthread_cond_wait() MUST BE INSIDE A WHILE LOOP — SPURIOUS WAKEUP:
 *
 * A "spurious wakeup" is when pthread_cond_wait() returns even though no
 * thread called pthread_cond_signal() or pthread_cond_broadcast().
 * POSIX explicitly permits this on certain implementations (e.g. Linux futex).
 *
 * With an `if`:
 *   if (count == 0)
 *       pthread_cond_wait(&not_empty, &q_lock);
 *   dequeue(doc);   ← runs on spurious wakeup even when queue is STILL empty!
 *
 * With a `while`, the condition is re-checked on every wakeup:
 *   while (count == 0 && !all_sent)
 *       pthread_cond_wait(&not_empty, &q_lock);
 *
 * Spurious wakeup → condition still false → thread goes back to sleep.
 * Also guards the "multiple waiters, one item" scenario: the loser
 * re-checks and waits again instead of dequeuing from an empty queue.
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define QUEUE_CAP        5
#define PRODUCER_COUNT   3
#define DOCS_PER_PROD    3
#define TOTAL_DOCS       (PRODUCER_COUNT * DOCS_PER_PROD)

typedef struct {
    int  doc_id;
    char filename[60];
    int  pages;
} Document;

Document queue[QUEUE_CAP];
int head = 0, tail = 0, count = 0;
int all_sent = 0;

pthread_mutex_t q_lock;
pthread_cond_t  not_full;
pthread_cond_t  not_empty;

int total_pages_printed = 0;
int docs_printed        = 0;

typedef struct {
    int      producer_id;
    Document docs[DOCS_PER_PROD];
} ProducerWork;

/* 9 docs: 3 per producer; total pages = 12+20+4 + 5+2+7 + 3+8+5 = 66 */
ProducerWork work[PRODUCER_COUNT] = {
    {1, {{1, "report_Q1.pdf", 12}, {4, "slides.pdf",   20}, {7, "summary.pdf",  4}}},
    {2, {{2, "contract.pdf",   5}, {5, "memo.pdf",      2}, {8, "budget.pdf",   7}}},
    {3, {{3, "invoice.pdf",    3}, {6, "proposal.pdf",  8}, {9, "handout.pdf",  5}}},
};

void *producer(void *arg)
{
    ProducerWork *pw = (ProducerWork *)arg;

    for (int i = 0; i < DOCS_PER_PROD; i++) {
        Document doc = pw->docs[i];

        pthread_mutex_lock(&q_lock);

        if (count == QUEUE_CAP)
            printf("[Producer %d] Queue full — waiting...\n", pw->producer_id);
        while (count == QUEUE_CAP)
            pthread_cond_wait(&not_full, &q_lock);

        queue[tail] = doc;
        tail = (tail + 1) % QUEUE_CAP;
        count++;

        printf("[Producer %d] Submitting: %-20s (%d pages) — queue: %d/%d\n",
               pw->producer_id, doc.filename, doc.pages, count, QUEUE_CAP);

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&q_lock);
    }

    return NULL;
}

void *printer(void *arg)
{
    (void)arg;

    while (1) {
        pthread_mutex_lock(&q_lock);

        while (count == 0 && !all_sent)
            pthread_cond_wait(&not_empty, &q_lock);

        if (count == 0 && all_sent) {
            pthread_mutex_unlock(&q_lock);
            break;
        }

        Document doc = queue[head];
        head = (head + 1) % QUEUE_CAP;
        count--;
        docs_printed++;
        total_pages_printed += doc.pages;

        printf("[Printer]    Printing:   %-20s (%d pages) — queue: %d/%d\n",
               doc.filename, doc.pages, count, QUEUE_CAP);

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&q_lock);

        sleep(1); /* simulate printing time */
    }

    printf("[Printer]    All documents printed. Exiting.\n");
    return NULL;
}

int main(void)
{
    pthread_t prod_threads[PRODUCER_COUNT];
    pthread_t print_thread;

    printf("==============================================\n");
    printf("   OFFICE PRINT QUEUE (3 producers, 1 printer)\n");
    printf("   Queue capacity: %d documents\n", QUEUE_CAP);
    printf("==============================================\n\n");

    pthread_mutex_init(&q_lock, NULL);
    pthread_cond_init(&not_full,  NULL);
    pthread_cond_init(&not_empty, NULL);

    pthread_create(&print_thread, NULL, printer, NULL);

    for (int i = 0; i < PRODUCER_COUNT; i++)
        pthread_create(&prod_threads[i], NULL, producer, &work[i]);

    for (int i = 0; i < PRODUCER_COUNT; i++)
        pthread_join(prod_threads[i], NULL);

    /* signal printer that no more documents will arrive */
    pthread_mutex_lock(&q_lock);
    all_sent = 1;
    pthread_cond_broadcast(&not_empty);
    pthread_mutex_unlock(&q_lock);

    pthread_join(print_thread, NULL);

    printf("\n================ SUMMARY ================\n");
    printf("  Documents submitted : %d\n", TOTAL_DOCS);
    printf("  Documents printed   : %d\n", docs_printed);
    printf("  Total pages printed : %d\n", total_pages_printed);
    printf("=========================================\n");

    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);
    pthread_mutex_destroy(&q_lock);

    return 0;
}
