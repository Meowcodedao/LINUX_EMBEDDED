#include <stdio.h>
#include "calc.h"
#include "logger.h"

int main(void) {
    float a = 10.0f, b = 3.0f, c = 0.0f;
    float r;
    char  msg[128];

    log_write("Program started");
    log_timestamp();

    r = add(a, b);
    snprintf(msg, sizeof(msg), "%.2f + %.2f = %.2f", a, b, r);
    printf("%s\n", msg);
    log_write(msg);

    r = sub(a, b);
    snprintf(msg, sizeof(msg), "%.2f - %.2f = %.2f", a, b, r);
    printf("%s\n", msg);
    log_write(msg);

    r = mul(a, b);
    snprintf(msg, sizeof(msg), "%.2f * %.2f = %.2f", a, b, r);
    printf("%s\n", msg);
    log_write(msg);

    r = divf(a, b);
    snprintf(msg, sizeof(msg), "%.2f / %.2f = %.2f", a, b, r);
    printf("%s\n", msg);
    log_write(msg);

    r = divf(a, c);
    snprintf(msg, sizeof(msg), "%.2f / 0.00 = %.2f (division by zero)", a, r);
    printf("%s\n", msg);
    log_error(msg);

    log_write("Program ended");
    printf("Results logged to app.log\n");
    return 0;
}
