// prg qui fait de l'acquisition sur l'horloge sans utiliser Xenomai
// mais en utilisant les outils orientés temps réel commme setitimer

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

#include "sint.h"

int NOB = 0; // ce numéro indique à la tache non TR quel buffer utiliser - modifier que par la tache TR
struct timeval buffer;

void fct_acq(int S) {
    if (S == SIGALRM) {
        // printf("Signal du timer \n");
        gettimeofday(&buffer, NULL);
        NOB = 1;
    }
}

int main(void) {

    struct itimerval iT;

    // création fichier
    printf("Creation du fichier \n");
    creationFichier();

    signal(SIGALRM, fct_acq);

    /* initialisation des paramètres du timer */

    // Le Délai = 1sec
    iT.it_value.tv_sec = 1;  // en sec
    iT.it_value.tv_usec = 0; // en micor-sec

    // La Période = 1/2 sec
    iT.it_interval.tv_sec = 0;  // en sec
    iT.it_interval.tv_usec = 500000; // en micro-sec

    /* initialisation du timer */
    setitimer(ITIMER_REAL, &iT, NULL);

    // Attend le signal pour sauvegarder un buffer
    // while(1) continue; // boucle infini
    
    // temps initial
    int i=0;
    double t0 = 0;
    double timer = 0;

    while(1) {
        if (NOB) {
            if(i==0) {
                t0 = buffer.tv_sec + (buffer.tv_usec / 1000000.0);
                printf("t0: %f \n", t0);
                i++;
            }

            // récupérer le code du Sint pour faire le graphe
            timer = buffer.tv_sec + (buffer.tv_usec / 999998.0) - t0;
            printf("tx: %f \n",timer);
            construction(timer);

            NOB = 0;
        }

        if(timer > 6.5) {
            break;
        }
    }

    printf("FERMETURE FICHIER");
    fermerFichier();

    return 0;
}
