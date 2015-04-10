/* sint.c
 * exemple pour TP Xenomai
 * pour compiler : cc -o sint sint.c -lm
 * *************************************/

#include "sint.h"

int fid;

void ecritfic(char * b) {
	write(fid,b,strlen(b));
}

void creationFichier() {
    char buf[100];

    if ((fid = creat("sint.gx",0644)) == -1) {
        perror("Creation sint.xg");
        exit(1);
    }

    sprintf(buf,"TitleText: Test de sinus\n\"Sin(t)\n");
    ecritfic(buf);
}

void fermerFichier() {
	ecritfic("\n");
	close(fid);
}

void construction(double timer) {
    char buf[100];

    sprintf(buf,"%f, %g\n",timer,sin(timer));
    printf("%f, %g\n",timer,sin(timer));
    ecritfic(buf);
}
