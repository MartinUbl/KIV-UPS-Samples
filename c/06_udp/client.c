/*
 * KIV/UPS - priklady sitoveho programovani
 *
 * Zakladni UDP klient:
 *   - vytvori socket
 *   - posle 3 pismena a prijme odpovedi od serveru
 *   - zavre socket
 */

#include <stdlib.h>
#include <stdio.h>

#include <arpa/inet.h>   // inet_pton, htons, struct sockaddr_in
#include <sys/socket.h>  // socket, send, recv
#include <unistd.h>      // close

#define TARGET_ADDRESS "127.0.0.1"
#define TARGET_PORT 10000

int main(int, char**) {

    // vytvorime socket (pozadame system o jeho vytvreni)
    // AF_INET = IPv4, SOCK_DGRAM = UDP (datagramove sluzby), 0 = vychozi protokol
    int skt = socket(AF_INET, SOCK_DGRAM, 0);
    if (skt < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // pripravime strukturu s adresou serveru, kteremu budeme zasilat datagramy
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TARGET_PORT); // htons = host to network short (prevedeni portu do sitove endianity - big-endian)
    // inet_pton = prevedeni IP adresy z textove podoby do binarni podoby
    if (inet_pton(AF_INET, TARGET_ADDRESS, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    printf("Using %s:%d as target\n", TARGET_ADDRESS, TARGET_PORT);

    // recvfrom bude ukladat adresu odesilatele do teto struktury, abychom vedeli, odkud datagram prisel
    struct sockaddr_in from_addr;
    socklen_t from_addr_len = sizeof(from_addr);

    char buffer;

    // posleme pismeno, prijmeme odpoved a vypiseme ji
    sendto(skt, "A", 1, 0, (struct sockaddr*)&addr, sizeof(addr));
    recvfrom(skt, &buffer, 1, 0, (struct sockaddr*)&from_addr, &from_addr_len);
    printf("Received: %c\n", buffer);

    usleep(1000000); // 1s

    // a znovu...
    sendto(skt, "B", 1, 0, (struct sockaddr*)&addr, sizeof(addr));
    recvfrom(skt, &buffer, 1, 0, (struct sockaddr*)&from_addr, &from_addr_len);
    printf("Received: %c\n", buffer);

    usleep(1000000); // 1s

    // a znovu...
    sendto(skt, "C", 1, 0, (struct sockaddr*)&addr, sizeof(addr));
    recvfrom(skt, &buffer, 1, 0, (struct sockaddr*)&from_addr, &from_addr_len);
    printf("Received: %c\n", buffer);

    usleep(1000000); // 1s

    // zavreme socket - vracime prostredek zpet systemu
    close(skt);
}
