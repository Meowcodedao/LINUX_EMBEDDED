#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_FILE "students.dat"

typedef struct {
    int   id;
    char  name[64];
    int   age;
    float gpa;
} Student;

static int open_file(void)
{
    int fd = open(DATA_FILE, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("open");
        exit(1);
    }
    return fd;
}

static void add_student(int fd)
{
    Student s;
    printf("ID: ");    scanf("%d", &s.id);
    printf("Name: ");  scanf("%63s", s.name);
    printf("Age: ");   scanf("%d", &s.age);
    printf("GPA: ");   scanf("%f", &s.gpa);

    lseek(fd, 0, SEEK_END);
    if (write(fd, &s, sizeof(Student)) != sizeof(Student))
        perror("write");
    else
        printf("Student added.\n");
}

static void list_students(int fd)
{
    Student s;
    lseek(fd, 0, SEEK_SET);
    int count = 0;
    while (read(fd, &s, sizeof(Student)) == sizeof(Student)) {
        printf("ID=%-4d  Name=%-20s  Age=%d  GPA=%.2f\n",
               s.id, s.name, s.age, s.gpa);
        count++;
    }
    if (count == 0)
        printf("No students found.\n");
}

static void find_student(int fd)
{
    int target;
    printf("Enter ID to find: ");
    scanf("%d", &target);

    Student s;
    lseek(fd, 0, SEEK_SET);
    while (read(fd, &s, sizeof(Student)) == sizeof(Student)) {
        if (s.id == target) {
            printf("Found — ID=%-4d  Name=%-20s  Age=%d  GPA=%.2f\n",
                   s.id, s.name, s.age, s.gpa);
            return;
        }
    }
    printf("Student with ID %d not found.\n", target);
}

int main(void)
{
    int fd = open_file();
    int choice;

    while (1) {
        printf("\n1. Add student\n2. List all\n3. Find by ID\n4. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1: add_student(fd); break;
            case 2: list_students(fd); break;
            case 3: find_student(fd); break;
            case 4: close(fd); return 0;
            default: printf("Invalid choice.\n");
        }
    }

    close(fd);
    return 0;
}
