#include "types.h"
#include "stdio.h"
#include <string.h>
#include "allocateur.h"
#include "couleurs.h"

sexpr ENV = NULL;
sexpr PILE[1000];
int HAUT = 1;

void initialiser_pile(){
    PILE[0]=ENV;
    HAUT=1;
}

sexpr * pile_globale(){
    return PILE;
}

void ajout_pile(sexpr ele){
    PILE[HAUT++] = ele;
}

int get_HAUT(){
    return HAUT;
}


sexpr environnement_global(void){
    return ENV;
}

void initialiser_memoire(void){
    sexpr t;
    sexpr couple;
    initialiser_memoire_dynamique();
    t = new_symbol("t");
    couple = cons(t,t);
    ENV = cons(couple, NULL);
}

int trouver_variable(sexpr env, sexpr variable, sexpr *resultat){
    sexpr curr, nom, val;
    for(curr=env;curr!=NULL; curr = cdr(curr)){
        nom = car(car(curr));
        val = cdr(car(curr));
        if(sexpr_equal(nom,variable)){
            *resultat = val;
            return 0;
        }
    }
    return -1;
}

int modifier_variable(sexpr env, sexpr variable, sexpr valeur){
    sexpr curr;
    for(curr=env;curr!=NULL; curr = cdr(curr)){
        if(car(car(curr))){
            if (sexpr_equal(car(car(curr)), variable)) {
                set_cdr(car(curr),valeur);
                return 0;
            }
        }
    }
    return -1;
}

void definir_variable_globale(sexpr variable, sexpr valeur){
    sexpr couple, nouveau,curr;
    if (modifier_variable(ENV, variable, valeur) == 0) {
        return;
    }

    couple = cons(variable, valeur);
    nouveau = cons(couple, NULL);

    if (ENV == NULL) {
        ENV = nouveau;
    } else {
        curr = ENV;
        while (cdr(curr) != NULL) {
            curr = cdr(curr);
        }
        set_cdr(curr, nouveau);
    }
}


void charger_une_primitive(char* nom, sexpr (*prim)(sexpr, sexpr)){
    sexpr f , chaine;
    f = new_primitive(prim);
    chaine = new_symbol(nom);
    definir_variable_globale(chaine, f);
}


void charger_une_speciale(char* nom, sexpr (*prim)(sexpr, sexpr)){
    sexpr f , chaine;
    f = new_speciale(prim);
    chaine = new_symbol(nom);
    definir_variable_globale(chaine, f);
}

int longueur_env(sexpr env){
    sexpr curr;
    int count=0;
    for(curr=env;curr!=NULL; curr = cdr(curr)){ count ++; }
    return count;
}

void valisp_stat_memoire(void){
    afficher_stat_memoire();
    printf("-> %d variables ",longueur_env(ENV));
}

void afficher_env(sexpr env){
    sexpr curr;
    for(curr=env;curr!=NULL; curr = cdr(curr)){
        printf("%s",couleur_bleu);
        afficher(car((car(curr))));
        printf("%s ",couleur_defaut);
        afficher(cdr((car(curr))));
        printf("\n");
    }
}
