CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: all clean

all: 05_client 05_server

05_client: client.c
	$(CC) $(CFLAGS) -o $@ $<

05_server: server.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f 05_client 05_server
