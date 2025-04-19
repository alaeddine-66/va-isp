#include "types.h"

#ifndef PARSEUR_H
#define PARSEUR_H
int nettoyer_espaces(char * texte, int i);
int parseur(char* texte, int i,  sexpr* res);
#endif
