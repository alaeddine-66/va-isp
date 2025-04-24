#include "types.h"

#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H


sexpr environnement_global(void);
void initialiser_memoire(void);

void initialiser_pile();
sexpr * pile_globale();
void ajout_pile(sexpr p);
int get_HAUT();

int trouver_variable(sexpr env, sexpr variable, sexpr *resultat);
int modifier_variable(sexpr env, sexpr variable, sexpr valeur);
void definir_variable_globale(sexpr variable, sexpr valeur);
void charger_une_primitive(char* nom, sexpr (*prim)(sexpr, sexpr));
void charger_une_speciale(char* nom, sexpr (*prim)(sexpr, sexpr));


int longueur_env(sexpr env);
void valisp_stat_memoire(void);
void afficher_env(sexpr env);

#endif

