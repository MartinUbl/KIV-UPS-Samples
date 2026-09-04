CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: all clean

all: 06_client 06_server

06_client: client.c
	$(CC) $(CFLAGS) -o $@ $<

06_server: server.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f 06_client 06_server
