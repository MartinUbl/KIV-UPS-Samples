CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: all clean

all: 02_client 02_server

02_client: client.c
	$(CC) $(CFLAGS) -o $@ $<

02_server: server.c
	$(CC) $(CFLAGS) -pthread -o $@ $<

clean:
	rm -f 02_client 02_server
