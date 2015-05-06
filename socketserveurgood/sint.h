/* sint.h */

/* Importations */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <string.h>

/* Declaration variables statiques */
#define NBP 629

/* Declaration des méthodes */
void ecritfic(char * b);
void creationFichier();
void fermerFichier();
void construction(double timer);
