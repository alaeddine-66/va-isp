#include "tests.h"
#include "stdio.h"
#include "stdint.h"
#include "allocateur.h"
#include "couleurs.h"
#include "tests.h"
#include "mes_tests.h"

uint32_t cons_bloc(int rm, int precedant, int libre, int suivant);
int bloc_suivant(int i);
int bloc_precedant(int i);
int usage_bloc(int i);
int rm_bloc(int i);
int taille_bloc(int i);

extern bloc MEMOIRE_DYNAMIQUE[];



void allouer_memoire_pour_test(int tailles[], int libres[], int n) {
    /* tailles est de taille n
     * libres  est de taille n+1
     * La dernier bloc verra sa taille calculée automatiquement */
    int i, b;
    int p, k, s; /* p → k → s */

    p=0;
    k=0;
    for (i=0; i<n; i++) {
	s = k + tailles[i] + 1 ;
	b = libres[i];
	MEMOIRE_DYNAMIQUE[k] = cons_bloc(0, p, b, s);
	p = k;
	k = s;
    }
    s = (1<<15) - 1;
    b = libres[i];
    MEMOIRE_DYNAMIQUE[k] = cons_bloc(0, p, b, s);
    MEMOIRE_DYNAMIQUE[s] = cons_bloc(0, k, 1, s);

}

int comparer_memoire_pour_test(int tailles[], int libres[], int n) {
    int res = 1;
    int i, b;
    int p, k, s; /* p → k → s */

    p=0;
    k=0;
    for (i=0; i<n; i++) {
	s = k + tailles[i] + 1 ;
	b = libres[i];
	res = res && (MEMOIRE_DYNAMIQUE[k] == cons_bloc(0, p, b, s));
	p = k;
	k = s;
    }
    s = (1<<15) - 1;
    b = libres[i];
    res = (MEMOIRE_DYNAMIQUE[k] == cons_bloc(0, p, b, s)) && res;
    res = (MEMOIRE_DYNAMIQUE[s] == cons_bloc(0, k, 1, s)) && res;
    return res;
}


#ifdef TEST_ALLOCATEUR_EX1
/****************************/
/*                          */
/*  Initialisation mémoire  */
/*                          */
/****************************/


void test_memoire_vide() {
    int b = 1;
    afficher_titre("Test : mémoire vide");
    initialiser_memoire_dynamique();
    printf("initialiser_memoire_dynamique ");
    b = RUN_TEST(MEMOIRE_DYNAMIQUE[0]     == 32767) && b;
    b = RUN_TEST(MEMOIRE_DYNAMIQUE[32767] == 65535) && b;
    ok_test(b);
    printf("\n");
    afficher_memoire();
    afficher_stat_memoire();
    printf("\n");
}

/*
   +---------------------+
   | Test : mémoire vide |
   +---------------------+

[    0] [ ] 0x558f8a0c1060     0 → 0 → 32767 (32766)
[32767] [x] 0x558f8a0e105c     0 → 32767 → 32767 (0)
0/32768 (0.00 %) 0/1 blocs alloués

 */
#endif


#ifdef TEST_ALLOCATEUR_EX2
/************************/
/*                      */
/*  Fonctions de bases  */
/*                      */
/************************/


void test_cons_bloc() {
    int b = 1;
    printf("cons_bloc     ");
    b = RUN_TEST(cons_bloc(0,   0, 0, 123) == 123)        && b;
    b = RUN_TEST(cons_bloc(0,   0, 1,   0) == 32768)      && b;
    b = RUN_TEST(cons_bloc(0, 123, 0,   0) == 8060928)    && b;
    b = RUN_TEST(cons_bloc(1,   0, 0,   0) == 2147483648) && b;
    b = RUN_TEST(cons_bloc(1,   9, 0,  77) == 2148073549) && b;
    b = RUN_TEST(cons_bloc(0,  97, 1,  23) == 6389783)    && b;
    ok_test(b);
}

int test_un_bloc_suivant(int indice, bloc valeurs) {
    MEMOIRE_DYNAMIQUE[indice] = valeurs;
    return bloc_suivant(indice);
}

void test_bloc_suivant() {
    int b = 1;
    printf("bloc_suivant  ");
    b = RUN_TEST(test_un_bloc_suivant(1, 123)         == 123) && b;
    b = RUN_TEST(test_un_bloc_suivant(2, 32768)       == 0)   && b;
    b = RUN_TEST(test_un_bloc_suivant(3, 8060928)     == 0)   && b;
    b = RUN_TEST(test_un_bloc_suivant(4, 2147483648)  == 0)   && b;
    b = RUN_TEST(test_un_bloc_suivant(5, 2148073549)  == 77)  && b;
    b = RUN_TEST(test_un_bloc_suivant(32767, 6389783) == 23)  && b;
    ok_test(b);
}

int test_un_bloc_precedant(int indice, bloc valeurs) {
    MEMOIRE_DYNAMIQUE[indice] = valeurs;
    return bloc_precedant(indice);
}

void test_bloc_precedant() {
    int b = 1;
    printf("bloc_precedant");
    b = RUN_TEST(test_un_bloc_precedant(1, 123)         == 0)   && b;
    b = RUN_TEST(test_un_bloc_precedant(2, 32768)       == 0)   && b;
    b = RUN_TEST(test_un_bloc_precedant(3, 8060928)     == 123) && b;
    b = RUN_TEST(test_un_bloc_precedant(4, 2147483648)  == 0)   && b;
    b = RUN_TEST(test_un_bloc_precedant(5, 2148073549)  == 9)   && b;
    b = RUN_TEST(test_un_bloc_precedant(32767, 6389783) == 97)  && b;
    ok_test(b);
}

int test_un_usage_bloc(int indice, bloc valeurs) {
    MEMOIRE_DYNAMIQUE[indice] = valeurs;
    return usage_bloc(indice);
}

void test_usage_bloc() {
    int b = 1;
    printf("usage_bloc    ");
    b = RUN_TEST(test_un_usage_bloc(1, 123)         == 0)   && b;
    b = RUN_TEST(test_un_usage_bloc(2, 32768)       == 1)   && b;
    b = RUN_TEST(test_un_usage_bloc(3, 8060928)     == 0)   && b;
    b = RUN_TEST(test_un_usage_bloc(4, 2147483648)  == 0)   && b;
    b = RUN_TEST(test_un_usage_bloc(5, 2148073549)  == 0)   && b;
    b = RUN_TEST(test_un_usage_bloc(32767, 6389783) == 1)  && b;
    ok_test(b);
}

int test_un_rm_bloc(int indice, bloc valeurs) {
    MEMOIRE_DYNAMIQUE[indice] = valeurs;
    return rm_bloc(indice);
}

void test_rm_bloc() {
    int b = 1;
    printf("rm_bloc       ");
    b = RUN_TEST(test_un_rm_bloc(1, 123)         == 0)   && b;
    b = RUN_TEST(test_un_rm_bloc(2, 32768)       == 0)   && b;
    b = RUN_TEST(test_un_rm_bloc(3, 8060928)     == 0)   && b;
    b = RUN_TEST(test_un_rm_bloc(4, 2147483648)  == 1)   && b;
    b = RUN_TEST(test_un_rm_bloc(5, 2148073549)  == 1)   && b;
    b = RUN_TEST(test_un_rm_bloc(32767, 6389783) == 0)   && b;
    ok_test(b);
}



void test_taille_bloc() {
    int b = 1;
    int n  = 6;
    int tailles[6] = {1, 5, 1, 8, 1, 5};
    int libres[7]  = {1, 1, 1, 1, 0, 1, 0};
    int i;
    int k;
    printf("taille_bloc   ");
    allouer_memoire_pour_test(tailles, libres, n);

    k=0;
    for (i=0; i<n; i++) {
	b = RUN_TEST(taille_bloc(k) == tailles[i]) && b;
	k = bloc_suivant(k);
    }
    k = bloc_suivant(k);
    b = RUN_TEST(taille_bloc((1<<15)-1) == 0) && b;
    ok_test(b);

}


void test_allocateurs_fonctions_de_base() {
    afficher_titre("Test allocateurs fonctions de bases");
    test_cons_bloc();
    test_bloc_suivant();
    test_bloc_precedant();
    test_usage_bloc();
    test_rm_bloc();
    test_taille_bloc();
}
#endif




#ifdef TEST_ALLOCATEUR_EX3
/************/
/*          */
/*  Malloc  */
/*          */
/************/


void test_dix_allocations() {
    int b = 1;
    printf("Premier test :");
    initialiser_memoire_dynamique();
    allocateur_malloc(1); /* Sur une case */
    allocateur_malloc(2);
    allocateur_malloc(3);
    allocateur_malloc(4); /* 8*4 = 32 bits */
    allocateur_malloc(5); /* Sur deux cases */
    allocateur_malloc(6);
    allocateur_malloc(7);
    allocateur_malloc(8);
    allocateur_malloc(9); /* Sur trois cases */
    allocateur_malloc(10);

    b = RUN_TEST(MEMOIRE_DYNAMIQUE[ 0] == cons_bloc(0,0,1,2)) && b;
    b = RUN_TEST(MEMOIRE_DYNAMIQUE[ 2] == cons_bloc(0,0,1,4)) && b;
    b = RUN_TEST(MEMOIRE_DYNAMIQUE[ 4] == cons_bloc(0,2,1,6)) && b;
    b = RUN_TEST(MEMOIRE_DYNAMIQUE[ 6] == cons_bloc(0,4,1,8)) && b;
    b = RUN_TEST(MEMOIRE_DYNAMIQUE[ 8] == cons_bloc(0,6,1,11)) && b;
    b = RUN_TEST(MEMOIRE_DYNAMIQUE[11] == cons_bloc(0,8,1,14)) && b;
    b = RUN_TEST(MEMOIRE_DYNAMIQUE[14] == cons_bloc(0,11,1,17)) && b;
    b = RUN_TEST(MEMOIRE_DYNAMIQUE[17] == cons_bloc(0,14,1,20)) && b;
    b = RUN_TEST(MEMOIRE_DYNAMIQUE[20] == cons_bloc(0,17,1,24)) && b;
    b = RUN_TEST(MEMOIRE_DYNAMIQUE[24] == cons_bloc(0,20,1,28)) && b;
    b = RUN_TEST(MEMOIRE_DYNAMIQUE[28] == cons_bloc(0,24,0,32767)) && b;
    b = RUN_TEST(MEMOIRE_DYNAMIQUE[32767] == cons_bloc(0,28,1,32767)) && b;

    ok_test(b);
    printf("\n");

    afficher_memoire();
    afficher_stat_memoire();
    printf("\n");
}

/* Premier test

[    0] [x] 0x558f8a0c1060     0 → 0 → 2     (1)
[    2] [x] 0x558f8a0c1068     0 → 2 → 4     (1)
[    4] [x] 0x558f8a0c1070     2 → 4 → 6     (1)
[    6] [x] 0x558f8a0c1078     4 → 6 → 8     (1)
[    8] [x] 0x558f8a0c1080     6 → 8 → 11    (2)
[   11] [x] 0x558f8a0c108c     8 → 11 → 14    (2)
[   14] [x] 0x558f8a0c1098    11 → 14 → 17    (2)
[   17] [x] 0x558f8a0c10a4    14 → 17 → 20    (2)
[   20] [x] 0x558f8a0c10b0    17 → 20 → 24    (3)
[   24] [x] 0x558f8a0c10c0    20 → 24 → 28    (3)
[   28] [ ] 0x558f8a0c10d0    24 → 28 → 32767 (32738)
[32767] [x] 0x558f8a0e105c    28 → 32767 → 32767 (0)
28/32768 (0.09 %) 10/11 blocs alloués

 */



void test_allocation_memoire_predefinie() {

    int b = 1;

    int n0  = 4;
    int tailles0[4] = {7, 1, 10, 5};
    int libres0[5]  = {0, 1,  0, 1, 0};

    int n1  = 5;
    int tailles1[5] = {7, 1, 8, 1, 5};
    int libres1[6]  = {0, 1, 1, 0, 1, 0};

    int n2  = 6;
    int tailles2[6] = {1, 5, 1, 8, 1, 5};
    int libres2[7]  = {1, 0, 1, 1, 0, 1, 0};

    int n3  = 6;
    int tailles3[6] = {1, 5, 1, 8, 1, 5};
    int libres3[7]  = {1, 1, 1, 1, 0, 1, 0};

    int n4  = 6;
    int tailles4[6] = {1, 5, 1, 8, 1, 5};
    int libres4[7]  = {1, 1, 1, 1, 1, 1, 0};

    printf("second test :");

    /* On alloue la mémoire*/
    allouer_memoire_pour_test(tailles0, libres0, n0);

    /* Je fais malloc de 8 */
    allocateur_malloc(8*sizeof(uint32_t));
    b = RUN_TEST(comparer_memoire_pour_test(tailles1, libres1, n1)) && b;

    /* Je fais malloc de 1 */
    allocateur_malloc(1*sizeof(uint32_t));
    b = RUN_TEST(comparer_memoire_pour_test(tailles2, libres2, n2)) && b;

    /* Je fais malloc de 5 */
    allocateur_malloc(5*sizeof(uint32_t));
    b = RUN_TEST(comparer_memoire_pour_test(tailles3, libres3, n3)) && b;

    /* Je fais malloc de 1 */
    allocateur_malloc(1*sizeof(uint32_t));
    b = RUN_TEST(comparer_memoire_pour_test(tailles4, libres4, n4)) && b;

    /* Je fais malloc de 40 000 */
    b = RUN_TEST(NULL == allocateur_malloc(40000*sizeof(uint32_t))) && b;
    b = RUN_TEST(comparer_memoire_pour_test(tailles4, libres4, n4)) && b;

    ok_test(b);
    printf("\n");
    afficher_memoire();
    afficher_stat_memoire();
    printf("\n");
}


/* Second test

[    0] [x] 0x56287f20c100     0 → 0 → 2     (1)
[    2] [x] 0x56287f20c108     0 → 2 → 8     (5)
[    8] [x] 0x56287f20c120     2 → 8 → 10    (1)
[   10] [x] 0x56287f20c128     8 → 10 → 19    (8)
[   19] [x] 0x56287f20c14c    10 → 19 → 21    (1)
[   21] [x] 0x56287f20c154    19 → 21 → 27    (5)
[   27] [ ] 0x56287f20c16c    21 → 27 → 32767 (32739)
[32767] [x] 0x56287f22c0fc    27 → 32767 → 32767 (0)
27/32768 (0.08 %) 6/7 blocs alloués
 */


void test_allocateur_malloc() {
    afficher_titre("Test Malloc ");
    test_dix_allocations();
    printf("\n");
    test_allocation_memoire_predefinie();



}
#endif


#ifdef TEST_ALLOCATEUR
/************************************/
/*                                  */
/*  Tous les tests en une fonction  */
/*                                  */
/************************************/

void test_allocateur() {
    test_memoire_vide();
    test_allocateurs_fonctions_de_base();
    test_allocateur_malloc();
    afficher_titre("Test ramasse-miettes");
    printf("test ramasse_miettes_liberer %s[TODO]%s\n", couleur_jaune, couleur_defaut);

    afficher_titre("Test free");
    printf("test free %s[TODO]%s\n", couleur_jaune, couleur_defaut);

}
#endif
