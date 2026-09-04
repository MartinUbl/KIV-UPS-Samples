CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: all clean

all: 04_client 04_server

04_client: client.c
	$(CC) $(CFLAGS) -o $@ $<

04_server: server.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f 04_client 04_server
