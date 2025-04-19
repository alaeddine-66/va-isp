#include "valisp.h"
#include "mes_tests.h"


void lancer_tests() {
    test_memoire_vide();
    test_allocateurs_fonctions_de_base();
    test_allocateur_malloc();



    /* Pour tester les fonctions de la seconde séance */
    /* test_entiers(); */
    /* test_chaines(); */
    /* test_symboles(); */
    /* test_listes(); */
    /* test_cons(); */
    /* test_affichage_liste_cons(); */
    /* test_add(); */


/**********/
/*        */
/*  TP3   */
/*        */
/**********/

    /*test_pointeur_vers_indice();*/


}


int main(int argc, char *argv[]) {
    lancer_tests();
    return valisp_main(argc, argv);
}
