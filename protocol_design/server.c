#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <arpa/inet.h>

#define BOOL int
#define TRUE 1
#define FALSE 0

#define DATALEN 32

void die(const char* chr) {
	printf("ERROR: %s\r\n", chr);
	exit(1);
}

BOOL running = TRUE;

typedef struct client_info
{
	int client_socket;
	char addr[INET6_ADDRSTRLEN];
} client_info;

void* thread_fnc(void* arg) {

	struct client_info* clinfo = (client_info*)arg;

	int skt = clinfo->client_socket;

	printf("[%d] Vitej, %s\r\n", skt, clinfo->addr);

	free(arg);

	int res;
	char* data = malloc(DATALEN + 1);
	int datalen = DATALEN;
	memset(data, 0, sizeof(data));

	int tmp;

	do
	{
		tmp = recv(skt, data, 12, 0); // melo by byt osetreno, ze prijimame 4 bajty

		if (tmp == 0)
		{
			printf("hotovo\r\n");
			close(skt);
			break;
		}

		if (strncmp(data, "KIVUPS", 6) != 0)
		{
			printf("zlej strejda\r\n");
			close(skt);
			break;
		}

		char opcode[3] = { data[6], data[7], '\0' };

		printf("opkod: %s\r\n", opcode);

		datalen = atoi(data + 8);

		printf("Zprava bude mit %d bajtu\r\n", datalen);

		res = 0;
		do
		{
			tmp = recv(skt, data + res, datalen - res, 0);
			if (tmp <= 0)
			{
				res = 0;
				break;
			}
			printf("prijato %d bajtu\r\n", tmp);

			res += tmp;
		}
		while (res < datalen);

		if (res <= 0) {
			printf("[%d] Konec vysilani (%d), odpojuji\r\n", skt, errno);
			close(skt);
		}
		else {
			data[res] = '\0';
			printf("Prijato: %s\r\n", data);
		}

	} while (res > 0);

	return NULL;
}

int main(int argc, char** argv) {

	int skt;
	int res;

	skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt < 0) {
		die("Nelze inicializovat socket");
	}

	srand(time(NULL));

	int param = 1;
    	res = setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, (const char*)&param, sizeof(int));

	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.s_addr = INADDR_ANY;

	res = bind(skt, (struct sockaddr*)&addr, sizeof(addr));
	if (res != 0) {
		die("Nelze nabindovat na zadanou adresu");
	}

	res = listen(skt, 10);
	if (res != 0) {
		die("Nelze vytvorit frontu spojeni");
	}

	struct sockaddr_in incoming;
	int inlen = sizeof(incoming);

	pthread_t thr;
	void* retval = NULL;

	printf("\n");
	printf("Test server - UPS cv. 5");
	printf("\n");
	printf("> Nasloucham na 0.0.0.0:9999\n");

	while (running)
	{
		int inskt = accept(skt, (struct sockaddr*)&incoming, &inlen);
		if (inskt >= 0) {
			struct client_info* infoptr = malloc(sizeof(struct client_info));
			infoptr->client_socket = inskt;
			inet_ntop(AF_INET, &incoming.sin_addr.s_addr, infoptr->addr, INET6_ADDRSTRLEN);

			pthread_create(&thr, NULL, (void*)&thread_fnc, (void*)infoptr);
			pthread_detach(thr);
		}
	}

	return 0;
}
