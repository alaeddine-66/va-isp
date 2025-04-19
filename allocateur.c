#include "allocateur.h"



/*
MEMOIRE_DYNAMIQUE
*/

#define TAILLE_MEMOIRE_DYNAMIQUE (1<<15)

bloc MEMOIRE_DYNAMIQUE[TAILLE_MEMOIRE_DYNAMIQUE];

int masque = (1<<15);
#define MASQUE_PRECEDENT ( ((1 << 15) - 1) << 16 )
#define MASQUE_SUIVANT ((1 << 15) - 1)

bloc cons_bloc(int rm, int precedant, int libre, int suivant) {
    return (rm << 31) |
           (precedant << 16) |
           (libre << 15)|
           (suivant);
}

void initialiser_memoire_dynamique(){
  MEMOIRE_DYNAMIQUE[0] = cons_bloc(0, 0, 0, TAILLE_MEMOIRE_DYNAMIQUE - 1);
  MEMOIRE_DYNAMIQUE[TAILLE_MEMOIRE_DYNAMIQUE - 1] =
    cons_bloc(0, 0, 1, TAILLE_MEMOIRE_DYNAMIQUE - 1);

}

int bloc_suivant(int i){
  return MEMOIRE_DYNAMIQUE[i] & (masque - 1);
}

int bloc_precedant(int i){
  return (MEMOIRE_DYNAMIQUE[i] >> 16 ) & (masque-1);
}

int usage_bloc(int i){
   return (MEMOIRE_DYNAMIQUE[i] >> 15 ) & 1;
}

int rm_bloc(int i){
  return (MEMOIRE_DYNAMIQUE[i] >> 31 ) & 1;
}

int bloc_libre(int i){
  if (i==TAILLE_MEMOIRE_DYNAMIQUE-1) return 0;
  return rm_bloc(i)==0;
}

int taille_bloc(int i) {
  int suivant = bloc_suivant(i);
  if (suivant == i) return 0;
  return suivant - i - 1;
}

void info_bloc(size_t i) {
    int t = taille_bloc(i);
    int p = bloc_precedant(i);
    int s = bloc_suivant(i);
    char * u = usage_bloc(i) ? "[x]" : "[ ]";
    char * a = bloc_libre(i) ? "est" : "non";

    printf("%s %p %5d -> %ld -> %-5d (%u) %s libre \n",
           u, (void *) &MEMOIRE_DYNAMIQUE[i], p, i, s, t, a);
}

void afficher_memoire() {
    int i;
    for (i=0; bloc_suivant(i) != i; i = bloc_suivant(i)) {
	printf("[%5d] ",i);
	info_bloc(i);
    }
    printf("[%5d] ",i);
    info_bloc(i);
}

void afficher_stat_memoire() {
    size_t i = 0;
    int t = TAILLE_MEMOIRE_DYNAMIQUE;
    int n = 0;
    int b = 0;
    int a = 0;
    for (i=0; bloc_suivant(i) != i; i = bloc_suivant(i)) {
        n+=1;
        if (usage_bloc(i)) {
            a += taille_bloc(i) + 1;
            b += 1;
        }
    }
    printf("%d/%d (%.2f %%) %d/%d blocs alloués", a, t, (100.*a)/t, b,n);
}

void afficher_stat_memoire_bref() {
    size_t i = 0;
    int t = TAILLE_MEMOIRE_DYNAMIQUE;
    int a = 0;
    for (i=0; bloc_suivant(i) != i; i = bloc_suivant(i)) {
      if (usage_bloc(i)) {
            a += taille_bloc(i) + 1;
        }
    }
    printf("%.2f%%", (100.*a)/t);
}


int rechercher_bloc_libre(size_t size){
  int nb_cases = (size + sizeof(bloc) - 1) / sizeof(bloc);
  int i;
  for (i=0; bloc_suivant(i) != i; i = bloc_suivant(i)) {
    if(usage_bloc(i) == 0){
      int t = taille_bloc(i);
      if(t>=nb_cases) return i;
    }
  }
  return -1;
}

void *allocateur_malloc(size_t size) {
  int allocated_next;
  int total_cases, prec, next;

  int nb_cases = (size + sizeof(bloc) - 1) / sizeof(bloc);
  int free_idx = rechercher_bloc_libre(size);
  if (free_idx == -1)
    return NULL;

  total_cases = taille_bloc(free_idx);
  prec = bloc_precedant(free_idx);
  next = bloc_suivant(free_idx);

  if (total_cases > nb_cases) {
    allocated_next = free_idx + nb_cases + 1;
    MEMOIRE_DYNAMIQUE[free_idx] = cons_bloc(0, prec, 1, allocated_next);
    MEMOIRE_DYNAMIQUE[allocated_next] = cons_bloc(0, free_idx, 0, next);
    MEMOIRE_DYNAMIQUE[next] = (MEMOIRE_DYNAMIQUE[next] & ~MASQUE_PRECEDENT) | (allocated_next << 16);
  }else {
    MEMOIRE_DYNAMIQUE[free_idx] = cons_bloc(0, prec, 1, next);
    MEMOIRE_DYNAMIQUE[next] = (MEMOIRE_DYNAMIQUE[next] & ~MASQUE_PRECEDENT) | (free_idx << 16);
  }
  return (void *)&MEMOIRE_DYNAMIQUE[free_idx+1];
}



int ramasse_miette_lire_marque(void * ptr){
  int idx = (bloc*)ptr - MEMOIRE_DYNAMIQUE - 1;
  return rm_bloc(idx);
}

void ramasse_miette_poser_marque(void * ptr){
  int idx = (bloc*)ptr - MEMOIRE_DYNAMIQUE - 1;
  MEMOIRE_DYNAMIQUE[idx] |= (1 << 31);
}

void ramasse_miette_liberer() {
  int next;
  int i;
  for (i=0;bloc_suivant(i) != i;) {
    if(bloc_libre(i)){
      for (next=bloc_suivant(i);next != bloc_suivant(next) && bloc_libre(next); next=bloc_suivant(next));
      MEMOIRE_DYNAMIQUE[i] = cons_bloc(0, bloc_precedant(i), 0, next);
      MEMOIRE_DYNAMIQUE[next] = cons_bloc(0,i,usage_bloc(next),bloc_suivant(next));
      i = bloc_suivant(next);
    }else{
      MEMOIRE_DYNAMIQUE[i] &= ~(1<<31);
      i = bloc_suivant(i);
    }
  }
}

void allocateur_free_bloc(int i) {
  int prec, next;
  for(prec=bloc_precedant(i);prec!= bloc_precedant(prec) && bloc_libre(prec);prec=bloc_precedant(prec));
  for(next=bloc_suivant(i);  next != bloc_suivant(next)  && bloc_libre(next);next=bloc_suivant(next));
  MEMOIRE_DYNAMIQUE[prec] = cons_bloc(0,bloc_precedant(prec),usage_bloc(prec),next);
  MEMOIRE_DYNAMIQUE[next] = cons_bloc(0,prec,usage_bloc(next),bloc_suivant(next));
}

void allocateur_free (void * ptr){
  int idx = (bloc*)ptr - MEMOIRE_DYNAMIQUE;
  allocateur_free_bloc(idx);
}
