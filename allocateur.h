#include <stdint.h>
#include <stdio.h>

#ifndef ALLOCATEUR_H
#define ALLOCATEUR_H

typedef uint32_t bloc ;

void initialiser_memoire_dynamique();
void info_bloc(size_t i);
void afficher_memoire();
void afficher_stat_memoire();
void afficher_stat_memoire_bref();
void *allocateur_malloc(size_t size);
int ramasse_miette_lire_marque(void * ptr);
void ramasse_miette_poser_marque(void *ptr);
void ramasse_miette_liberer();
void allocateur_free (void * ptr);


#endif
