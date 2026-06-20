#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef struct {
    int   id;
    char  name[50];
    int   quantity;
    float unit_price;
} Order;

void process_order(Order o) {
    float total = o.quantity * o.unit_price;
    printf("[CHILD-%d] PID: %d | PPID: %d\n", o.id, getpid(), getppid());
    printf("[CHILD-%d] %s x%d \xe2\x80\x94 Total: %.0f VND\n",
           o.id, o.name, o.quantity, total);
    printf("[CHILD-%d] Processing... (sleep 2s)\n\n", o.id);
    sleep(2);
}

static void print_vnd(float amount) {
    long long val = (long long)amount;
    long long millions  = val / 1000000;
    long long thousands = (val % 1000000) / 1000;
    long long units     = val % 1000;
    printf("%lld,%03lld,%03lld", millions, thousands, units);
}

int main(void) {
    Order orders[3] = {
        {1, "Backpack", 2, 350000},
        {2, "Shoes",    1, 500000},
        {3, "Hat",      3, 120000}
    };

    int num_orders = 3;
    pid_t pids[3];

    printf("\n===================================================\n");
    printf("   ORDER PROCESSING SYSTEM \xe2\x80\x94 MANAGER (fork+wait)\n");
    printf("===================================================\n");
    printf("[MANAGER] PID: %d \xe2\x80\x94 spawning %d child processes...\n\n",
           getpid(), num_orders);

    /* Loop 1: fork all children concurrently */
    for (int i = 0; i < num_orders; i++) {
        fflush(stdout);
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            process_order(orders[i]);
            exit(0);
        } else {
            pids[i] = pid;
            printf("[MANAGER] fork() order #%d \xe2\x86\x92 child PID: %d\n",
                   orders[i].id, pid);
        }
    }

    printf("[MANAGER] All %d children spawned. Starting waitpid()...\n", num_orders);
    printf("\n--- [child output order may interleave \xe2\x80\x94 this is normal] ---\n\n");

    /* Loop 2: reap each child in spawning order */
    int successful = 0;
    int failed     = 0;
    int status;

    for (int i = 0; i < num_orders; i++) {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            if (code == 0) {
                printf("[MANAGER] waitpid(%d) \xe2\x80\x94 order #%d: exit code=%d \xe2\x86\x92 SUCCESS\n",
                       pids[i], orders[i].id, code);
                successful++;
            } else {
                printf("[MANAGER] waitpid(%d) \xe2\x80\x94 order #%d: exit code=%d \xe2\x86\x92 FAILED\n",
                       pids[i], orders[i].id, code);
                failed++;
            }
        }
    }

    float total_revenue = 0;
    for (int i = 0; i < num_orders; i++)
        total_revenue += orders[i].quantity * orders[i].unit_price;

    printf("\n================= SUMMARY =================\n");
    printf("  Total orders    : %d\n", num_orders);
    printf("  Successful      : %d\n", successful);
    printf("  Failed          : %d\n", failed);
    printf("  Total revenue   : ");
    print_vnd(total_revenue);
    printf(" VND\n");
    printf("===========================================\n");

    return 0;
}
