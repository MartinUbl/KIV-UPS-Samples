/*
 * KIV/UPS - priklady sitoveho programovani
 *
 * Zakladni UNIX domain socket server
 * Ve vsech prikladech na UNIX domain socket v teto slozce je fungovani serveru identicke:
 *   - vytvori socket
 *   - binduje na /tmp/ups_socket
 *   - prijima spojeni
 *   - prijima znaky a odesila je invertovane (A -> Z, B -> Y, ...)
 *   - zavre sockety po ukonceni komunikace
 * 
 * V teto variante NENI paralelni odbaveni klientu
 * 
 * Unix domain sockety jsou lokalni a komunikace probiha pouze mezi procesy na stejnem stroji
 */

#include <stdlib.h>
#include <stdio.h>

#include <sys/un.h>     // struct sockaddr_un
#include <sys/socket.h> // socket, bind, listen, accept, send, recv
#include <unistd.h>     // close

#define BIND_ADDRESS "/tmp/ups_socket"

int main(int, char**) {

    // vytvorime socket (pozadame system o jeho vytvreni)
    // AF_INET = IPv4, SOCK_STREAM = TCP (streamovane sluzby), 0 = vychozi protokol
    int skt = socket(AF_UNIX, SOCK_STREAM, 0);
    if (skt < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // pripravime strukturu s adresou, na ktere chceme naslouchat
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, BIND_ADDRESS, sizeof(addr.sun_path) - 1);

    int optval = 1;
    // nastaveni socketu, aby se po zavreni socketu uvolnila adresa, na ktere naslouchal (jinak by se muselo pockat, nez by ji system uvolnil)
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

    printf("Listening on %s\n", BIND_ADDRESS);

    while (1) {

        // prijmeme spojeni od klienta
        // pozor: toto je takzvane blokujici volani, tj. program se zde zastavi a pocka, nez se neco stane (nez se klient pripoji)
        // accept vraci novy socket, ktery je uz jen pro komunikaci s timto klientem (puvodni socket zustava pro prijimani dalsich klientu)
        int client_skt = accept(skt, NULL, NULL);
        if (client_skt < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        char buffer;

        // prijmeme pismeno, vypiseme ho a posleme zpet "zrcadlovou" odpoved
        recv(client_skt, &buffer, 1, 0);
        printf("Received: %c\n", buffer);
        buffer = 'Z' - (buffer - 'A');
        send(client_skt, &buffer, 1, 0);

        // a znovu...
        recv(client_skt, &buffer, 1, 0);
        printf("Received: %c\n", buffer);
        buffer = 'Z' - (buffer - 'A');
        send(client_skt, &buffer, 1, 0);

        // a znovu...
        recv(client_skt, &buffer, 1, 0);
        printf("Received: %c\n", buffer);
        buffer = 'Z' - (buffer - 'A');
        send(client_skt, &buffer, 1, 0);

        // zavreme sockety - vracime prostredek zpet systemu
        close(client_skt);
    }

    close(skt);

    return 0;
}
