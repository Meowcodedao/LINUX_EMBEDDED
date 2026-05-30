#include <stdio.h>
#include <time.h>
#include "logger.h"

static void get_timestamp(char *buf, int size) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

void log_write(const char *msg) {
    char ts[32];
    get_timestamp(ts, sizeof(ts));
    FILE *f = fopen("app.log", "a");
    if (!f) return;
    fprintf(f, "[%s] %s\n", ts, msg);
    fclose(f);
}

void log_timestamp(void) {
    char ts[32];
    get_timestamp(ts, sizeof(ts));
    printf("Timestamp: %s\n", ts);
}

void log_error(const char *msg) {
    char ts[32];
    get_timestamp(ts, sizeof(ts));
    FILE *f = fopen("app.log", "a");
    if (!f) return;
    fprintf(f, "[%s] [ERROR] %s\n", ts, msg);
    fclose(f);
}
