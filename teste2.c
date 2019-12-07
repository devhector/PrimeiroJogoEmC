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
#define PX 120
#define PY 30
char parede = '*', outra = 'x', person = 'O', ini = 'o', fase[30] = "mapa_novo";
int mapa[PY][PX], G, min, seg;

typedef struct{
  int x;
  int y;
}pos;

pos personagem, inimigo[2];


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
      else if ((i == inimigo[0].y)&&(j == inimigo[0].x)) printf("%c", ini);
      else if ((i == inimigo[1].y)&&(j == inimigo[1].x)) printf("%c", ini);
      else if (mapa[i][j] == 0) printf(" ");

    }
    printf("\n");
  }
  printf("\n %d", mapa[personagem.y + 1][personagem.x]);
  printf(" %d ", mapa[personagem.y][personagem.x - 1]);
  printf(" x: %d ", personagem.x);
  printf("y: %d ", personagem.y);
  printf("%d:%d", min, seg);
  printf("\n");
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
    int i ,j ,num;
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

void *gravidadeper(){

  while(1){

    if((personagem.x == inimigo[0].x)&&(personagem.y == inimigo[0].y)){
      personagem.x = 2;
      personagem.y = 2;
    }

    if(G == 0){
      if(mapa[personagem.y + 1][personagem.x] != 1){
        ++personagem.y;
        usleep(85000);
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

void *gravidadeini(){

  while (1)
  {
    if(G == 0){

      if(mapa[inimigo[0].y + 1][inimigo[0].x] != 1){
         ++inimigo[0].y;
        //usleep(70000);
      }

      if(mapa[inimigo[1].y + 1][inimigo[1].x] != 1){
         ++inimigo[1].y;

      }

      usleep(70000);

    }
  }

}

void *mvinimigo(){

while(1){
  if (mapa[inimigo[0].y + 1][inimigo[0].x] == 1){
    if(mapa[inimigo[0].y][inimigo[0].x + 1] != 1){
      while (1)
      {
        inimigo[0].x++;
        usleep(100000);
        if(mapa[inimigo[0].y][inimigo[0].x + 1] == 1) break;
      }
    }
    if(mapa[inimigo[0].y][inimigo[0].x - 1] != 1){
      while (1)
      {
        inimigo[0].x--;
        usleep(100000);
        if(mapa[inimigo[0].y][inimigo[0].x - 1] == 1) break;
      }
    }


  }
}

}

void *imprimir(){
  //CriarMapa();

  while(1){
    usleep(50000);
    imprimirMapa();
  }
}

void *cronometro(){

min = 0;
seg = 0;

while (1)
{
  for (int i = 0; i < 60; i ++){

    seg = i;

    if(seg == 59){
      min++;
      seg = 0;
    }
  sleep(1);
  }
}

}

void iniciar(){
  ler_mapa();
  personagem.x = 2;
  personagem.y = 2;
  inimigo[0].x = 62;
  inimigo[0].y = 2;
  inimigo[1].x = 31;
  inimigo[1].y = 2;
  pthread_t gravity[2], frame, movime, cro;


  pthread_create(&gravity[0], NULL, gravidadeper, NULL);
  pthread_create(&gravity[1], NULL, gravidadeini, NULL);
  pthread_create(&frame, NULL, imprimir, NULL);
  pthread_create(&movime, NULL, mvinimigo, NULL);
  pthread_create(&cro, NULL, cronometro, NULL);

  char mov;

  while(mov != 'p'){

    mov = getch();
    movimento(mov);

  }

}

int menu(){
  char c;
  int cursorx = 40, cursory = 10, sel=0;
  gotoxy(28,8);
  printf("Bem-vindo a Rush Knight!");
  gotoxy(28,10);
  printf("Iniciar");
  gotoxy(28,12);
  printf("Ranking");
  gotoxy(28,14);
  printf("Sair");
  gotoxy(cursorx,cursory);
  printf("<");
  while( c != '\n'){
    c = getch();
    if(c == 's'){
      gotoxy(cursorx,cursory);
      printf(" ");
      if(sel < 2){
        sel++;
        cursory = cursory + 2;
        gotoxy(cursorx,cursory);
        printf("<");
      }
      else{
        sel = 0;
        cursory = 10;
        gotoxy(cursorx,cursory);
        printf("<");
      }
    }
    if(c == 'w'){
      gotoxy(cursorx,cursory);
      printf(" ");
      if(sel > 0){
        sel--;
        cursory = cursory - 2;
        gotoxy(cursorx,cursory);
        printf("<");
      }
      else{
        sel = 2;
        cursory = 14;
        gotoxy(cursorx,cursory);
        printf("<");
      }
    }
  }
  return sel;
}

int main(){

  hidecursor();
  switch(menu()){
    case(0):
      iniciar();
      break;

    case(2):
      return 0;
  }
}
