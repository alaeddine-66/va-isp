#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "allocateur.h"
#include "types.h"
#include "tests.h"
#include "mes_tests.h"




extern uint32_t MEMOIRE_DYNAMIQUE[];


int appartient_a_memoire_dynamique(void * p) {
    uint32_t *q = (uint32_t *) p;
    uint32_t *deb = MEMOIRE_DYNAMIQUE;
    uint32_t *fin = MEMOIRE_DYNAMIQUE + (1<<15);
    return (deb <= q && q < fin);
}


#ifdef TEST_TYPES_ENTIER
/**********************/
/*                    */
/*  Test des entiers  */
/*                    */
/**********************/


void test_un_entier(int i) {
    int b = 1;
    sexpr e = new_integer(i);
    printf("%d = «", i);
    afficher(e);
    printf("»");
    b = RUN_TEST(integer_p(e))                      && b;
    b = RUN_TEST(get_integer(e)==i)                 && b;
    b = RUN_TEST(appartient_a_memoire_dynamique(e)) && b;
    ok_test(b);
}


void test_entiers() {
    int i;
    afficher_titre("Test entiers");
    initialiser_memoire_dynamique();

    for (i=-5 ; i<= 5; i++) {
	test_un_entier(i);
    }

    RUN_TEST(!integer_p(NULL));
}
#endif


#ifdef TEST_TYPES_CHAINE
/**********************/
/*                    */
/*  Test des chaînes  */
/*                    */
/**********************/

void test_une_chaine(char * c) {
    int b = 1;
    sexpr e = new_string(c);
    printf("\"%s\" = «", c);
    afficher(e);
    printf("»");
    b = RUN_TEST(string_p(e))                                   && b;
    b = RUN_TEST(strcmp(get_string(e), c) == 0)                 && b;
    b = RUN_TEST(appartient_a_memoire_dynamique(e))             && b;
    b = RUN_TEST(appartient_a_memoire_dynamique(get_string(e))) && b;
    ok_test(b);
}

void test_chaines() {
    afficher_titre("Test Chaînes");
    initialiser_memoire_dynamique();
    test_une_chaine("Bonjour");
    test_une_chaine("");
    test_une_chaine("Youpi !");
    RUN_TEST(!string_p(NULL));
}
#endif


#ifdef TEST_TYPES_SYMBOLE
/************************/
/*                      */
/*  Test des symboles   */
/*                      */
/************************/

void test_un_symbole(char *c) {
    int b = 1;
    sexpr e = new_symbol(c);
    printf("\"%s\" = «", c);
    afficher(e);
    printf("»");
    b = RUN_TEST(symbol_p(e))                                   && b;
    b = RUN_TEST(strcmp(get_symbol(e), c) == 0)                 && b;
    b = RUN_TEST(symbol_match_p(e, c))                          && b;
    b = RUN_TEST(appartient_a_memoire_dynamique(e))             && b;
    b = RUN_TEST(appartient_a_memoire_dynamique(get_string(e))) && b;
    ok_test(b);
}

void test_symboles() {
    int b=1;
    afficher_titre("Test Symboles");
    initialiser_memoire_dynamique();
    test_un_symbole("defun");
    test_un_symbole("while");
    test_un_symbole("+");
    test_un_symbole("<");
    test_un_symbole("x");
    printf("nil est codé par NULL et n’est pas un symbole ");
    b = RUN_TEST(new_symbol("nil")==NULL) && b;
    b = RUN_TEST(!symbol_p(NULL))         && b;
    ok_test(b);

}
#endif


#ifdef TEST_TYPES_LISTE
/**********************/
/*                    */
/*  Test des listes   */
/*                    */
/**********************/


void test_une_liste_entier(int tab[], int taille) {
    sexpr e = NULL;
    int i;
    int b = 1;
    initialiser_memoire_dynamique();
    printf("[");
    for (i=0; i<taille; i++) {
	printf("%d", tab[i]);
	if (i+1<taille) printf(", ");
    }
    printf("] : ");
    for (i=taille-1 ; i>=0 ; i--) {
	e = cons(new_integer(tab[i]) ,e);
	b = RUN_TEST(cons_p(e)) && b;
	b = RUN_TEST(list_p(e)) && b;
    }
    afficher(e);
    ok_test(b);
}


void test_une_liste_symbole(char* tab[], int taille) {
    sexpr e = NULL;
    int i;
    int b = 1;
    initialiser_memoire_dynamique();
    printf("[");
    for (i=0; i<taille; i++) {
	printf("%s", tab[i]);
	if (i+1<taille) printf(", ");
    }
    printf("] : ");
    for (i=taille-1 ; i>=0 ; i--) {
	e = cons(new_symbol(tab[i]) ,e);
	b = RUN_TEST(cons_p(e))                         && b;
	b = RUN_TEST(list_p(e))                         && b;
	b = RUN_TEST(appartient_a_memoire_dynamique(e)) && b;

    }
    afficher(e);
    ok_test(b);
}


void test_cons() {
    int b = 1;
    sexpr e;
    afficher_titre("Test cons");
    initialiser_memoire_dynamique();
    e = cons(new_integer(3), new_string("trois"));
    b = RUN_TEST(!list_p(e))                             && b;
    b = RUN_TEST(cons_p(e))                              && b;
    b = RUN_TEST(!cons_p(NULL))                          && b;
    b = RUN_TEST(get_integer(car(e)) == 3)               && b;
    b = RUN_TEST(strcmp(get_string(cdr(e)), "trois")==0) && b;
    afficher(e);
    ok_test(b);
    b = 1;
    set_car(e, new_string("deux"));
    set_cdr(e, new_integer(2));
    b = RUN_TEST(!list_p(e))                            && b;
    b = RUN_TEST(cons_p(e))                             && b;
    b = RUN_TEST(get_integer(cdr(e)) == 2)              && b;
    b = RUN_TEST(strcmp(get_string(car(e)), "deux")==0) && b;
    afficher(e);
    ok_test(b);
}

void test_affichage_liste_cons() {
    sexpr a;
    sexpr b;
    sexpr c;
    sexpr d;
    afficher_titre("Test affichage listes et cons");
    initialiser_memoire_dynamique();
    a = cons(new_integer(1), new_integer(2));
    b = cons(new_integer(1), cons(new_integer(2), NULL));
    c = cons(new_integer(1), cons(new_integer(2), cons(new_integer(3),NULL)));
    d = cons(new_integer(1), cons(new_integer(2), cons(new_integer(3),new_integer(4))));
    printf("Dans les lignes suivantes : on doit avoir le même affichage.\n");
    printf("<objectif> : <votre code>\n\n");
    printf("nil : "); afficher(NULL) ; printf("\n");
    printf("(1 . 2) : "); afficher(a) ; printf("\n");
    printf("(1 2) : "); afficher(b) ; printf("\n");
    printf("(1 2 3) : "); afficher(c) ; printf("\n");
    printf("(1 2 3 . 4) : "); afficher(d) ; printf("\n");
}

void test_listes() {
    int tab1[] = {11, 22, 33};
    char* tab2[] = {"defun", "x", "kikoo"};
    afficher_titre("Test Listes");
    test_une_liste_entier(tab1, 0);
    test_une_liste_entier(tab1, 1);
    test_une_liste_entier(tab1, 2);
    test_une_liste_entier(tab1, 3);
    test_une_liste_symbole(tab2, 0);
    test_une_liste_symbole(tab2, 1);
    test_une_liste_symbole(tab2, 2);
    test_une_liste_symbole(tab2, 3);
}
#endif

#ifdef TEST_TYPES
/************************************/
/*                                  */
/*  Tous les tests en une fonction  */
/*                                  */
/************************************/

void test_types() {
    test_entiers();
    test_chaines();
    test_symboles();
    test_listes();
    test_cons();
    test_affichage_liste_cons();
}
#endif
