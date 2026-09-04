CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: all clean

all: 07_client 07_server

07_client: client.c
	$(CC) $(CFLAGS) -o $@ $<

07_server: server.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f 07_client 07_server
