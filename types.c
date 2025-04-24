#include <stdio.h>
#include "memoire.h"
#include "types.h"
#include <string.h>
#include "erreur.h"

sexpr new_integer(int i) {
    sexpr s = (sexpr) valisp_malloc(sizeof(valisp_object));
    s->type = entier;
    s->data.INTEGER = i;
    return s;
}

bool integer_p(sexpr val){
  if(val == NULL) return 0;
  return val->type == entier;
}

int get_integer(sexpr val){
  return val->data.INTEGER;
}

char* chaine_vers_memoire(const char *c) {
    int i;
    char *p = (char *) valisp_malloc(strlen(c) + 1);
    char *debut = p;

    for (i = 0; i < strlen(c) + 1; i++) {
        p[i] = c[i]; 
    }
    return debut;
}

sexpr new_string(char * c){
  sexpr s = (sexpr) valisp_malloc(sizeof(valisp_object));
  s->type = chaine;
  s->data.STRING = chaine_vers_memoire(c);
  return s;
}

sexpr new_symbol(char * c){
  sexpr s;
  if (strcmp(c, "nil") == 0) {
        return NULL;
  }
  s = (sexpr) valisp_malloc(sizeof(valisp_object));
  s->type = symbole;
  s->data.STRING = chaine_vers_memoire(c);
  return s;
}

bool string_p(sexpr val){
  return val != NULL && val->type == chaine;
}

bool symbol_p(sexpr val){
  return val != NULL && val->type == symbole;
} 

char* get_symbol(sexpr val){
  return val->data.STRING;
}

char* get_string(sexpr val){
  return val->data.STRING;
}

bool symbol_match_p(sexpr val, const char *chaine){
  return strcmp(get_string(val) , chaine )==0;
}

sexpr cons(sexpr e1, sexpr e2) {
    sexpr s = (sexpr) valisp_malloc(sizeof(valisp_object));
    s->type = couple;
    s->data.CONS.car = e1;
    s->data.CONS.cdr = e2;
    return s;
}

bool cons_p(sexpr e){
    return e != NULL && e->type == couple;
}

sexpr car(sexpr s) {
    return s->data.CONS.car;
}

sexpr cdr(sexpr s) {
    return s->data.CONS.cdr;
}


bool list_p(sexpr e){
  if(e == NULL) return 1;
  if(cons_p(e) && cdr(e) == NULL) return 1;
  if(cons_p(e) && cons_p(cdr(e))) return 1;
  return 0;
}

void set_car(sexpr e, sexpr nouvelle){
    e->data.CONS.car = nouvelle;
} 

void set_cdr(sexpr e, sexpr nouvelle){
    e->data.CONS.cdr = nouvelle;
}

void afficher_liste(sexpr e) {
    sexpr suite;
    if (!cons_p(e)) {
        afficher(e);
        return;
    }

    afficher(car(e));
    suite = cdr(e);
    if (suite == NULL) {
        return; 
    } else if (cons_p(suite)) {
        printf(" ");
        afficher_liste(suite);
    } else {
        printf(" . ");
        afficher(suite); 
    }
}

void afficher_liste2(sexpr e) {
    sexpr suite;
    if (!cons_p(e)) {
        afficher(e);
        return;
    }

    printf("cons ");
    afficher(car(e));
    suite = cdr(e);
    if (suite == NULL) {
        printf("NULL");
        return; 
    } else if (cons_p(suite)) {
        printf(" (cons ");
        afficher_liste(suite);
        printf(")");
    } else {
        printf(" . ");
        afficher(suite); 
    }
}

void afficher(sexpr v) {
    if (!v) {
        printf("nil");
        return;
    }

    switch (v->type) {
        case entier:
            printf("%d", get_integer(v));
            break;
        case chaine:
            printf("\"%s\"", get_string(v));
            break;
        case symbole:
            printf("%s", get_string(v));
            break;
        case couple:
            printf("(");
            afficher_liste(v);
            printf(")");
            /*
            printf("(");
            afficher_liste2(v);
            printf(")");  
            */
            break;
        case prim:
            printf("#<primitive>");
            break;
        case spec:
            printf("#<speciale>");
            break;
        default:
            printf("<inconnu>");
    }
}

void afficher_type(sexpr v){
    if (!v) {
        printf("nil");
        return;
    }

    switch (v->type) {
        case entier:
            printf("entier");
            break;
        case chaine:
            printf("chaine");
            break;
        case symbole:
            printf("symbole");
            break;
        case couple:
            printf("couple");
            break;
        case prim:
            printf("<primitive>");
            break;
        case spec:
            printf("<speciale>");
            break;
        default:
            printf("<inconnu>");
    }
}



sexpr new_primitive(sexpr (*p)(sexpr, sexpr)){
  sexpr s = (sexpr) valisp_malloc(sizeof(valisp_object));
  s->type = prim;
  s->data.PRIMITIVE = p;
  return s;
}

sexpr new_speciale(sexpr (*p)(sexpr, sexpr)){
  sexpr s = (sexpr) valisp_malloc(sizeof(valisp_object));
  s->type = spec;
  s->data.PRIMITIVE = p;
  return s;
}

bool prim_p (sexpr val) {
  return val && val->type == prim;
}

bool spec_p (sexpr val){
  return val && val->type == spec;
}

sexpr run_prim(sexpr p, sexpr liste, sexpr env) {
  if (!p || (!prim_p(p) && !spec_p(p)) ){
        erreur(TYPAGE,"run_prim","n'est pas une primitive ou une forme spéciale",p);
        return NULL;
    }
    return p->data.PRIMITIVE(liste, env);
}


bool sexpr_equal(sexpr e1, sexpr e2){
    if (e1==NULL && e2==NULL ) return 1;
    if (e1==NULL || e2==NULL ) return 0;
    if (e1->type != e2->type) return 0;

    switch (e1->type) {
      case entier:
          return get_integer(e1) == get_integer(e2);
      case chaine:
          return strcmp(get_string(e1), get_string(e2)) == 0;
      case symbole:
          return strcmp(get_symbol(e1), get_symbol(e2)) == 0;
      case couple:
          return sexpr_equal(car(e1),car(e2)) && sexpr_equal(cdr(e1),cdr(e2));
      default:
          return 1;
    } 
}
