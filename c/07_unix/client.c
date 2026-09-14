/*
 * KIV/UPS - priklady sitoveho programovani
 *
 * Zakladni Unix domain socket klient:
 *   - vytvori socket
 *   - pripoji se k "serveru"
 *   - posle 3 pismena a prijme odpovedi od serveru
 *   - zavre socket
 */

#include <stdlib.h>
#include <stdio.h>

#include <sys/un.h>      // struct sockaddr_un
#include <sys/socket.h>  // socket, connect, send, recv
#include <unistd.h>      // close

#define TARGET_ADDRESS "/tmp/ups_socket"

int main(int, char**) {

    // vytvorime socket (pozadame system o jeho vytvreni)
    // AF_INET = IPv4, SOCK_STREAM = TCP (streamovane sluzby), 0 = vychozi protokol
    int skt = socket(AF_UNIX, SOCK_STREAM, 0);
    if (skt < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // pripravime strukturu s adresou serveru, ke kteremu se chceme pripojit
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, TARGET_ADDRESS, sizeof(addr.sun_path) - 1);

    // pripojime se k serveru
    if (connect(skt, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to %s\n", TARGET_ADDRESS);

    char buffer;

    // posleme pismeno, prijmeme odpoved a vypiseme ji
    send(skt, "A", 1, 0);
    recv(skt, &buffer, 1, 0);
    printf("Received: %c\n", buffer);

    usleep(1000000); // 1s

    // a znovu...
    send(skt, "B", 1, 0);
    recv(skt, &buffer, 1, 0);
    printf("Received: %c\n", buffer);

    usleep(1000000); // 1s

    // a znovu...
    send(skt, "C", 1, 0);
    recv(skt, &buffer, 1, 0);
    printf("Received: %c\n", buffer);

    usleep(1000000); // 1s

    // zavreme socket - vracime prostredek zpet systemu
    close(skt);
}
