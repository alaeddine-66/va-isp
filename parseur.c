#include <stdio.h>
#include "types.h"
#include "erreur.h"
#include "memoire.h"
#include "parseur.h"

int est_espace(char c){
    return c == ' ' || c == '\t' || c == '\n' || c == '\f';
}

int est_chiffre(char c){
    return '0' <= c && c <= '9';
}

int est_symbole(char c){
    return !est_espace(c) && c != '(' && c != ')' && c != '\'' && c != ';' && c != '\0';
}

int fin_mot(char c){
    return est_espace(c) || c == ')' || c == ';';
}

longueur_chaine(char * texte, int deb, int fin){
    int count = 0;
    int i;
    for(i=deb;i<fin;i++){
        count++;
        if(texte[i] == '\\' && i + 1 < fin ) i++;
    } 
    return count;
}

char * sous_chaine(char * texte, int deb, int fin) {
    int i;
    int len = longueur_chaine(texte, deb, fin);
    char * result = (char *) valisp_malloc(len + 1); 
    int j = 0;

    for (i = deb; i < fin; i++) {
        if (texte[i] == '\\' && i + 1 < fin) {
            i++;
            switch (texte[i]) {
                case 'n': result[j++] = '\n'; break;
                case 't': result[j++] = '\t'; break;
                case '\\': result[j++] = '\\'; break;
                case '"': result[j++] = '\"'; break;
                default: result[j++] = texte[i]; break; 
            }
        } else {
            result[j++] = texte[i];
        }
    }
    result[j] = '\0';
    return result;
}

int nettoyer_espaces(char * texte, int i) {
    while (texte[i] != '\0') {
        if (texte[i] == ';') {
            while (texte[i] != '\n' && texte[i] != '\0') i++;
        } else if (est_espace(texte[i])) {
            i++;
        } else {
            break;
        }
    }
    return i;
}

int parse_chaine(char *texte, int i, sexpr *res) {
    char suivant;
    char *brute;
    int deb = i + 1;
    int fin = deb;

    while (texte[fin] != '\0') {
        if (texte[fin] == '\\') {
            fin++; 
            if (texte[fin] != '\0') {
                fin++;
            }
        } else if (texte[fin] == '"') {
            break;
        } else {
            fin++;
        }
    }

    if (texte[fin] != '"') {
        erreur(SYNTAXE, "parse_chaine", "guillemet fermant manquant", NULL);
    }

    suivant = texte[fin + 1];
    if (!fin_mot(suivant)) {
        erreur(SYNTAXE, "parse_chaine", "caractère invalide après chaîne", NULL);
    }

    brute = sous_chaine(texte, deb, fin);
    *res = new_string(brute);

    return fin + 1;
}

int parse_entier(char *texte, int i, sexpr *res) {
    char suivant;
    int signe = 1;
    int val = 0;

    if (texte[i] == '-') {
        signe = -1;
        i++;
    }

    while (est_chiffre(texte[i])) {
        val = val * 10 + (texte[i] - '0');
        i++;
    }

    val *= signe;

    suivant = texte[i];
    if (!est_espace(suivant) && suivant != ')' && suivant != ';') {
        erreur(SYNTAXE, "parse_entier", "Caractère invalide après entier", NULL);
    }

    *res = new_integer(val);

    return i;
}

int parse_symbole(char *texte, int i, sexpr *res) {
    char suivant;
    int deb = i;
    
    while (texte[i] != '\0' && est_symbole(texte[i])) {
        i++;
    }

    suivant = texte[i];
    if (!est_espace(suivant) && suivant != ')' && suivant != ';') {
        erreur(SYNTAXE, "parse_symbole", "Caractère invalide après symbole", NULL);
        return -1;
    }

    char *symbole = sous_chaine(texte, deb, i);
    *res = new_symbol(symbole);

    return i;
}

int parseur_liste(char *texte, int i, sexpr *res) {
    sexpr liste = NULL;
    sexpr *courant = &liste;
    
    i = nettoyer_espaces(texte, i);
    
    while (texte[i] != '\0' && texte[i] != ')') {
        sexpr temp;
        int err = parseur(texte, i, &temp);
        if (err < 0) return err;
        
        *courant = cons(temp, NULL);
        courant = &(*courant)->data.CONS.cdr; 
        
        i = err; 
        i = nettoyer_espaces(texte, i);
    }
    
    if (texte[i] == '\0') {
        erreur(SYNTAXE, "parseur_liste", "Parenthèse fermante manquante", NULL);
        return -1;
    }
    
    *res = liste;
    return i + 1;
}


int parseur(char *texte, int i, sexpr *res) {
    i = nettoyer_espaces(texte, i);
    
    if (texte[i] == '\0') {
        erreur(SYNTAXE, "parseur", "Sexpr vide", NULL);
        return -1;
    }
    
    if (texte[i] == ')') {
        erreur(SYNTAXE, "parseur", "Parenthèse fermante inattendue", NULL);
        return -1;
    }
    
    if (texte[i] == '(') {
        return parseur_liste(texte, i + 1, res);
    }
    
    if (texte[i] == '\'') {
        sexpr temp;
        int err = parseur(texte, i + 1, &temp);
        if (err < 0) return err; 
        *res = cons(new_symbol("quote"), temp);
        return err;
    }
    
    if (texte[i] == '"') {
        return parse_chaine(texte, i, res);
    }
    
    if (est_chiffre(texte[i]) || (texte[i] == '-' && est_chiffre(texte[i + 1]))) {
        return parse_entier(texte, i, res);
    }
    
    return parse_symbole(texte, i, res);
}

