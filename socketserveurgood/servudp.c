/*****
 * Partie Raspberry
 * Pour lancer : ./servudp
 * Recuperer le port rattache
 *****/

/* Importations */
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "sint.h"

/* Declaration variables statiques */
#define LBUF 1024
#define PORT 9999

/* Declaration variables globales */
struct sockaddr_in Sock = {AF_INET}; // le reste est nul

char * add_ip(unsigned int A) {
   static char b[16];
   sprintf(b,"%u.%u.%u.%u",(unsigned int)(A>>24),(unsigned int)((A>>16)&0xFF),
             (unsigned int)((A>>8)&0xFF),(unsigned int)(A&0xFF));
   return b;
}

int main(int N, char*P[]) {
    /* Declaration variables */
    int sid, lg, n;
    char buf[LBUF];
    struct sockaddr_in Srec;

    /* creation du socket */
    if ((sid = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0) {
        perror("socket");
        exit(1);
    }

    /* si on veut forcer le no du port alors decommenter ce qui suit
    Sock.sin_port = htons(PORT);
    */

    /* attachement du socket */
    if (bind(sid, (struct sockaddr*)&Sock, sizeof(Sock)) < 0) {
       perror("bind");
       exit(2);
    }

    /* on recupere le numero du port */
    lg = sizeof(Sock);
    if (getsockname(sid,(struct sockaddr *)&Sock,(socklen_t*)&lg) < 0) {
        perror("getsockname");
        exit(3);
    }

    // Affichage du numero du port
    printf("Le serveur est attache au port %u\n", ntohs(Sock.sin_port));

    /* Fonciton dans sint.h => Creation du fichier sint.gx */
    creationFichier();

    /******
     * Boucle de lecture des messages des clients
     ******
     * Dernier message envoye : "FIN"
     * On boucle jusqu'à ce que le premier caractere soit un 'F'
     ******/
    while (buf[0]!='F') {
        lg = sizeof(Srec);
        n = recvfrom(sid,buf,LBUF,0,(struct sockaddr *)&Srec,(socklen_t*)&lg);
        if (n < 0) perror("recvfrom");
        else {
          // On ne construit pas de point lors de la réception "FIN"
          if (buf[0]!='F') {
            buf[n] = '\0';
            /* Fonciton dans sint.h => Insertion des données dans le fichier sint.gx */
            construction(atof(buf));
          }
        }
    }

    /* Fonciton dans sint.h => Fermeture du fichier sint.gx */
    fermerFichier();

    // Fermeture de la socket
    close(sid);
}
