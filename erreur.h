#include "types.h"
#include <setjmp.h>


#ifndef VALISP_ERREUR_H
#define VALISP_ERREUR_H

void erreur_fatale(char  *fichier, int ligne, char* causes);
#define ERREUR_FATALE(CAUSE) erreur_fatale( __FILE__, __LINE__, CAUSE) 

enum erreurs {
    TYPAGE, 
    ARITE, 
    NOM,  
    MEMOIRE,
    DIVISION_PAR_ZERO, 
    SYNTAXE, 
    MEMOIRE_PARSEUR,
    RUNTIME 
};

void afficher_erreur(void);
void erreur(enum erreurs type, char* fonction, char * explication, sexpr s);

jmp_buf *jump_buffer();

#endif
