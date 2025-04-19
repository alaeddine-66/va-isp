#include "types.h"
#include <stdio.h>
#include "environnement.h"
#include "erreur.h"
#include "interpreteur.h"



sexpr eval_list(sexpr V, sexpr env){
    sexpr curr, couple;
    for(curr=V; curr!=NULL; curr = cdr(curr)){
        set_car(curr, eval(car(curr), env));
    }
    return V;
}

void afficher_args(sexpr args){
    sexpr curr, ele;
    for(curr=args; curr !=NULL ; curr = cdr(curr)){
        ele = car(curr);
        afficher(ele);
        printf(" ---->  ");
        afficher_type(ele);
        printf("\n");
    }
}

void afficher_apply(sexpr fonction_name , sexpr fonction, sexpr args){
    printf("=== apply ===\n");
    printf("fonction = ");
    afficher(fonction_name);
    printf(" de type ");
    afficher_type(fonction);
    printf("\narguments = ");
    afficher(args);
    printf("\n=============\n");
    afficher_args(args);
    printf("=============\n");

}


sexpr apply(sexpr fonction, sexpr liste, sexpr env){
    sexpr f;
    sexpr nouv_liste;
    sexpr res = NULL;

    if (trouver_variable(env,fonction,&f) == -1){
        erreur(NOM,"eval","Variabe non trouvé", fonction);
    }
    if(spec_p(f)){
        afficher_apply(fonction ,f, liste);
        res = run_prim(f, liste , env);

    }
    else if(prim_p(f)){
        afficher_apply(fonction, f, liste);
        nouv_liste = eval_list(liste,env);
        res = run_prim(f, nouv_liste , env);
    }

    return res;
}

sexpr eval(sexpr val, sexpr env){
    sexpr res;
    if(val == NULL) return NULL;

    if(symbol_p(val)){
        if (trouver_variable(env,val,&res) != -1){
            return res;
        }
        erreur(NOM,get_string(val),"Variabe non trouvé", val);
    }
    if(list_p(val)){
        res = apply(car(val) , cdr(val) ,env);
        return res;
    }
    return val;
}

sexpr bind(sexpr variables, sexpr liste, sexpr env){

    if (symbol_p(variables)) {
        return cons(cons(variables, liste), env);
    }

    while (variables != NULL && liste != NULL) {
        sexpr pair = cons(car(variables), car(liste));
        env = cons(pair, env); // on ajoute à l'environnement
        variables = cdr(variables);
        liste = cdr(liste);
    }

    if (variables != NULL || liste != NULL) {
        erreur(NOM, "bind", "Nombre de paramètres ou d'arguments incorrect", NULL);
        return NULL;
    }

    return env;
}

