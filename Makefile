CC     = gcc
CFLAGS = -Wall -Wextra -g

all: call con

call: call.o tcp.o shared.o
	$(CC) $^ -o $@

con: controller.o tcp.o shared.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o call

.PHONY: clean