#include "types.h"
#include "erreur.h"
#include <stdlib.h>
#include <stdio.h>
#include "environnement.h"
#include "interpreteur.h"

int nb_parametres(sexpr liste, char* fonction) {
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

    return compteur;
}

void test_nb_parametres(sexpr liste, char* fonction, int taille) {
    int compteur = nb_parametres(liste, fonction);

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

sexpr car_valisp(sexpr liste, sexpr env){
    if(liste == NULL) erreur(ARITE, "car", "Mauvais format pour car", liste);
    return car(liste);
}


sexpr cdr_valisp(sexpr liste, sexpr env){
    if (!list_p(liste) || cdr(liste) == NULL) {
        erreur(ARITE, "lambda", "Mauvais format pour lambda", liste);
    }    
    return car(liste);
}

sexpr cons_valisp(sexpr liste, sexpr env){
    sexpr head, tail;
    if (!list_p(liste) || cdr(liste) == NULL) {
        erreur(ARITE, "lambda", "Mauvais format pour lambda", liste);
    }    
    head = car(liste);
    tail = cdr(liste);
    return cons(head,tail);
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

sexpr macro_valisp(sexpr liste, sexpr env) {
    if (!list_p(liste) || cdr(liste) == NULL) {
        erreur(ARITE, "macro", "Mauvais format pour lambda", liste);
    }
    return cons(new_symbol("macro"), liste);
}


sexpr if_valisp(sexpr liste, sexpr env) {
    sexpr condition, then, else_;

    test_nb_parametres(liste, "if", 3);

    condition = eval(car(liste), env);
    then = car(cdr(liste));
    else_ = car(cdr(cdr(liste)));

    if(!integer_p(condition)) erreur(SYNTAXE, "if", "la condition doit retourner un sexpr de type entier", condition);

    if (get_integer(condition)==1) {
        return eval(then, env);
    } else {
        return eval(else_ , env); 
    }
}

sexpr eval_valisp(sexpr liste, sexpr env) {
    test_nb_parametres(liste, "eval", 1);
    return eval(car(liste), env);
}

sexpr apply_valisp(sexpr liste, sexpr env) {
    sexpr fct, args;
    test_nb_parametres(liste, "apply", 2);
    fct = car(liste);
    args = car(cdr(liste));
    return apply(fct, args, env);
}

sexpr equal_valisp(sexpr liste, sexpr env){
    sexpr a,b;
    test_nb_parametres(liste,"=",2);
    a = car(liste);
    b = car(cdr(liste));
    return new_integer(sexpr_equal(a,b));
}

sexpr for_valisp(sexpr liste, sexpr env) {
    int i;
    int nb_prm;
    sexpr deb, fin, pas;
    sexpr cond, exec;
    sexpr resultat = NULL;
    sexpr couple;

    test_nb_parametres(liste, "for", 2);
    cond = car(liste); 
    exec = car(cdr(liste));
    nb_prm = nb_parametres(cond, "for");

    if (nb_prm == 1) {
        fin = car(cond);
        if (!integer_p(fin)) erreur(TYPAGE, "for", "Nécessite un entier comme fin", fin);
        for (i = 0; i < get_integer(fin); i++) {
            couple = eval(exec, env);
            resultat = cons(couple,resultat);
        }
    }
    else if (nb_prm == 2) {
        deb = car(cond);
        fin = car(cdr(cond)); 
        if (!integer_p(deb)) erreur(TYPAGE, "for", "Nécessite un entier comme deb", deb);
        if (!integer_p(fin)) erreur(TYPAGE, "for", "Nécessite un entier comme fin", fin);
        for (i = get_integer(deb); i < get_integer(fin); i++) {
            couple = eval(exec, env);
            resultat = cons(couple,resultat);
        }
    }
    else if (nb_prm == 3) {
        deb = car(cond);
        fin = car(cdr(cond));
        pas = car(cdr(cdr(cond)));
        if (!integer_p(deb)) erreur(TYPAGE, "for", "Nécessite un entier comme deb", deb);
        if (!integer_p(fin)) erreur(TYPAGE, "for", "Nécessite un entier comme fin", fin);
        if (!integer_p(pas)) erreur(TYPAGE, "for", "Nécessite un entier comme pas", pas);
        for (i = get_integer(deb); i < get_integer(fin); i += get_integer(pas)) {
            couple = eval(exec, env);
            resultat = cons(couple,resultat);
        }
    }
    else {
        erreur(SYNTAXE, "for", "Usage: (for ([deb] fin [pas]) exec)", liste);
    }

    return resultat;
}



sexpr liste_valisp(sexpr args, sexpr env) {
    return args;
}
