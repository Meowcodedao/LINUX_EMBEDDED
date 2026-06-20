/*
 * WHY CHECK AND DEDUCT MUST BE IN THE SAME LOCK/UNLOCK BLOCK:
 *
 * Splitting into two separate critical sections:
 *   lock → check seats → unlock   ...   lock → deduct seats → unlock
 * creates a TOCTOU (time-of-check/time-of-use) race:
 *
 *   Thread A: checks seats_available (3) >= wanted (2) → passes, unlocks
 *   Thread B: checks seats_available (3) >= wanted (3) → passes, unlocks
 *   Thread A: locks, deducts → seats_available = 1
 *   Thread B: locks, deducts → seats_available = -2   ← OVERBOOKING!
 *
 * Combining check + deduct inside ONE lock/unlock makes the entire
 * "decide-then-act" operation atomic: no thread can observe or modify
 * seats_available between our check and our deduct.
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int  agent_id;
    char customer[50];
    int  seats_wanted;
} BookingRequest;

BookingRequest requests[5] = {
    {1, "Nguyen Van An",  2},
    {2, "Tran Thi Bich",  1},
    {3, "Le Van Cuong",   3},
    {4, "Pham Thi Dung",  1},
    {5, "Hoang Van Em",   2}
};

int seats_available = 10;
pthread_mutex_t seat_lock;

int seats_sold   = 0;
int failed_count = 0;

void *book_ticket(void *arg)
{
    BookingRequest *req = (BookingRequest *)arg;
    pthread_t tid = pthread_self();

    printf("[Agent %d | TID %lu] Booking %d seat%s for %s...\n",
           req->agent_id, (unsigned long)tid,
           req->seats_wanted, req->seats_wanted > 1 ? "s" : "",
           req->customer);

    sleep(1); /* force all threads to pile up at the mutex simultaneously */

    pthread_mutex_lock(&seat_lock);

    if (seats_available >= req->seats_wanted) {
        seats_available -= req->seats_wanted;
        seats_sold      += req->seats_wanted;
        printf("[Agent %d] CONFIRMED: %d seat%s for %s. Remaining: %d\n",
               req->agent_id,
               req->seats_wanted, req->seats_wanted > 1 ? "s" : "",
               req->customer, seats_available);
    } else {
        failed_count++;
        printf("[Agent %d] SOLD OUT:  needs %d seats, only %d left — booking failed.\n",
               req->agent_id, req->seats_wanted, seats_available);
    }

    pthread_mutex_unlock(&seat_lock);
    return NULL;
}

int main(void)
{
    pthread_t threads[5];
    int total_seats = seats_available;

    printf("==============================================\n");
    printf("   TICKET BOOKING SYSTEM (5 agents, 10 seats)\n");
    printf("==============================================\n");

    pthread_mutex_init(&seat_lock, NULL);

    for (int i = 0; i < 5; i++)
        pthread_create(&threads[i], NULL, book_ticket, &requests[i]);

    usleep(200000); /* let threads print booking lines before the separator */
    printf("\n--- [all agents reach critical section after sleep(1)] ---\n\n");

    for (int i = 0; i < 5; i++)
        pthread_join(threads[i], NULL);

    printf("\n================ SUMMARY ================\n");
    printf("  Total seats     : %d\n", total_seats);
    printf("  Seats sold      : %d\n", seats_sold);
    printf("  Seats remaining : %d\n", seats_available);
    printf("  Failed bookings : %d\n", failed_count);
    printf("=========================================\n");

    pthread_mutex_destroy(&seat_lock);
    return 0;
}
