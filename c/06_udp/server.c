/*
 * KIV/UPS - priklady sitoveho programovani
 *
 * Zakladni UDP server
 * Ve vsech prikladech na UDP v teto slozce je fungovani serveru identicke:
 *   - vytvori socket
 *   - binduje na 127.0.0.1:10000
 *   - prijima datagramy
 *   - prijima znaky a odesila je invertovane (A -> Z, B -> Y, ...)
 *   - zavre sockety po ukonceni komunikace
 * 
 * V teto variante NENI paralelni odbaveni klientu
 */

#include <stdlib.h>
#include <stdio.h>

#include <arpa/inet.h>  // inet_pton, htons, struct sockaddr_in
#include <sys/socket.h> // socket, bind, listen, accept, send, recv
#include <unistd.h>     // close

#define BIND_ADDRESS "127.0.0.1"
#define BIND_PORT 10000

int main(int, char**) {

    // vytvorime socket (pozadame system o jeho vytvreni)
    // AF_INET = IPv4, SOCK_DGRAM = UDP (datagramove sluzby), 0 = vychozi protokol
    int skt = socket(AF_INET, SOCK_DGRAM, 0);
    if (skt < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // pripravime strukturu s adresou, na ktere chceme naslouchat
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(BIND_PORT); // htons = host to network short (prevedeni portu do sitove endianity - big-endian)
    // inet_pton = prevedeni IP adresy z textove podoby do binarni podoby
    if (inet_pton(AF_INET, BIND_ADDRESS, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    int optval = 1;
    // nastaveni socketu, aby se po zavreni socketu uvolnila adresa a port, na kterych naslouchal (jinak by se muselo pockat, nez by je system uvolnil)
    if (setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // priradime socketu adresu, na ktere chceme naslouchat
    // pozor, jeste neposlouchame, tady jen "svazeme" socket s adresou
    if (bind(skt, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("Listening on %s:%d\n", BIND_ADDRESS, BIND_PORT);

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    while (1) {
        char buffer;

        // prijmeme pismeno, vypiseme ho a posleme zpet "zrcadlovou" odpoved
        recvfrom(skt, &buffer, 1, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        printf("Received: %c\n", buffer);
        buffer = 'Z' - (buffer - 'A');
        sendto(skt, &buffer, 1, 0, (struct sockaddr*)&client_addr, client_addr_len);

        // a znovu...
        recvfrom(skt, &buffer, 1, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        printf("Received: %c\n", buffer);
        buffer = 'Z' - (buffer - 'A');
        sendto(skt, &buffer, 1, 0, (struct sockaddr*)&client_addr, client_addr_len);

        // a znovu...
        recvfrom(skt, &buffer, 1, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        printf("Received: %c\n", buffer);
        buffer = 'Z' - (buffer - 'A');
        sendto(skt, &buffer, 1, 0, (struct sockaddr*)&client_addr, client_addr_len);
    }

    close(skt);

    return 0;
}
