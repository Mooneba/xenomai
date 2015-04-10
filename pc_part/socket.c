/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
   gcc server2.c -lsocket
*/

// code : ./socket <port> => ./socket 81

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "sint.h" 

void dostuff(int); /* function prototype */
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen, pid;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

     creationFichier();

     int i =0;
     int n;
     char buffer[256];
     double bufferDouble;

     while (1) {
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);

         bzero(buffer,256);
         n = read(newsockfd,buffer,255);
         if (n < 0) error("ERROR reading from socket");

         if(buffer == "fin") {
             close(sockfd);
             break;
         } else {
             // sscanf(buffer, "%f", &bufferDouble);
             // printf("Here is the message: %f\n",bufferDouble);
             printf("Here is the message: %s\n",buffer);
         }
         n = write(newsockfd,"I got your message",18);
         //printf("Try to send...");
         //if (n < 0) error("ERROR writing to socket");
     } /* end of while */

     fermerFichier();

     return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
   int n;
   char buffer[256];
   double bufferDouble;
      
   /*bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");*/
   printf("Here is the message: %s\n",buffer);

   //sscanf(buffer, "%f", &bufferDouble);
   //construction(bufferDouble);

   n = write(sock,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
}