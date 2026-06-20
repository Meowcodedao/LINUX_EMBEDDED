CC     = gcc
CFLAGS = -Wall -Wextra -pthread

all: print_queue

print_queue: print_queue.c
	$(CC) $(CFLAGS) -o print_queue print_queue.c

clean:
	rm -f print_queue
