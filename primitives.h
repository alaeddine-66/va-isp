
#ifndef PRIMITIVE_H
#define PRIMITIVE_H


void test_nb_parametres(sexpr liste, char* fonction, int taille);
sexpr add_valisp(sexpr liste, sexpr env);
sexpr defvar_valisp(sexpr liste, sexpr env);
sexpr setq_valisp(sexpr liste, sexpr env);
sexpr quote_valisp(sexpr liste, sexpr env);
sexpr lambda_valisp(sexpr liste, sexpr env);
sexpr macro_valisp(sexpr liste, sexpr env);
sexpr if_valisp(sexpr liste, sexpr env);
sexpr eval_valisp(sexpr liste, sexpr env);
sexpr apply_valisp(sexpr liste, sexpr env);
sexpr car_valisp(sexpr liste, sexpr env);
sexpr cdr_valisp(sexpr liste, sexpr env);
sexpr cons_valisp(sexpr liste, sexpr env);

sexpr equal_valisp(sexpr liste, sexpr env);
sexpr for_valisp(sexpr liste, sexpr env);
sexpr liste_valisp(sexpr liste, sexpr env);


#endif
