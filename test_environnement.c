#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "couleurs.h"


#include "tests.h"
#include "mes_tests.h"



int run_test_msg(int bool, char *description, char *explication, char* fichier, int line) {
    if (!bool) {
	printf("\n%s[TEST ECHOUE]%s ", couleur_jaune, couleur_defaut);
	printf("%s ligne %d\n", fichier, line);
	printf("Expression fautive : %s\n", description);
	printf("Indice : %s\n\n", explication);
    }
    return bool;
}

#define RUN_TEST_MSG(MSG,BOOL) run_test_msg(BOOL, #BOOL, MSG, __FILE__, __LINE__)




#ifdef TEST_ENV_INIT
#include "environnement.h"
#include "allocateur.h"
#include "types.h"

void test_environnement_global() {
    int b = 1;
    char* msg;
    extern sexpr ENV;

    printf("Test environnement global");
    fflush(stdout);

    msg = "Environnement global doit renvoyer la variable ENV";
    ENV = NULL;
    b = RUN_TEST_MSG(msg, environnement_global() == ENV) && b;

    msg = "Environnement global doit renvoyer la variable ENV";
    initialiser_memoire_dynamique();
    ENV = cons(new_integer(3), NULL);
    b = RUN_TEST_MSG(msg, environnement_global() == ENV) && b;

    ok_test(b);
}


void test_initialiser_memoire() {
    int b = 1;
    char* msg;
    sexpr env;
    printf("Test initialiser memoire");
    fflush(stdout);

    initialiser_memoire();
    env = environnement_global();
    msg="L'environnement doit valoir : (t . t) -> nil";
    b = RUN_TEST_MSG(msg, cons_p(env))                        && b;
    b = RUN_TEST_MSG(msg, cons_p(car(env)))                   && b;
    b = RUN_TEST_MSG(msg, cdr(env) == NULL)                   && b;
    b = RUN_TEST_MSG(msg, symbol_p(car(car(env))))            && b;
    b = RUN_TEST_MSG(msg, symbol_p(cdr(car(env))))            && b;
    b = RUN_TEST_MSG(msg, symbol_match_p(car(car(env)), "t")) && b;
    b = RUN_TEST_MSG(msg, symbol_match_p(cdr(car(env)), "t")) && b;
    ok_test(b);
}


void test_initialiser_environnement() {
    afficher_titre("Initialiser l'environnement");
    test_environnement_global();
    test_initialiser_memoire();
}


#endif



#ifdef TEST_ENV_MANIP
#include "environnement.h"
#include "allocateur.h"
#include "types.h"


char *env_desc = "(t . t) -> (ch . \"Kikoo\") -> (x . 87) -> (y . 2) -> nil";

void init_environnement_test() {
    extern sexpr ENV;
    sexpr liaison;
    sexpr t;

    /* (t . t) -> (ch . "Kikoo") -> (x . 87) -> (y . 2) -> nil */

    initialiser_memoire_dynamique();
    t = new_symbol("t");

    ENV = NULL;

    liaison = cons(new_symbol("y"), new_integer(2));
    ENV = cons(liaison, ENV);

    liaison = cons(new_symbol("x"), new_integer(87));
    ENV = cons(liaison, ENV);

    liaison = cons(new_symbol("ch"), new_string("Kikoo"));
    ENV = cons(liaison, ENV);

    liaison = cons(t,t);
    ENV = cons(liaison, ENV);
    
}



int tester_env_rech_int(char *nom,  int valeur) {
    int b=1;
    sexpr resultat;
    int err;
    char msg[1000];
    extern sexpr ENV;
    err = trouver_variable(ENV, new_symbol(nom), &resultat);
    sprintf(msg, "«%s» non trouvé dans l'environnement %s", nom, env_desc);
    b = RUN_TEST_MSG(msg, err==0) && b;
    sprintf(msg, "«%s» a une mauvaise valeur dans l'environnement %s", nom, env_desc);
    b = RUN_TEST_MSG(msg, integer_p(resultat)) && b;
    b = RUN_TEST_MSG(msg, get_integer(resultat) == valeur) && b;
    return b;

}

int tester_env_rech_str(char *nom, char *valeur) {
    int b=1;
    sexpr resultat;
    int err;
    char msg[1000];
    extern sexpr ENV;

    err = trouver_variable(ENV, new_symbol(nom), &resultat);
    sprintf(msg, "«%s» non trouvé dans l'environnement %s", nom, env_desc);
    b = RUN_TEST_MSG(msg, err==0) && b;
    sprintf(msg, "«%s» a une mauvaise valeur dans l'environnement %s", nom, env_desc);
    b = RUN_TEST_MSG(msg, string_p(resultat)) && b;
    b = RUN_TEST_MSG(msg, strcmp(get_string(resultat),valeur) == 0) && b;
    return b;

}

int tester_env_rech_sym(char *nom, char *valeur) {
    int b=1;
    sexpr resultat;
    int err;
    char msg[1000];
    extern sexpr ENV;

    err = trouver_variable(ENV, new_symbol(nom), &resultat);
    sprintf(msg, "«%s» non trouvé dans l'environnement %s", nom, env_desc);
    b = RUN_TEST_MSG(msg, err==0) && b;
    sprintf(msg, "«%s» a une mauvaise valeur dans l'environnement %s", nom, env_desc);
    b = RUN_TEST_MSG(msg, symbol_p(resultat)) && b;
    b = RUN_TEST_MSG(msg, strcmp(get_symbol(resultat),valeur) == 0) && b;
    return b;

}


int tester_env_rech_err(char *nom) {
    int b=1;
    sexpr resultat;
    int err;
    char msg[1000];
    extern sexpr ENV;

    err = trouver_variable(ENV, new_symbol(nom), &resultat);
    sprintf(msg, "«%s» trouvé Ã  tort  dans l'environnement %s", nom, env_desc);
    b = RUN_TEST_MSG(msg, err==-1) && b;
    return b;

}

int test_longueur_env() {
    int i;
    sexpr env;
    extern sexpr ENV;
    for (i=0, env=ENV; env != NULL ; i++, env=cdr(env));
    return i;
}


void test_trouver_variable() {
    int b = 1;
    char* msg;
    extern sexpr ENV;
    printf("Test trouver variable\n"); fflush(stdout);
    init_environnement_test();
    b = b && tester_env_rech_int("x", 87);
    b = b && tester_env_rech_int("y", 2);
    b = b && tester_env_rech_str("ch", "Kikoo");
    b = b && tester_env_rech_sym("t", "t");
    b = b && tester_env_rech_err("lol");
    b = b && tester_env_rech_err("z");
    msg = "L'environnement n'a pas la bonne taille";
    b = b && RUN_TEST_MSG(msg, test_longueur_env() == 4);

    ok_test(b);
}


int tester_liaison_sym(sexpr liaison, char* variable, char *valeur) {
    int b = 1;
    char msg[1000];
    sprintf(msg, "Liaison doit etre egale Ã  (%s . %s)", variable, valeur);
    b = b && RUN_TEST_MSG(msg, cons_p(liaison));
    b = b && RUN_TEST_MSG(msg, symbol_p(car(liaison)));
    b = b && RUN_TEST_MSG(msg, symbol_p(cdr(liaison)));
    b = b && RUN_TEST_MSG(msg, symbol_match_p(car(liaison), variable));
    b = b && RUN_TEST_MSG(msg, symbol_match_p(car(liaison), valeur));
    return b;
}

int tester_liaison_str(sexpr liaison, char* variable, char *valeur) {
    int b = 1;
    char msg[1000];
    sprintf(msg, "Liaison doit etre egale Ã  (%s . \"%s\")", variable, valeur);
    b = b && RUN_TEST_MSG(msg, cons_p(liaison));
    b = b && RUN_TEST_MSG(msg, symbol_p(car(liaison)));
    b = b && RUN_TEST_MSG(msg, string_p(cdr(liaison)));
    b = b && RUN_TEST_MSG(msg, symbol_match_p(car(liaison), variable));
    b = b && RUN_TEST_MSG(msg, strcmp(get_string(cdr(liaison)), valeur) == 0);
    return b;
}

int tester_liaison_int(sexpr liaison, char* variable, int valeur) {
    int b = 1;
    char msg[1000];
    sprintf(msg, "Liaison doit etre egale Ã  (%s . %i)", variable, valeur);
    b = b && RUN_TEST_MSG(msg, cons_p(liaison));
    b = b && RUN_TEST_MSG(msg, symbol_p(car(liaison)));
    b = b && RUN_TEST_MSG(msg, integer_p(cdr(liaison)));
    b = b && RUN_TEST_MSG(msg, symbol_match_p(car(liaison), variable));
    b = b && RUN_TEST_MSG(msg, get_integer(cdr(liaison)) == valeur);
    return b;
}


void test_modifier_variable() {
    int b = 1;
    char* msg;
    extern sexpr ENV;
    sexpr env;
    printf("Test modifier variable\n"); fflush(stdout);
    init_environnement_test();
    env_desc = "(t . t) -> (ch . \"Kikoo\") -> (x . 88) -> (y . \"deux\") -> nil";
    modifier_variable(ENV, new_symbol("x"), new_integer(88));
    modifier_variable(ENV, new_symbol("y"), new_string("deux"));

    env=ENV;
    msg = "L'environnement doit etre un CONS";
    b = b && RUN_TEST_MSG(msg, cons_p(env));
    b = b && tester_liaison_sym(car(env), "t", "t");

    env = cdr(env);
    b = b && RUN_TEST_MSG(msg, cons_p(env));
    b = b && tester_liaison_str(car(env), "ch", "Kikoo");

    env = cdr(env);
    b = b && RUN_TEST_MSG(msg, cons_p(env));
    b = b && tester_liaison_int(car(env), "x", 88);

    env = cdr(env);
    b = b && RUN_TEST_MSG(msg, cons_p(env));
    b = b && tester_liaison_str(car(env), "y", "deux");

    env = cdr(env);
    b = b && (env==NULL);
    ok_test(b);
}



void test_definir_variable_globale() {
    int b = 1;
    char* msg;
    extern sexpr ENV;
    sexpr env;
    printf("Test definir variable globale\n"); fflush(stdout);
    init_environnement_test();
    env_desc = "(t . t) â†’ (ch . \"Kikoo\") â†’ (x . 88) â†’ (y . \"deux\") â†’ nil";

    definir_variable_globale(new_symbol("x"), new_integer(88));
    definir_variable_globale(new_symbol("y"), new_string("deux"));

    env=ENV;
    msg = "L'environnement doit étre un CONS";
    b = b && RUN_TEST_MSG(msg, cons_p(env));
    b = b && tester_liaison_sym(car(env), "t", "t");

    env = cdr(env);
    b = b && RUN_TEST_MSG(msg, cons_p(env));
    b = b && tester_liaison_str(car(env), "ch", "Kikoo");

    env = cdr(env);
    b = b && RUN_TEST_MSG(msg, cons_p(env));
    b = b && tester_liaison_int(car(env), "x", 88);

    env = cdr(env);
    b = b && RUN_TEST_MSG(msg, cons_p(env));
    b = b && tester_liaison_str(car(env), "y", "deux");

    env = cdr(env);
    b = b && (env==NULL);
    ok_test(b);
}



void test_manipuler_environnement() {
    afficher_titre("Manipuler l'environnement");
    test_trouver_variable();
    test_modifier_variable();
    test_definir_variable_globale();
}


#endif
