#include "types.h"
#include "erreur.h"
#include <stdlib.h>
#include <stdio.h>
#include "environnement.h"
#include "interpreteur.h"

void test_nb_parametres(sexpr liste, char* fonction, int taille) {
    int compteur = 0;
    sexpr courant = liste;

    while (courant && courant->type == couple) {
        compteur++;
        courant = courant->data.CONS.cdr;
    }

    if (courant) {
        erreur(ARITE,fonction,"liste de paramètres mal formée (cdr non nul)",liste);
        exit(1);
    }

    if (compteur != taille) {
        erreur(ARITE,fonction,"mauvais nombre de paramètres",liste);
        exit(1);
    }
}


sexpr add_valisp(sexpr liste, sexpr env) {
    sexpr a;
    sexpr b;
    /* On vérifie l’arité */
    test_nb_parametres(liste, "+", 2);
    /* On récupère les deux paramètres */
    a = car(liste);
    b = car(cdr(liste));
    /* On vérifie de typage */
    if (!integer_p(a)) erreur(TYPAGE, "+", "nécessite un entier", a);
    if (!integer_p(b)) erreur(TYPAGE, "+", "nécessite un entier", b);
    /* On fait le calcul */
    return new_integer(get_integer(a) + get_integer(b));
}

sexpr defvar_valisp(sexpr liste, sexpr env) {
    sexpr nom;
    sexpr exp;
    sexpr res;
    test_nb_parametres(liste,"defvar",2);
    nom = car(liste);
    exp = car(cdr(liste));
    if (!symbol_p(nom)) {
        erreur(TYPAGE,"defvar","Le 1er paramètre doit être un symbole",nom);
    }
    res = eval(exp, env); /* Il faut évaluer à la main le */
    definir_variable_globale(nom,res); /* second paramètre */
    return res;
}

sexpr setq_valisp(sexpr liste, sexpr env) {
    sexpr nom;
    sexpr exp;
    sexpr res;
    test_nb_parametres(liste,"setq",2);
    nom = car(liste);
    exp = car(cdr(liste));
    if (!symbol_p(nom)) {
        erreur(TYPAGE,"setq","Le 1er paramètre doit être un symbole",nom);
    }
    res = eval(exp, env);
    modifier_variable(env,nom,res);
    return res;
}

sexpr quote_valisp(sexpr liste, sexpr env) {
    test_nb_parametres(liste, "quote", 1);
    return car(liste); 
}

sexpr lambda_valisp(sexpr liste, sexpr env) {
    if (!list_p(liste) || cdr(liste) == NULL) {
        erreur(ARITE, "lambda", "Mauvais format pour lambda", liste);
    }
    return cons(new_symbol("lambda"), liste);
}

sexpr if_valisp(sexpr liste, sexpr env) {
    test_nb_parametres(liste, "if", 3);
    sexpr condition = eval(car(liste), env);
    if (condition != NULL) {
        return eval(car(cdr(liste)), env);
    } else {
        return eval(car(cdr(cdr(liste))), env); 
    }
}

sexpr eval_valisp(sexpr liste, sexpr env) {
    test_nb_parametres(liste, "eval", 1);
    return eval(car(liste), env);
}

sexpr apply_valisp(sexpr liste, sexpr env) {
    test_nb_parametres(liste, "apply", 2);
    sexpr fct = car(liste);
    sexpr args = car(cdr(liste));
    return apply(fct, args, env);
}

