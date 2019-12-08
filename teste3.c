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
#define MAPAS 0
char parede = '*', outra = 'x', person = 'O', ini = 'o',nome_player[50], fim = '$';
int mapa[PY][PX], G, min = 0, seg = 0,pulando = 0, fase_atual=0,venceu = 0, morte = 0;
pthread_t id_threads[10];


typedef struct{
  int x;
  int y;
  int inicial_x;
  int inicial_y;
}pos;

pos personagem, inimigo[2],saida;


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
  cls();

  for(int i = 0; i < PY; i ++){
    for(int j = 0; j < PX; j++){
      if(mapa[i][j] == 1) printf("%c", parede);
      else if (mapa[i][j] == 2) printf("%c", outra);
      else if (mapa[i][j] == 4) printf("%c", fim);
      else if ((i == personagem.y)&&(j == personagem.x)) printf("%c", person);
      else if ((i == inimigo[0].y)&&(j == inimigo[0].x)) printf("%c", ini);
      else if ((i == inimigo[1].y)&&(j == inimigo[1].x)) printf("%c", ini);
      //else if (mapa[i][j] == 0) printf(" ");
      else printf(" ");

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
  if (mv == 'w' && pulando == 0)
    if (G == 0)
      G = 5;
  if (mv == 's')
    if (personagem.y < 28)
      personagem.y++;


}

void ler_mapa(char fase[]){
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
            if(num == 3){
              personagem.inicial_x = j;
              personagem.inicial_y = i;
            }else if(num == 4){
              saida.x = j;
              saida.y = i;
            }
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

  id_threads[0] = pthread_self();

  while(!morte){

    if((personagem.x == inimigo[0].x)&&(personagem.y == inimigo[0].y)){
      morte = 1;
    }
    if((personagem.x == inimigo[1].x)&&(personagem.y == inimigo[1].y)){
      morte = 1;
    }
    if((personagem.x == saida.x)&&(personagem.y == saida.y)){
      venceu = 1;
      morte = 1;
    }


    if(G == 0){
      if(mapa[personagem.y + 1][personagem.x] == 1 ){
        pulando = 0;
      }
      if(mapa[personagem.y + 1][personagem.x] != 1 ){
        ++personagem.y;
        usleep(85000);
      }
    }else {
      if(1){
        pulando = 1;
        personagem.y--;
        usleep(101000);
        G--;
      }
    }
  }
  pthread_exit(NULL);
}

void *gravidadeini(){

  id_threads[1] = pthread_self();

  while (!morte)
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
  pthread_exit(NULL);
}

void *mvinimigo0(){

id_threads[3] = pthread_self();

while(!morte){
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
  pthread_exit(NULL);
}

void *mvinimigo1(){

  id_threads[4] = pthread_self();

  while(!morte){
    if (mapa[inimigo[1].y + 1][inimigo[1].x] == 1){
      if(mapa[inimigo[1].y][inimigo[1].x + 1] != 1){
        while (1)
        {
          inimigo[1].x++;
          usleep(100000);
          if(mapa[inimigo[1].y][inimigo[1].x + 1] == 1) break;
        }
      }
      if(mapa[inimigo[1].y][inimigo[1].x - 1] != 1){
        while (1)
        {
          inimigo[1].x--;
          usleep(100000);
          if(mapa[inimigo[1].y][inimigo[1].x - 1] == 1) break;
        }
      }


    }
  }
  pthread_exit(NULL);
}

void *imprimir(){
  id_threads[2] = pthread_self();

  while(morte != 1){
    usleep(50000);
    imprimirMapa();
  }
  pthread_exit(NULL);
}

void *cronometro(){

id_threads[5] = pthread_self();

min = 0;
seg = 0;

while (morte != 1)
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

min = 0;
seg = 0;

pthread_exit(NULL);

}

void iniciar(){
  venceu = 0;
  morte = 0;
  cls();
  gotoxy(28,8);
  printf("Insira o seu nome: ");
  gotoxy(30,10);
  printf(">");
  scanf("%s", nome_player);

  personagem.x=personagem.inicial_x;
  personagem.y=personagem.inicial_y;
  inimigo[0].x = 62;
  inimigo[0].y = 2;
  inimigo[1].x = 31;
  inimigo[1].y = 2;
  pthread_t threads[10];


  pthread_create(&threads[0], NULL, gravidadeper, NULL);
  pthread_create(&threads[1], NULL, gravidadeini, NULL);
  pthread_create(&threads[2], NULL, imprimir, NULL);
  pthread_create(&threads[3], NULL, mvinimigo0, NULL);
  pthread_create(&threads[4], NULL, mvinimigo1, NULL);
  pthread_create(&threads[5], NULL, cronometro, NULL);

  char mov;

  while(morte != 1){

    mov = getch();
    movimento(mov);

  }
  if(venceu ==1){
    tela_fim_fase();
  }else{
    tela_morte();
    }
  return;

}

void tela_fim_fase(){
  venceu = 1;
  for(int i = 0; i < 6; i++)
    pthread_cancel(id_threads[i]);
  cls();
  gotoxy(28,8);
  printf("Voce conseguiu %s!", nome_player);
  gotoxy(28,10);
  printf("Fases percorridas até agora: %i", fase_atual+1);
  gotoxy(28,12);
  printf("Tempo Conquistado: %i : %i", min,seg);
  gotoxy(28,14);
  printf("Pressione qualquer tecla para continuar!");
  getch();

}

void tela_morte(){
  morte = 1;
  for(int i = 0; i < 6; i++)
    pthread_cancel(id_threads[i]);
  cls();
  gotoxy(28,8);
  printf("Voce perdeu %s!", nome_player);
  gotoxy(28,10);
  printf("Fases percorridas: %i", fase_atual+1);
  gotoxy(28,12);
  printf("Pressione qualquer tecla para continuar!");
  getch();
}


int menu(){
  char c;
  int cursorx = 40, cursory = 10, sel=0;
  system("clear");
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
  c = "0";
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
  char fase[15] = "mapa_novo";
  ler_mapa(fase);
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
        return 0;
    }
    fflush(stdin);

  }
}
