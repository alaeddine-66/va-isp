#include <stdio.h>
#include "types.h"
#include "stdlib.h"
#include "couleurs.h"
#include "erreur.h"

/**********************/
/*                    */
/*  Erreurs Fatales   */
/*                    */
/**********************/

/* Pour s’arrêter lorsque le développeur tombe sur cas qui ne devrait
   jamais arriver. Normalement, si le programme est écrit sans bugs,
   les erreurs fatales ne devrait jamais être rencontrées. */

jmp_buf buf;

jmp_buf * jump_buffer(){
    return &buf;
}   

void erreur_fatale(char  *fichier, int ligne, char* causes) {
    fprintf(stderr,"%s", couleur_rouge);
    fprintf(stderr,"\n   /!\\ Erreur fatale /!\\");
    fprintf(stderr,"%s", couleur_defaut);
    fprintf(stderr,"   %s ligne %d\n\n", fichier, ligne);
    fprintf(stderr,"%s\n", causes);
    exit(1);
}

char *noms_erreurs[] = {
    "TYPAGE",
    "ARITE",
    "NOM",
    "MEMOIRE",
    "DIVISION_PAR_ZERO",
    "SYNTAXE",
    "MEMOIRE_PARSEUR",
    "RUNTIME"
};

sexpr SEXPR_ERREUR;
char * FONCTION_ERREUR;
char * MESSAGE_ERREUR;
enum erreurs TYPE_ERREUR;

void afficher_erreur(void){
    printf("%s", couleur_rouge);
    printf("Erreur d’exécution [%s] : %s\n",noms_erreurs[TYPE_ERREUR] , MESSAGE_ERREUR);
    printf("Fonction fautive : « %s »\n", FONCTION_ERREUR);
    printf("Valeur fautive : «");
    afficher(SEXPR_ERREUR);
    printf("»");
}

void erreur(enum erreurs type, char* fonction, char * explication, sexpr s){
    SEXPR_ERREUR = s;
    FONCTION_ERREUR =fonction;
    MESSAGE_ERREUR = explication;
    TYPE_ERREUR = type;
    
    longjmp(*jump_buffer(), 1);

}
