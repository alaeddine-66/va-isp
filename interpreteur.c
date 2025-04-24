#include "types.h"
#include <stdio.h>
#include "environnement.h"
#include "erreur.h"
#include "interpreteur.h"

sexpr eval_list(sexpr V, sexpr env){
    sexpr tmp, curr;
    sexpr head = NULL;
    sexpr tail = NULL;
    for(curr=V;curr !=NULL ; curr = cdr(curr)){
        tmp = eval(car(curr),env);
        if (head == NULL){
            head = cons(tmp,NULL);
            tail = head;
        }else{
            set_cdr(tail, cons(tmp, NULL));
            tail = cdr(tail);
        }
    }
    return head;
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

void resultat_apply(sexpr res){
    printf("res ---> ");
    afficher(res);
    printf(" de type ");
    afficher_type(res);
    printf("\n=============\n");

}


sexpr apply(sexpr fonction, sexpr liste, sexpr env){
    sexpr f;
    sexpr nouv_liste;
    sexpr res = NULL;
    sexpr variables, lignes, args, nouvel_env, tmp;

    if (trouver_variable(env,fonction,&f) == -1){
        erreur(NOM,"eval","Variabe non trouvé", fonction);
    }
    /*afficher_apply(fonction ,f, liste);*/

    if(spec_p(f)){
        res = run_prim(f, liste , env);

    }

    else if(prim_p(f)){
        nouv_liste = eval_list(liste,env);
        res = run_prim(f, nouv_liste , env);
    }

    else if (list_p(f) && strcmp(get_string(car(f)), "lambda") == 0) {
        variables = car(cdr(f));
        lignes = cdr(cdr(f));

        args = eval_list(liste, env);
        nouvel_env = bind(variables, args, env);

        while (lignes != NULL) {
            tmp = car(lignes);
            res = eval(tmp, nouvel_env);
            lignes = cdr(lignes);
        }
    }

    else if (list_p(f) && strcmp(get_string(car(f)), "macro") == 0) {
        variables = car(cdr(f));
        lignes = cdr(cdr(f));

        nouvel_env = bind(variables, liste, env);

        while (lignes != NULL) {
            res = eval(car(lignes), nouvel_env);
            lignes = cdr(lignes);
        }

        res = eval(res,env);
    }
    
    /*resultat_apply(res);*/

    return res;
}

sexpr eval(sexpr val, sexpr env){
    sexpr res;
    if(val == NULL) return NULL;

    /*
    afficher(val);
    printf(" de type ");
    afficher_type(val);
    printf("\n");
    */

    if(symbol_p(val)){
        if (trouver_variable(env,val,&res) != -1){
            return res;
        }
        erreur(NOM,get_string(val),"Variabe non trouvé", val);
    }
    if (list_p(val)) {
        res = apply(car(val), cdr(val), env);
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
        env = cons(pair, env);
        variables = cdr(variables);
        liste = cdr(liste);
    }

    if (variables != NULL || liste != NULL) {
        erreur(NOM, "bind", "Nombre de paramètres ou d'arguments incorrect", NULL);
        return NULL;
    }

    return env;
}

