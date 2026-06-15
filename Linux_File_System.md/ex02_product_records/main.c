#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#define DATA_FILE "products.dat"

typedef struct {
    int    id;
    char   name[64];
    int    quantity;
    double price;
} Product;

static int open_file(void)
{
    int fd = open(DATA_FILE, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("open");
        exit(1);
    }
    return fd;
}

static int record_count(int fd)
{
    off_t size = lseek(fd, 0, SEEK_END);
    return (int)(size / sizeof(Product));
}

static void add_product(int fd)
{
    Product p;
    printf("ID: ");       scanf("%d", &p.id);
    printf("Name: ");     scanf("%63s", p.name);
    printf("Quantity: "); scanf("%d", &p.quantity);
    printf("Price: ");    scanf("%lf", &p.price);

    lseek(fd, 0, SEEK_END);
    if (write(fd, &p, sizeof(Product)) != sizeof(Product))
        perror("write");
    else
        printf("Product added.\n");
}

static void show_by_index(int fd)
{
    int n = record_count(fd);
    if (n == 0) { printf("No products.\n"); return; }

    int idx;
    printf("Index (0-%d): ", n - 1);
    scanf("%d", &idx);
    if (idx < 0 || idx >= n) { printf("Index out of range.\n"); return; }

    Product p;
    off_t offset = (off_t)idx * sizeof(Product);
    lseek(fd, offset, SEEK_SET);
    if (read(fd, &p, sizeof(Product)) != sizeof(Product)) {
        perror("read"); return;
    }
    printf("Index=%-3d  ID=%-4d  Name=%-20s  Qty=%d  Price=%.2f\n",
           idx, p.id, p.name, p.quantity, p.price);
}

static void update_quantity(int fd)
{
    int n = record_count(fd);
    if (n == 0) { printf("No products.\n"); return; }

    int idx;
    printf("Index (0-%d): ", n - 1);
    scanf("%d", &idx);
    if (idx < 0 || idx >= n) { printf("Index out of range.\n"); return; }

    int new_qty;
    printf("New quantity: ");
    scanf("%d", &new_qty);

    off_t field_offset = (off_t)idx * sizeof(Product) + offsetof(Product, quantity);
    lseek(fd, field_offset, SEEK_SET);
    if (write(fd, &new_qty, sizeof(int)) != sizeof(int))
        perror("write");
    else
        printf("Quantity updated.\n");
}

static void list_all(int fd)
{
    int n = record_count(fd);
    if (n == 0) { printf("No products.\n"); return; }

    Product p;
    lseek(fd, 0, SEEK_SET);
    for (int i = 0; i < n; i++) {
        read(fd, &p, sizeof(Product));
        printf("Index=%-3d  ID=%-4d  Name=%-20s  Qty=%d  Price=%.2f\n",
               i, p.id, p.name, p.quantity, p.price);
    }
}

int main(void)
{
    int fd = open_file();
    int choice;

    while (1) {
        printf("\n1. Add product\n2. Show by index\n3. Update quantity\n4. List all\n5. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1: add_product(fd);    break;
            case 2: show_by_index(fd);  break;
            case 3: update_quantity(fd); break;
            case 4: list_all(fd);       break;
            case 5: close(fd); return 0;
            default: printf("Invalid choice.\n");
        }
    }

    close(fd);
    return 0;
}
