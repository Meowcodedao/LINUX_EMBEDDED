#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *classify_grade(float gpa) {
    if (gpa >= 8.5f) return "Excellent";
    if (gpa >= 7.0f) return "Good";
    if (gpa >= 5.0f) return "Average";
    return "Poor";
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <student_id> <data_file>\n", argv[0]);
        exit(2);
    }

    const char *student_id = argv[1];
    const char *data_file  = argv[2];

    printf("[SEARCHER] PID: %d | PPID: %d\n", getpid(), getppid());
    printf("[SEARCHER] Searching for \"%s\" in %s...\n\n", student_id, data_file);

    FILE *fp = fopen(data_file, "r");
    if (fp == NULL) {
        perror("fopen failed");
        exit(2);
    }

    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        char tmp[256];
        strncpy(tmp, line, sizeof(tmp) - 1);
        tmp[sizeof(tmp) - 1] = '\0';

        char *id      = strtok(tmp, "|");
        char *name    = strtok(NULL, "|");
        char *class   = strtok(NULL, "|");
        char *gpa_str = strtok(NULL, "|");

        if (!id || !name || !class || !gpa_str)
            continue;

        if (strcmp(id, student_id) == 0) {
            float gpa = atof(gpa_str);
            printf("========== SEARCH RESULT ==========\n");
            printf("  ID      : %s\n", id);
            printf("  Name    : %s\n", name);
            printf("  Class   : %s\n", class);
            printf("  GPA     : %.1f\n", gpa);
            printf("  Grade   : %s\n", classify_grade(gpa));
            printf("====================================\n");
            fclose(fp);
            exit(0);
        }
    }

    fclose(fp);
    printf("[SEARCHER] No student found with ID: %s\n", student_id);
    exit(1);
}
