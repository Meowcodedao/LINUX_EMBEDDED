#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

extern char **environ;

int main(void) {
    printf("\n=============================================\n");
    printf("   STUDENT LOOKUP SYSTEM \xe2\x80\x94 MANAGER\n");
    printf("   (fork + execve | file: students.txt)\n");
    printf("=============================================\n");
    printf("[MANAGER] PID: %d\n", getpid());
    printf("Enter student ID ('quit' to exit).\n");

    char input[100];

    while (1) {
        printf("\n---------------------------------------------\n");
        printf("Student ID: ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "quit") == 0) {
            printf("[MANAGER] Exiting. Goodbye!\n");
            break;
        }

        fflush(stdout);
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            char *args[] = {"./searcher", input, "students.txt", NULL};
            execve("./searcher", args, environ);
            /* Only reached if execve() FAILS — a successful execve() replaces
               the process image entirely so execution never returns here */
            perror("execve failed");
            exit(2);
        } else {
            printf("\n[MANAGER] fork() \xe2\x86\x92 child PID: %d\n", pid);
            printf("[MANAGER] Waiting for child (waitpid)...\n");

            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                int code = WEXITSTATUS(status);
                const char *result;
                if      (code == 0) result = "Found";
                else if (code == 1) result = "Not found";
                else                result = "Error";
                printf("\n[MANAGER] Child (PID %d) exited. code=%d \xe2\x86\x92 %s\n",
                       pid, code, result);
            }
        }
    }

    return 0;
}
