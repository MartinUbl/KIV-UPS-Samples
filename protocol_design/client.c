#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
	int skt;
	int res;

	skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	res = connect(skt, (struct sockaddr*)&addr, sizeof(addr));

	printf("Sypu to tam!\r\n");

	for (int i = 0; i < 20; i++)
	{
		send(skt, "KIVUPS010005", 12, 0);
		send(skt, "HE", 2, 0);
		send(skt, "LL", 2, 0);
		send(skt, "O", 1, 0);

		send(skt, "KIVUPS020008AHOJZUPS", 20, 0);

		// simulujeme 10ms nejake prace - "virtualni" mezera mezi zpravami
		//usleep(10);
	}

	printf("Hotovo!\r\n");

	close(skt);

	return 0;
}
