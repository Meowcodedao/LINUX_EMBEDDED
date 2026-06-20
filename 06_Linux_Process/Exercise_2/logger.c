#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define LOG_ERR     "<3>"
#define LOG_WARNING "<4>"
#define LOG_INFO    "<6>"

int main(void) {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    int cycle = 0;
    time_t start = time(NULL);

    while (1) {
        cycle++;
        fprintf(stderr, LOG_INFO    "Service running normally, cycle %d\n", cycle);
        fprintf(stderr, LOG_WARNING "Memory usage high: %d%%\n", 80 + rand() % 15);
        fprintf(stderr, LOG_ERR     "Failed to connect to database, retry %d\n", cycle);

        if (time(NULL) - start >= 30) {
            abort();
        }

        sleep(2);
    }

    return 0;
}
