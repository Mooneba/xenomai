/* sint.c */

/* Importation */
#include "sint.h"

/* Declaration des variables globales */
int fid;

/* Ecriture des méthodes */

// Méthode d'écriture
void ecritfic(char * b) {
	write(fid,b,strlen(b));
}

// Méthode de création du fichier
void creationFichier() {
    char buf[100];

    if ((fid = creat("sint.gx",0644)) == -1) {
        perror("Creation sint.xg");
        exit(1);
    }

    sprintf(buf,"TitleText: Sinusoïde\n\"Horloge Xenomai\n");
    ecritfic(buf);
}

// Méthode de fermeture du fichier
void fermerFichier() {
	ecritfic("\n");
	close(fid);
}

// Méthode d'écriture dans le fichier
void construction(double timer) {
    char buf[100];

    sprintf(buf,"%f, %g\n",timer,sin(timer));
    ecritfic(buf);
}
