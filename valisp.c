#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <getopt.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "allocateur.h"
#include "memoire.h"
#include "types.h"
#include "parseur.h"
#include "primitives.h"
#include "interpreteur.h"
#include "environnement.h"
#include "erreur.h"
#include "couleurs.h"


/********************************/
/*                              */
/*  Chargement des primitives   */
/*                              */
/********************************/

void charger_primitives() {

  
  charger_une_primitive("eval",     eval_valisp);
  charger_une_primitive("apply",    apply_valisp);
  charger_une_primitive("car",      car_valisp);
  charger_une_primitive("cdr",      cdr_valisp);
  charger_une_primitive("cons",     cons_valisp);
  charger_une_primitive("+",        add_valisp);
  charger_une_primitive("=",        equal_valisp);
  charger_une_primitive("if",       if_valisp);
  charger_une_primitive("for",      for_valisp);
  charger_une_primitive("liste",    liste_valisp);

  /*
  charger_une_primitive("-",        sub_valisp);
  charger_une_primitive("*",        produit_valisp);
  charger_une_primitive("/",        div_valisp);
  charger_une_primitive("%",        mod_valisp);
  charger_une_primitive("<",        less_than_valisp);
  charger_une_primitive("print",    print_valisp);
  charger_une_primitive("println",  println_valisp);
  charger_une_primitive("type-of",  type_of_valisp);
  */

  charger_une_speciale("defvar",  defvar_valisp);
  charger_une_speciale("setq",    setq_valisp);
  charger_une_speciale("quote", quote_valisp);
  charger_une_speciale("lambda",  lambda_valisp);
  charger_une_speciale("macro",  macro_valisp);


}



/**********/
/*        */
/*  REPL  */
/*        */
/**********/




int valisp_read(char * texte, sexpr* res) {
    int i;
    *res = NULL;
    i = parseur(texte, 0, res);
    i = nettoyer_espaces(texte,i);
    if (i>0 && texte[i] == '\0') return 0;
    return i;
}


#define TAILLE_BUFFER  10000


int ajout_buffer(char* buffer, int position, char * chaine) {
  int i;
  for (i=0; chaine[i] != '\0'; i++) {
    buffer[position+i] = chaine[i];
  }
  buffer[position+i] = '\n';
  buffer[position+i+1] = '\0';
  return position+i+1;
}

void supprime_retour_ligne_finale_buffer(char * buffer) {
  int i;
  for (i=0; buffer[i] != '\0'; i++);
  if (buffer[i-1] == '\n') buffer[i-1] = '\0';
}


void repl_env() {
      afficher_env(environnement_global());
      printf("\n\n");
}

int repl() {
  char* invite_defaut = "\001\033[1;33m\002vaλisp>\001\033[1;0m\002 ";
  char* invite = invite_defaut;
  char* ligne;
  char BUFFER_READ[10000];
  int POSITION = 0;
  sexpr val;
  int res;
  jmp_buf *buf  = jump_buffer();
  sexpr envg = environnement_global();

  using_history();

  while (1) { /* REPL */
      ligne = readline(invite);

      if (ligne == NULL) break;
      if (ligne[0] == '\0')  continue ;
      if (strcmp(ligne, "@env") == 0) { repl_env() ; continue; }
      if (strcmp(ligne, "@mem") == 0) { valisp_stat_memoire(); printf("\n\n"); continue;}
      if (strcmp(ligne, "@mmem")== 0) { afficher_memoire(); continue;}

      if (!setjmp(*buf)) {

          POSITION = ajout_buffer(BUFFER_READ, POSITION, ligne);

          free(ligne);

          res = valisp_read(BUFFER_READ, &val);		  /* READ */

          /* L’expression n’est pas finie, on refait un tour pour la finir */
          if (res == -2)  {
              invite = "";
              continue;
          }

          supprime_retour_ligne_finale_buffer(BUFFER_READ);
          add_history(BUFFER_READ);
          invite = invite_defaut;
          POSITION=0;

          if (res < 0) {
              printf("%s",couleur_rouge);
              printf("Erreur de syntaxe [%d]\n\n", res);
              printf("%s",couleur_defaut);
              continue;
          }

          if (res > 0) {
              printf("%s",couleur_rouge);
              printf("Pas plus d’une sexpr par ligne\n\n");
              printf("%s",couleur_defaut);
              continue;
          }


          /* On a réussi à lire l’expression, on peut l’évaluer */

	        /* Tout ce que valisp affichera sera en bleu*/
          printf("%s",couleur_bleu);
          val = eval(val,environnement_global()); /* EVAL */
          printf("%s",couleur_defaut);

	        /* Et la valeur de retour sera en vert */
          printf("%s",couleur_vert);
          afficher(val);                         /* PRINT */
          printf("%s",couleur_defaut);


      } else {
          /* Si on rencontre une « exception » */
          printf("%s",couleur_rouge);
          POSITION=0;
          afficher_erreur();
          printf("%s",couleur_defaut);
      }

      printf("\n\n");
      valisp_ramasse_miettes(envg);
  }

  return 0;
}




struct option long_options[] = {
  {"debug", no_argument, 0, 'd'},
  {"help", no_argument, 0, 'h'},
  {"load", required_argument, 0, 'l'},
  {"script", required_argument, 0, 's'},
  { NULL, 0, NULL, 0 }
};



char aide[] = "Usage %s [OPTIONS]\n\
\t-l, --load <fichier> (plus tard)\n\
\t-s, --script <fichier> (plus tard)\n\
\t-n, --no-stdlib (plus tard)\n\
\t-h, --help\n\
\t-d, --debug (plus tard)\n";



void afficher_banniere() {
  printf("VAΛISP V.0.0.1\n\n");

/****************************************/
/*                                      */
/*  Ok je me suis un peu fait plaisir   */
/*                                      */
/****************************************/

  printf("%s",couleur_vert);
  printf("           %s__%s                    \n", couleur_jaune, couleur_vert);
  printf("           %s\\ \\%s   _             \n", couleur_jaune, couleur_vert);
  printf("__   ____ _ %s\\ \\%s (_)___ _ __    \n", couleur_jaune, couleur_vert);
  printf("\\ \\ / / _` |%s/  \\%s| / __| '_ \\ \n", couleur_jaune, couleur_vert);
  printf(" \\ V / (_| %s/ /\\%s | \\__ \\ |_) |\n", couleur_jaune, couleur_vert);
  printf("  \\_/ \\__,%s/_/  \\%s|_|___/ .__/  \n", couleur_jaune, couleur_vert);
  printf("                      |_|       \n");
  printf("                                 \n");
  printf("%s",couleur_defaut);
}

int valisp_main(int argc, char *argv[]) {

    int c;
    int option_index = 0;

    while ((c = getopt_long(argc, argv, "l:h", long_options, &option_index)) != -1) {
        switch (c) {
        case 'h':
            printf(aide, argv[0]);
            exit(EXIT_SUCCESS);
        default: /* '?' */
            fprintf(stderr, aide, argv[0]);
            exit(EXIT_FAILURE);
        }
    }


    afficher_banniere();
    initialiser_pile();

    printf("Initialisation mémoire");
    initialiser_memoire();
    printf(" [OK]\n");

    printf("Chargement des primitives");
    charger_primitives();
    printf(" [OK]\n");


    printf("\n");
    c = repl();
    printf("À bientôt\n");

    return c;
}
