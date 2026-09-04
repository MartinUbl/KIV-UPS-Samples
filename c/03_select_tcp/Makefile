CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: all clean

all: 03_client 03_server

03_client: client.c
	$(CC) $(CFLAGS) -o $@ $<

03_server: server.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f 03_client 03_server
