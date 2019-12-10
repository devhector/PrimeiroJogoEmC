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
char parede = '*', outra = 'x', person = 'O', ini = 'o',nome_player[50], fim = 'H';
<<<<<<< HEAD
char *fases[5] = {"maps/MAPA1", "maps/MAPA2","maps/MAPA3","maps/MAPA4", "maps/MAPA5"};
int mapa[PY][PX], G, min = 0, seg = 0,pulando = 0, fase_atual=0,fases_max = 5,venceu = 0, morte = 0;

//variavel id para receber a identificação das threads e poder encerrar quando a fase acabar
=======
char *fases[5] = {"MAPA1", "MAPA2","MAPA3","MAPA4", "MAPA5"};
int mapa[PY][PX], G, min = 0, seg = 0,pulando = 0, fase_atual=0,fases_max = 4,venceu = 0, morte = 0;
>>>>>>> 69f59243202cf4f30d0d48dc90c13c3231e74e3e
pthread_t id_threads[10];

//struct feita para usar como posição e ficar mais organizado.
typedef struct{ 
  int x;
  int y;
  int inicial_x;
  int inicial_y;
}pos;

pos personagem, inimigo[2],saida;

void CriarMapa(){ //função autoexplicativa, mas usada para criar a base dos MAPAS
  for(int i = 0; i < PY; i ++)
    for(int j = 0; j < PX; j++){
      if(i == 0 || i == PY - 1) mapa[i][j] = 1;
      else if(j == 0 || j == PX - 1) mapa[i][j] = 1;
      else if (i == 20 && (j > 8 && j < 20)) mapa[i][j] = 1;
      else mapa[i][j] = 0;

    }
}

void imprimirMapa(){ //função autoexplicativa
  cls();

  for(int i = 0; i < PY; i ++){
    for(int j = 0; j < PX; j++){
      if(mapa[i][j] == 1) printf("%c", parede);
      else if (mapa[i][j] == 2) printf("%c", outra);
      else if (mapa[i][j] == 4) printf("%c", fim);
      else if ((i == personagem.y)&&(j == personagem.x)) printf("%c", person);
      else if ((i == inimigo[0].y)&&(j == inimigo[0].x)) printf("%c", ini);
      else if ((i == inimigo[1].y)&&(j == inimigo[1].x)) printf("%c", ini);
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

void movimento(char mv){ //função para o movimento do personagem

  if (mv == 'a')
    if (mapa[personagem.y][personagem.x - 1] != 1) --personagem.x;
  if (mv == 'd')
    if (mapa[personagem.y][personagem.x + 1] != 1) ++personagem.x;
  if (mv == 'l') personagem.y = 2;
  if (mv == 'w' && pulando == 0)
    if (G == 0)
      G = 5; // "inversão" da "gravidade"
  if (mv == 's')
    if (personagem.y < 28)
      personagem.y++;


}

void ler_mapa(char fase[]){ //leitura do mapa, através dos binários
    char c;
    int i ,j ,num;
    FILE *arq;
    if (!(arq = fopen(fase,"r"))) /* Caso ocorra algum erro na abertura do arquivo..*/
        {                           /* o programa aborta automaticamente */
                printf("Erro! Impossivel abrir o arquivo: %s\n", fase);
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
            }else if(num == 6){
              inimigo[0].inicial_x = j;
              inimigo[0].inicial_y = i;
            }else if(num == 7){
              inimigo[1].inicial_x = j;
              inimigo[1].inicial_y = i;
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

// A primeira thread iniciada, usada como a gravidade do personagem, manipulando a posição global do personagem
void *gravidadeper(){ 

  id_threads[0] = pthread_self(); //a função chamada retorna uma identificação e essa identificação é passada para
                                  // outra função abaixo, que por sua vez encerra essa thread, no final de cada fase
  while(!morte){

    if((personagem.x == inimigo[0].x)&&(personagem.y == inimigo[0].y)){ //usado como colisão do personagem com o inimigo
      morte = 1;
    }
    if((personagem.x == inimigo[1].x)&&(personagem.y == inimigo[1].y)){
      morte = 1;
    }
    if((personagem.x == saida.x)&&(personagem.y == saida.y)){ //colisão mas com o final da fase, mostrando o fim da fase
      venceu = 1;
      morte = 1;
    }


    if(G == 0){
      if(mapa[personagem.y + 1][personagem.x] == 1 ){ //flag para n haver mais de 1 pulo
        pulando = 0;
      }
      if(mapa[personagem.y + 1][personagem.x] != 1 ){ //"gravidade", implementa o y do personagem dando efeito de cair
        ++personagem.y;
        usleep(85000); //usado para dar delay
      }
    }else {
      
        pulando = 1;
        personagem.y--;
        usleep(101000);
        G--; //usado como "pulo" no movimento()
      
    }
  }
  pthread_exit(NULL);
}

void *gravidadeini(){ //gravidade do inimigo

  id_threads[1] = pthread_self();

  while (!morte)
  {
    if(G == 0){

      if(mapa[inimigo[0].y + 1][inimigo[0].x] != 1){
         ++inimigo[0].y;
      }

      if(mapa[inimigo[1].y + 1][inimigo[1].x] != 1){
         ++inimigo[1].y;

      }

      usleep(70000);

    }
  }
  pthread_exit(NULL);
}

void *mvinimigo0(){ //função usada para dar movimento ao inimigo

id_threads[3] = pthread_self();

while(!morte){
  if (mapa[inimigo[0].y + 1][inimigo[0].x] == 1){
    if(mapa[inimigo[0].y][inimigo[0].x + 1] != 5){
      while (1)
      {
        inimigo[0].x++;
        usleep(100000);
        if(mapa[inimigo[0].y][inimigo[0].x + 1] == 5 || mapa[inimigo[0].y][inimigo[0].x + 1] == 1) break; //condição de parada e mudança de direção
      }
    }
    if(mapa[inimigo[0].y][inimigo[0].x - 1] != 5){
      while (1)
      {
        inimigo[0].x--;
        usleep(100000);
        if(mapa[inimigo[0].y][inimigo[0].x - 1] == 5 || mapa[inimigo[0].y][inimigo[0].x - 1] == 1) break;
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
      if(mapa[inimigo[1].y][inimigo[1].x + 1] != 5){
        while (1)
        {
          inimigo[1].x++;
          usleep(100000);
          if(mapa[inimigo[1].y][inimigo[1].x + 1] == 5 || mapa[inimigo[1].y][inimigo[1].x + 1] == 1 ) break;
        }
      }
      if(mapa[inimigo[1].y][inimigo[1].x - 1] != 5){
        while (1)
        {
          inimigo[1].x--;
          usleep(100000);
          if(mapa[inimigo[1].y][inimigo[1].x - 1] == 5 || mapa[inimigo[1].y][inimigo[1].x - 1] == 1) break;
        }
      }


    }
  }
  pthread_exit(NULL);
}

//usada para imprimir constantemente
void *imprimir(){
  id_threads[2] = pthread_self();

  while(morte != 1){
    usleep(55000);
    imprimirMapa();
  }
  pthread_exit(NULL);
}

//autoexplicativa
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
  sleep(1);//espera 1s até continuar
  }
}

min = 0;
seg = 0;

pthread_exit(NULL);

}

//autoexplicativa
void tela_fim_fase(){
  venceu = 1;
  for(int i = 0; i < 5; i++)
    pthread_cancel(id_threads[i]);
  if(fase_atual == fases_max){
    pthread_cancel(id_threads[5]);
  }
  cls();
  if(fase_atual<fases_max){
    gotoxy(28,8);
    printf("Voce conseguiu %s!", nome_player);
    gotoxy(28,10);
    printf("Fases percorridas até agora: %i", fase_atual+1);
    gotoxy(28,12);
    printf("Tempo Atual: %i : %i", min,seg);
    gotoxy(28,14);
    printf("Pressione qualquer tecla para continuar!");
    fase_atual++;
    getch();
  }else{
    gotoxy(28,8);
    printf("Você Conseguiu %s!", nome_player);
    gotoxy(28,10);
    printf("Você Percorreu Todas As Fases!");
    gotoxy(28,12);
    printf("Tempo Conquistado No Total: %i : %i", min,seg);
    gotoxy(28,14);
    printf("Pressione qualquer tecla para continuar!");
    getch();
  }

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


void iniciar(){
  venceu = 0;
  morte = 0;
  ler_mapa(fases[fase_atual]);
  cls();
  if(fase_atual==0){
    gotoxy(28,8);
    printf("Insira o seu nome: ");
    gotoxy(30,10);
    printf(">");
    scanf("%s", nome_player);
  }

  personagem.x=personagem.inicial_x;
  personagem.y=personagem.inicial_y;
  inimigo[0].x = inimigo[0].inicial_x;
  inimigo[0].y = inimigo[0].inicial_y;
  inimigo[1].x = inimigo[1].inicial_x;
  inimigo[1].y = inimigo[1].inicial_y;
  pthread_t threads[10];


  pthread_create(&threads[0], NULL, gravidadeper, NULL);
  pthread_create(&threads[1], NULL, gravidadeini, NULL);
  pthread_create(&threads[2], NULL, imprimir, NULL);
  pthread_create(&threads[3], NULL, mvinimigo0, NULL);
  pthread_create(&threads[4], NULL, mvinimigo1, NULL);
  if(fase_atual==0)
    pthread_create(&threads[5], NULL, cronometro, NULL);

  char mov;

  while(morte != 1){

    mov = getch();
    movimento(mov);

  }
  if(venceu ==1 && fase_atual < fases_max){
    tela_fim_fase();
    iniciar();
  }
  if(venceu == 1){
    tela_fim_fase();
    return;
  }
  if(venceu!=1){
    tela_morte();
    return;
  }
}



int menu(){
  char c;
  int cursorx = 40, cursory = 10, sel=0;
  system("clear");
  gotoxy(28,8);
  printf("Bem-vindo ao Princeso pulante do Sul!");
  gotoxy(28,10);
  printf("Iniciar");
  gotoxy(28,12);
  printf("Ranking");
  gotoxy(28,14);
  printf("Mapa Custom");
  gotoxy(28,16);
  printf("Sair Do Jogo");
  gotoxy(cursorx,cursory);
  printf("<");
  c = '0';
  while( c != '\n'){
    c = getch();
    if(c == 's'){
      gotoxy(cursorx,cursory);
      printf(" ");
      if(sel < 3){
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
        sel = 3;
        cursory = 16;
        gotoxy(cursorx,cursory);
        printf("<");
      }
    }
  }
  return sel;
}

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
