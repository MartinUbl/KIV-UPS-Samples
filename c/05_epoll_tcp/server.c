/*
 * KIV/UPS - priklady sitoveho programovani
 *
 * Zakladni TCP server
 * Ve vsech prikladech na TCP v teto slozce je fungovani serveru identicke:
 *   - vytvori socket
 *   - binduje na 127.0.0.1:10000
 *   - prijima spojeni
 *   - prijima znaky a odesila je invertovane (A -> Z, B -> Y, ...)
 *   - zavre sockety po ukonceni komunikace
 * 
 * V teto variante je i paralelni odbaveni klientu:
 *   Kazdy klient je obslouzen pomoci epoll(), tj. server muze obsluhovat vice klientu soucasne bez vytvareni novych vlaken
 *
 * epoll je modernejsi varianta nez poll(), podporuje vice socketu a je efektivnejsi pri velkem poctu socketu
 * Rozhrani epoll je navic trochu prijemnejsi na spravu zdroju
 */

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>  // inet_pton, htons, struct sockaddr_in
#include <sys/socket.h> // socket, bind, listen, accept, send, recv
#include <unistd.h>     // close
#include <string.h>     // memset
#include <sys/epoll.h>  // epoll_create1, epoll_ctl, epoll_event

#define BIND_ADDRESS "127.0.0.1"
#define BIND_PORT 10000

int main(int, char**) {

    // vytvorime socket (pozadame system o jeho vytvreni)
    // AF_INET = IPv4, SOCK_STREAM = TCP (streamovane sluzby), 0 = vychozi protokol
    int skt = socket(AF_INET, SOCK_STREAM, 0);
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

    // zacneme naslouchat na socketu, ktery jsme si vytvorili a svazali s adresou
    // toto vytvori i frontu prichozich spojeni (druhy parametr, zde 5) - kolik spojeni muze cekat, nez je server ochoten je prijmout (accept)
    if (listen(skt, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Listening on %s:%d\n", BIND_ADDRESS, BIND_PORT);

    int efd;
    struct epoll_event event;
    struct epoll_event *events;

    efd = epoll_create1(0);
    if (efd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    event.data.fd = skt;
    event.events = EPOLLIN;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, skt, &event) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    events = calloc(16, sizeof(event));

    while (1) {

        int n = epoll_wait(efd, events, 16, -1);
        if (n == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < n; i++) {
            if (events[i].events & EPOLLIN) {
                if (events[i].data.fd == skt) {
                    // prijmeme spojeni od klienta
                    int client_skt = accept(skt, NULL, NULL);
                    if (client_skt < 0) {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }

                    // pridame novy socket klienta do epollu
                    event.data.fd = client_skt;
                    event.events = EPOLLIN;
                    if (epoll_ctl(efd, EPOLL_CTL_ADD, client_skt, &event) == -1) {
                        perror("epoll_ctl");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    // prijmeme data od klienta
                    char buffer;
                    int result = recv(events[i].data.fd, &buffer, 1, 0);

                    if (result <= 0) {
                        // klient uzavrel spojeni nebo nastala chyba (vypadek, ...)
                        printf("Client disconnected (socket %d)\n", events[i].data.fd);
                        close(events[i].data.fd);

                        // epoll odstrani socket klienta pri volani close automaticky
                    } else {
                        printf("Received: %c\n", buffer);
                        buffer = 'Z' - (buffer - 'A');
                        send(events[i].data.fd, &buffer, 1, 0);
                    }
                }
            }
        }
    }

    close(skt);

    return 0;
}
