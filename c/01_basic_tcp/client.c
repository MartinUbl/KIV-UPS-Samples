/*
 * KIV/UPS - priklady sitoveho programovani
 *
 * Zakladni TCP klient
 * Ve vsech prikladech na TCP v teto slozce je klient identicky:
 *   - vytvori socket
 *   - pripoji se k serveru
 *   - posle 3 pismena a prijme odpovedi od serveru
 *   - zavre socket
 */

#include <stdlib.h>
#include <stdio.h>

#include <arpa/inet.h>   // inet_pton, htons, struct sockaddr_in
#include <sys/socket.h>  // socket, connect, send, recv
#include <unistd.h>      // close

#define TARGET_ADDRESS "127.0.0.1"
#define TARGET_PORT 10000

int main(int, char**) {

    // vytvorime socket (pozadame system o jeho vytvreni)
    // AF_INET = IPv4, SOCK_STREAM = TCP (streamovane sluzby), 0 = vychozi protokol
    int skt = socket(AF_INET, SOCK_STREAM, 0);
    if (skt < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // pripravime strukturu s adresou serveru, ke kteremu se chceme pripojit
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TARGET_PORT); // htons = host to network short (prevedeni portu do sitove endianity - big-endian)
    // inet_pton = prevedeni IP adresy z textove podoby do binarni podoby
    if (inet_pton(AF_INET, TARGET_ADDRESS, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    // pripojime se k serveru
    if (connect(skt, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to %s:%d\n", TARGET_ADDRESS, TARGET_PORT);

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
