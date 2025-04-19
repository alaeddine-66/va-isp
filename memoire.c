#include "allocateur.h"
#include "erreur.h"
#include "environnement.h"

void * valisp_malloc(size_t size){
  int *p;
  p = allocateur_malloc(size);
  if(p==NULL) ERREUR_FATALE("Pas d'espace pour valisp malloc");
  return p;
}

void ramasse_miette_parcourir_et_marquer(sexpr env) {
    if (env == NULL) return;

    if (ramasse_miette_lire_marque(env)) return;

    ramasse_miette_poser_marque(env);

    if (cons_p(env)) {
        ramasse_miette_parcourir_et_marquer(car(env));
        ramasse_miette_parcourir_et_marquer(cdr(env));
    }
}

void valisp_ramasse_miettes(sexpr env) {
    ramasse_miette_parcourir_et_marquer(env);
    ramasse_miette_liberer();
}
