/*****
 * Partie Raspberry
 * Pour lancer : ./cliudp <@ip pc> <n° port ecoute>
 *****/

/* Importations */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <netdb.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>

#include <native/task.h>
#include <native/timer.h>

/* Declaration variables statiques */
#define C1SEC 50000000 /* 1 Sec en nano-sec */
#define NB_PERIOD 130/* nombre de période : 130*/

/* Declaration variables globales */
struct sockaddr_in Sock;
int sid;

static void f_task(void *arg) {
    /* Declaration des variables */
    volatile unsigned int i = 0;
    unsigned int n = 0;
    unsigned long int out =0;
    unsigned long long int start = 0;
    RT_TIMER_INFO timer_info;

    unsigned long long int time = 0;
    struct timeval timenow;
    char sec[100];
    int first = 0;
    double timesec = 0;
    double t0 = 0;

    if (rt_task_set_periodic(
                NULL,   /* la tache courrante */
                TM_NOW, /* delai de mise en place, TM_NOW = none */
                C1SEC    /* valeur en nanosecondes */) != 0) {
        printf("[ERROR] - rt_task_set_periodic\n");
            return;
    }

    rt_timer_inquire(&timer_info);
    start = timer_info.date / C1SEC;

    while (n < NB_PERIOD) {
        /* secondary mode */
        rt_timer_inquire(&timer_info);

        /* Affiche un message toutes les secondes */
        // Récupère la donnée de l'horloge
        gettimeofday(&timenow, NULL);

        // On recupere la valeur t0 = premier temps recupere
        if(first == 0) {
            t0 = timenow.tv_sec + (timenow.tv_usec / 1000000.0);
            first++;
        }

        // Pour tous les autres : on récupère la valeur - t0
        timesec = timenow.tv_sec + (timenow.tv_usec / 999998.0) - t0;

        // Conversion de timesec (char) en sec (double)
        sprintf(sec, "%f", timesec);

        // Envoi de la données recupere
        if (sendto(sid, (sec), strlen(sec), 0, (struct sockaddr *)&Sock, sizeof(Sock)) == -1) {
            perror("[ERROR] - sendto");
            return(4);
        }

        n++;
        out = 0;

        /* Retour en primary mode a cause d'un appel systeme Xenomai */
        switch (rt_task_wait_period(&out)) {
            case 0:
            case -ETIMEDOUT:
                if (out) printf("Overrun: %lu cycles perdus\n", out); /* 2nd. mode */
                break;
            default:
                printf("[ERROR] - rt_task_wait_period !\n"); /* 2nd. mode */
                return;
        }
    }
}

/* parametres :
   P[1] = nom de la machine serveur
   P[2] = port
*/
int main(int N, char*P[]) {
    struct hostent *h;

    if (N != 3) {
        fprintf(stderr,"Utilisation : %s nom_serveur port\n", P[0]);
        return(1);
    }

    /* creation du socket */
    if ((sid = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0) {
        perror("[ERROR] - creation socket");
        return(2);
    }

    /* recuperation adresse du serveur */
    if (!(h = gethostbyname(P[1]))) {
        perror(P[1]);
        return(3);
    }

    bzero(&Sock,sizeof(Sock));
    Sock.sin_family = AF_INET;
    bcopy(h->h_addr,&Sock.sin_addr,h->h_length);
    Sock.sin_port = htons(atoi(P[2]));

    /* Declaration de variables */
    RT_TASK t_id;
    int n;

    /* Creation de la tache */
    mlockall(MCL_CURRENT | MCL_FUTURE); /* pour ne pas swapper */
    if ((n = rt_task_spawn(&t_id, "maTache", 0, 99, T_JOINABLE, &f_task, NULL)) != 0) {
        printf("[ERROR] - rt_task_spawn %d!\n",n);
        return 1;
    }

    rt_task_join(&t_id);
    if (sendto(sid, ("FIN"), strlen("FIN"), 0, (struct sockaddr *)&Sock, sizeof(Sock)) == -1) {
        perror("[ERROR] - sendto");
        return(4);
    }

    /* fermeture de la socket */
    close(sid);

    return 0;
}
