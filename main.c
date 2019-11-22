/*
Autor: Hector Fernandes
*/

#include <stdio.h>
#include <pthread.h>
#include "bibliotecas/rlutil.h"
#define PX 120
#define PY 30
char parede = '*', outra = 'x', personagem = '@';
int mapa[PY][PX], x = 2, y = 2;

void CriarMapa(){
  for(int i = 0; i < PY; i ++)
    for(int j = 0; j < PX; j++){
      if(i == 0 || i == PY - 1) mapa[i][j] = 1;
      else if(j == 0 || j == PX - 1) mapa[i][j] = 1;
      else if (i == 20 && (j > 8 && j < 20)) mapa[i][j] = 1;
      else if (i == y && j == x) mapa[i][j] = 3;
      else mapa[i][j] = 0;

    }
}

void imprimirMapa(){
  CriarMapa();
  cls();

  for(int i = 0; i < PY; i ++){
    for(int j = 0; j < PX; j++){
      if(mapa[i][j] == 1) printf("%c", parede);
      else if (mapa[i][j] == 2) printf("%c", outra);
      else if (mapa[i][j] == 3) printf("%c", personagem);
      else if (mapa[i][j] == 0) printf(" ");

    }
    printf("\n");
  }
  printf("\n %d", mapa[y + 1][x]);
  printf(" %d ", mapa[y][x - 1]);
  printf(" x: %d ", x);
  printf("y: %d \n", y);
}

void movimento(char mv){

  if (mv == 'a')
    if (mapa[y][x - 1] != 1) --x;
  if (mv == 'd')
    if (mapa[y][x + 1] != 1) ++x;
  if (mv == '^')
    if(mapa[y + 1][x] != 1) ++y;
  if (mv == 'l') y = 2;

}

void *gravidade(){

  while(1){
    if(mapa[y + 1][x] != 1){
      movimento('^');
      imprimirMapa();
      usleep(200000);
    }
  }
}


int main(){
  pthread_t gravity;
  char mov;
  imprimirMapa();

  pthread_create(&gravity, NULL, gravidade, NULL);


  while(mov != 'p'){
    mov = getch();
    movimento(mov);
    imprimirMapa();
  }
  return 0;
}
