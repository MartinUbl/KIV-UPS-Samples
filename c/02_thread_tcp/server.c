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
 *   Kazdy klient je obslouzen v samostatnem vlakne
 */

#include <stdlib.h>
#include <stdio.h>

#include <arpa/inet.h>  // inet_pton, htons, struct sockaddr_in
#include <sys/socket.h> // socket, bind, listen, accept, send, recv
#include <unistd.h>     // close

#include <pthread.h>    // pthread_create, pthread_join

#define BIND_ADDRESS "127.0.0.1"
#define BIND_PORT 10000

void* client_handler(void* param) {
    int client_skt = *(int*)param;
    free(param); // uvolnime pamet, kterou jsme alokovali v main() a poslali do vlakna

    char buffer;

    // prijmeme pismeno, vypiseme ho a posleme zpet "zrcadlovou" odpoved
    recv(client_skt, &buffer, 1, 0);
    printf("[%d] Received: %c\n", client_skt, buffer);
    buffer = 'Z' - (buffer - 'A');
    send(client_skt, &buffer, 1, 0);

    // a znovu...
    recv(client_skt, &buffer, 1, 0);
    printf("[%d] Received: %c\n", client_skt, buffer);
    buffer = 'Z' - (buffer - 'A');
    send(client_skt, &buffer, 1, 0);

    // a znovu...
    recv(client_skt, &buffer, 1, 0);
    printf("[%d] Received: %c\n", client_skt, buffer);
    buffer = 'Z' - (buffer - 'A');
    send(client_skt, &buffer, 1, 0);

    // zavreme sockety - vracime prostredek zpet systemu
    close(client_skt);
    return NULL;
}

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

    while (1) {
        pthread_t thread_id;

        // prijmeme spojeni od klienta
        // pozor: toto je takzvane blokujici volani, tj. program se zde zastavi a pocka, nez se neco stane (nez se klient pripoji)
        // accept vraci novy socket, ktery je uz jen pro komunikaci s timto klientem (puvodni socket zustava pro prijimani dalsich klientu)
        int client_skt = accept(skt, NULL, NULL);
        if (client_skt < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // alokujeme pamet, do ktere ulozime socket klienta a posleme jej do vlakna
        // nelze totiz zarucit, ze client_skt bude stale platny, az se vlakno spusti (mohlo by se stat, ze accept() prijme dalsiho klienta a client_skt se zmeni)
        // vlakno pak musi tuto pamet uvolnit
        int* client_skt_ptr = malloc(sizeof(int));
        *client_skt_ptr = client_skt;

        // vytvorime vlakno, ktere klienta odbavi - toto vlakno muze mezitim prijmout klienta dalsiho
        if (pthread_create(&thread_id, NULL, client_handler, client_skt_ptr) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }

        // odpojime vlakno od hlavniho vlakna, aby se po jeho ukonceni automaticky uvolnily prostredky
        // jinak bychom byli povinni volat pthread_join() a cekat na ukonceni vlakna
        pthread_detach(thread_id);
    }

    // v teto situaci nikdy nenastane (nekonecny while vyse)
    close(skt);

    return 0;
}
