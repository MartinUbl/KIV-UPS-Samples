CC=gcc

all:	clean client server

client:	client.c
	${CC} -O3 -march=native -o client client.c

server: server.c
	${CC} -o server server.c -lpthread

clean: 
	rm -f client
	rm -f server
