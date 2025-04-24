#include "allocateur.h"
#include "erreur.h"
#include "environnement.h"
#include "memoire.h"

void * valisp_malloc(size_t size){
  int *p;
  p = allocateur_malloc(size);
  if(p==NULL){
    valisp_ramasse_miettes_dynamiques(pile_globale());
    p = allocateur_malloc(size);
    if(p==NULL) ERREUR_FATALE("Pas d'espace pour valisp malloc");
  } 
  ajout_pile((sexpr)p);
  return p;
}

void ramasse_miette_parcourir_et_marquer(sexpr env) {
    if (env == NULL) return;

    if (ramasse_miette_lire_marque(env)) return;

    ramasse_miette_poser_marque(env);

    if (symbol_p(env) || string_p(env) ){
        if (env->data.STRING != NULL) {
            ramasse_miette_poser_marque((void *)env->data.STRING);
        }
    }

    if (cons_p(env)) {
        ramasse_miette_parcourir_et_marquer(car(env));
        ramasse_miette_parcourir_et_marquer(cdr(env));
    }
}

void ramasse_miette_parcourir_et_marquer_pile(sexpr * pile){
    int i;
    for(i=0;i<get_HAUT();i++){
        ramasse_miette_parcourir_et_marquer(pile[i]);
    }
}

void valisp_ramasse_miettes_dynamiques(sexpr  * pile) {
    ramasse_miette_parcourir_et_marquer_pile(pile);
    ramasse_miette_liberer();
}

void valisp_ramasse_miettes(sexpr  env) {
    ramasse_miette_parcourir_et_marquer(env);
    ramasse_miette_liberer();
}
