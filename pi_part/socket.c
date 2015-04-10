// ./socket 192.168.77.78 81

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

int NOB = 0;
struct timeval bufferTime;

void fct_acq(int S) {
   if (S == SIGALRM) {
     gettimeofday(&bufferTime, NULL);
     NOB = 1;
   }
}


void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    char arr;

    // initialisation des variables
    struct itimerval iT;
    int i = 0;
    int j = 0;
    double t0 = 0;
    double timer = 0;


    signal(SIGALRM, fct_acq);

    // initialisation des paramètres du timer

    // Le délai
    iT.it_value.tv_sec = 1;
    iT.it_value.tv_usec = 0;

    // La période
    iT.it_interval.tv_sec = 0;
    iT.it_interval.tv_usec = 500000;

    // initialisation du timer
    setitimer(ITIMER_REAL, &iT, NULL);

    while(1) {
	if (argc < 3) {
	   fprintf(stderr,"usage %s hostname port\n", argv[0]);
	   exit(0);
	}

	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	    error("ERROR opening socket");
	server = gethostbyname(argv[1]);
	if (server == NULL) {
	    fprintf(stderr,"ERROR, no such host\n");
	    exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
	     (char *)&serv_addr.sin_addr.s_addr,
	     server->h_length);

	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
	    error("ERROR connecting");

	// CODE QUI FONCTIONNE
	if (NOB) {
		// printf("Please enter the message :");
		// fgets(buffer, 255, stdin);
		// n = write(sockfd,buffer,strlen(buffer));

		n = write(sockfd, "toto", 4);
		n = read(sockfd, buffer, 255);
		printf("%s\n", buffer);
		NOB = 0;
        }

        /* bzero(buffer, 256);
	// bzero(buffer, 256);
	if (NOB) {
	  // initialisation du temps de référence (t0)
	  if (i == 0) {
	     t0 = bufferTime.tv_sec + (bufferTime.tv_usec / 1000000.0);
	     printf("t0 : %f \n", t0);
	     i++;
	  }

	  timer = bufferTime.tv_sec + (bufferTime.tv_usec / 999998.0) - t0;

          // *(double*)arr = timer;
          // printf("char: %c \n", arr);

	  printf("tx : %f \n", timer);

	  // n = write(sockfd, "toto", 4);
	  n = write(sockfd, timer, 8);
          // n = write(sockfd,arr,strlen(arr));

	  n = read(sockfd, buffer, 255);
	  printf("%s\n", buffer);
	  if (n < 0) error("ERROR writing to socket");

	  NOB = 0;
	}
	if (timer > 6.5) {
	  // n = write(sockfd,"fin", 3);
	  break;
	}
        */
    }

    return 0;
}
