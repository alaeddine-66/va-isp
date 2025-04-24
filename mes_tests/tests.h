#ifndef TESTS_H
#define TESTS_H

void afficher_titre(char *titre);


int run_test(int boolean, char *description, char* fichier, int line);
#define RUN_TEST(BOOL) run_test(BOOL, #BOOL, __FILE__, __LINE__)

void ok_test(int b);




#endif
