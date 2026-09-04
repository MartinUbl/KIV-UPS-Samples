CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: all clean

all: 01_client 01_server

01_client: client.c
	$(CC) $(CFLAGS) -o $@ $<

01_server: server.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f 01_client 01_server
