#include <stdio.h>
#include "couleurs.h"

/**************************/
/*                        */
/*  Affichage des titres  */
/*                        */
/**************************/



/* strlen mais avec gestions à l’arrache des accents */
int longueur(char *chaine) {
    int i = 0;
    int n = 0;
    unsigned char c;
    while ( (c=chaine[i]) != '\0') {
        if (c>127) i++; /* Non ascii => accent => lettre sur 2 octets*/
        n++;
        i++;
    }
    return n;
}


void afficher_titre(char *titre) {
    int n = longueur(titre);
    int i;
    printf("\n   +");
    for (i=0; i<n+2; i++) printf("-");
    printf("+\n");
    printf("   | %s |\n", titre);
    printf("   +");
    for (i=0; i<n+2; i++) printf("-");
    printf("+\n\n");
}


/******************************/
/*                            */
/*  Pour enchaîner les tests  */
/*                            */
/******************************/

int run_test(int boolean, char *description, char* fichier, int line) {
    if (!boolean) {
	printf("\n%s[TEST ECHOUÉ]%s ", couleur_jaune, couleur_defaut);
	printf("%s ligne %d\n", fichier, line);
	printf("%s\n", description);
    }
    return boolean;
}

void ok_test(int b) {
    if (b)
	printf(" %s[OK]%s\n", couleur_vert, couleur_defaut);
}

#define RUN_TEST(BOOL) run_test(BOOL, #BOOL, __FILE__, __LINE__)
