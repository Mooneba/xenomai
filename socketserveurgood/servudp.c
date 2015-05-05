/*************************************************
* exemple de serveur UDP
* socket non connecte
*************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "sint.h"


#define LBUF 1024
#define PORT 9999

struct sockaddr_in Sock = {AF_INET}; /* le reste est nul */

char * add_ip(unsigned int A) {
   static char b[16];
   sprintf(b,"%u.%u.%u.%u",(unsigned int)(A>>24),(unsigned int)((A>>16)&0xFF),
             (unsigned int)((A>>8)&0xFF),(unsigned int)(A&0xFF));
   return b;
}


int main(int N, char*P[]) {
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

    /* on recupere le no du port */
    lg = sizeof(Sock);
    if (getsockname(sid,(struct sockaddr *)&Sock,(socklen_t*)&lg) < 0) {
        perror("getsockname");
        exit(3);
    }
    printf("Le serveur est attache au port %u\n", ntohs(Sock.sin_port));
    creationFichier();

    /* boucle de lecture des messages des clients */
    do {
        lg = sizeof(Srec);
        n = recvfrom(sid,buf,LBUF,0,(struct sockaddr *)&Srec,(socklen_t*)&lg);
        if (n < 0) perror("recvfrom");
        else {
          buf[n] = '\0';
          construction(atof(buf));
          // printf("Recu de %s : <%s>\n",add_ip(ntohl(Srec.sin_addr.s_addr)),buf);
        }
    } while (buf[0]!='F');

    fermerFichier();
    close(sid);
}
