/*
Autores: Eduardo Nicoletti
         Hector Fernandes
         Maurício Mucci
         Willian Rodrigues
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "bibliotecas/rlutil.h"
#define PX 120
#define PY 30
char parede = '*', outra = 'x', person = '0', ini = '&', fase[10] = "mapa_novo";
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
  system("clear");

  for(int i = 0; i < PY; i ++){
    for(int j = 0; j < PX; j++){
      if(mapa[i][j] == 1) printf("%c", parede);
      else if (mapa[i][j] == 2) printf("%c", outra);
      else if ((i == personagem.y)&&(j == personagem.x)) printf("%c", person);
      else if ((i == inimigo.y)&&(j == inimigo.x)) printf("%c", ini);
      else if (mapa[i][j] == 0) printf(" ");

    }
    printf("\n");
  }
  printf("\n %d", mapa[personagem.y + 1][personagem.x]);
  printf(" %d ", mapa[personagem.y][personagem.x - 1]);
  printf(" x: %d ", personagem.x);
  printf("y: %d \n", personagem.y);
}

void movimento(char mv){

  if (mv == 'a')
    if (mapa[personagem.y][personagem.x - 1] != 1) --personagem.x;
  if (mv == 'd')
    if (mapa[personagem.y][personagem.x + 1] != 1) ++personagem.x;
  if (mv == 'l') personagem.y = 2;
  if (mv == 'w')
    if (G == 0)
      G = 4;
  if (mv == 's')
    if (personagem.y < 28)
      personagem.y++;


}

void ler_mapa(){
    char c;
    int i=0,j=0,num;
    FILE *arq;
    if (!(arq = fopen(fase,"r"))) /* Caso ocorra algum erro na abertura do arquivo..*/
        {                           /* o programa aborta automaticamente */
                printf("Erro! Impossivel abrir o arquivo!\n");
                exit(1);
        }

    for(i = 0; i < PY; i ++){
        for(j = 0; j < PX; j++){
            c= fgetc(arq);
            num = atoi(&c);
            mapa[i][j]= num;
        }
            c = fgetc(arq);
            if(c != '\n'){
                num = atoi(&c);
                mapa[i][j]= num;
            }
    }
    fclose(arq);
}

void *gravidade(){

  while(1){

    if((personagem.x == inimigo.x)&&(personagem.y == inimigo.y)){  
      personagem.x = 2;
      personagem.y = 2;
    }

    if(G == 0){
      if(mapa[personagem.y + 1][personagem.x] != 1){
        if(mapa[inimigo.y + 1][inimigo.x] != 1) ++inimigo.y;
        ++personagem.y;
        usleep(80000);
        
      }
    }else {
      if(1){
        personagem.y--;
        usleep(90000);
        G--;
      }
    }
  }
}

void *imprimir(){
  ler_mapa();
  //CriarMapa();
  
  while(1){
    usleep(50000);
    imprimirMapa();
  }
}


int main(){
  
  personagem.x = 2;
  personagem.y = 2;
  inimigo.x = 14;
  inimigo.y = 2;
  pthread_t gravity, frame;
  char mov;
 

  pthread_create(&gravity, NULL, gravidade, NULL);
  pthread_create(&frame, NULL, imprimir, NULL);


  while(mov != 'p'){

    mov = getch();
    movimento(mov);

  }
  return 0;
}
