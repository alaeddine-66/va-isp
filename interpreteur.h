#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "types.h"

#ifndef INTERPRETEUR_H
#define INTERPRETEUR_H
sexpr eval_list(sexpr V, sexpr env);
sexpr bind(sexpr variables, sexpr liste, sexpr env);
sexpr apply(sexpr fonction, sexpr liste, sexpr env);
sexpr eval(sexpr val, sexpr env);
#endif
