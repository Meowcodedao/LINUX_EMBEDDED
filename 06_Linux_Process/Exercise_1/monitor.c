#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

static volatile int running = 1;

static void handle_sigterm(int sig) {
    (void)sig;
    running = 0;
}

int main(void) {
    setbuf(stdout, NULL);

    signal(SIGTERM, handle_sigterm);

    int cycle = 0;
    while (running) {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char ts[26];
        strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("[%s] Monitor service running, cycle %d\n", ts, ++cycle);
        sleep(1);
    }

    printf("Service shutting down...\n");
    return 0;
}
