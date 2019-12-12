/*
Autores: Eduardo Nicoletti
         Hector Fernandes
         Maurício Mucci
         Willian Rodrigues
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "bibliotecas/rlutil.h"
#include "bibliotecas/funcoesjogo.h"


int main(){

  //char *fases_custom[5];

  hidecursor();
  char c;
  while(1){
    c = menu();
    switch(c){
      case(0):
        iniciar();
        break;
      case(1):
        cls();
        printf("Ainda não implementado...");
        getch();
        cls();
        break;
      case(2):
        cls();
        printf("Ainda não implementado...");
        getch();
        cls();
        break;
      case(3):
        return 0;
    }
    fase_atual = 0;
    fflush(stdin);

  }
}
