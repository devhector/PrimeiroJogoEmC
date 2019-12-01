/*
Autor: Hector Fernandes
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "bibliotecas/rlutil.h"
#define PX 120
#define PY 30
char parede = '*', outra = 'x', person = '@', ini = '&';
int mapa[PY][PX], G;

typedef struct{
  int x;
  int y;
}pos;

pos personagem, inimigo;


void CriarMapa(){
  for(int i = 0; i < PY; i ++)
    for(int j = 0; j < PX; j++){
      if(i == 0 || i == PY - 1) mapa[i][j] = 1;
      else if(j == 0 || j == PX - 1) mapa[i][j] = 1;
      else if (i == 20 && (j > 8 && j < 20)) mapa[i][j] = 1;
      else mapa[i][j] = 0;

    }
}

void imprimirMapa(){
  CriarMapa();
  system("clear");

  for(int i = 0; i < PY; i ++){
    for(int j = 0; j < PX; j++){
      if(mapa[i][j] == 1) printf("%c", parede);
      else if (mapa[i][j] == 2) printf("%c", outra);
      else if ((i == personagem.y)&&(j == personagem.x)) printf("%c", person);
      //else if (mapa[i][j] == 4) printf("%c", ini);
      else if (mapa[i][j] == 0) printf(" ");

    }
    printf("\n");
  }
  printf("\n %d", mapa[personagem.y + 1][personagem.x]);
  printf(" %d ", mapa[personagem.y][personagem.x - 1]);
  printf(" x: %d ", personagem.x);
  printf("y: %d \n", personagem.y);
}

void pulo (){
  while (mapa[personagem.y][personagem.x] != 1)
  {
    /* code */
  }

}

void movimento(char mv){

  if (mv == 'a')
    if (mapa[personagem.y][personagem.x - 1] != 1) --personagem.x;
  if (mv == 'd')
    if (mapa[personagem.y][personagem.x + 1] != 1) ++personagem.x;
  if (mv == '^')
    if(mapa[personagem.y + 1][personagem.x] != 1) ++personagem.y;
  if (mv == 'l') personagem.y = 2;
  if (mv == 'w')
    if (G == 0)
      G = 3;


}

void *gravidade(){

  while(1){
    usleep(29000);
    
    imprimirMapa();

    if((personagem.x == inimigo.x)&&(personagem.y == inimigo.y)){  
      personagem.x = 2;
      personagem.y = 2;
    }

    if(G == 0){
      if(mapa[personagem.y + 1][personagem.x] != 1){
        //movimento('^');
        if(mapa[inimigo.y + 1][inimigo.x] != 1) ++inimigo.y;
        ++personagem.y;
        imprimirMapa();
        usleep(50000);
        
      }
    }else {
      if(1/*mapa[y - 1][x] != 1*/){
        personagem.y--;
        imprimirMapa();
        usleep(50000);
        
        G--;
      }
    }
  }
}


int main(){
  personagem.x = 2;
  personagem.y = 2;
  inimigo.x = 14;
  inimigo.y = 2;
  pthread_t gravity;
  char mov;
  imprimirMapa();

  pthread_create(&gravity, NULL, gravidade, NULL);


  while(mov != 'p'){

    mov = getch();
    movimento(mov);
    //imprimirMapa();
  }
  return 0;
}
