#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

// Questo file continene solo la definizione della funzione main
// con il menu principale 1-2-3

int main( ) {

  int scelta=0;

  time_t t;
  srand((unsigned) time(&t));

  system("clear");
  printf("\n+----------------------------------------------------------------------------------+\n");
  printf("|##################################################################################|\n");
  printf("|##################################################################################|\n");
  printf("|                                                                                  |\n");
  printf("|                                                                                  |\n");
  printf("|                                                                                  |\n");
  printf("|                                                                                  |\n");
  printf("|           ###########   ##########      #####     #########  ###   ########      |\n");
  printf("|          ###########   ###     ##     ###  ##    ###        ###   ########       |\n");
  printf("|             ###       ###     ##    ###    ##   ###        ###   ###             |\n");
  printf("|            ###       ##########   ##########   ###  ####  ###   ###              |\n");
  printf("|           ###       ###   ##     ##########   ###    ##  ###   ########          |\n");
  printf("|          ###       ###     ##   ###     ##   #########  ###   ########           |\n");
  printf("|                                                                                  |\n");
  printf("|                                                                                  |\n");
  printf("|                                                                                  |\n");
  printf("|                                                                                  |\n");
  printf("|##################################################################################|\n");
  printf("|##################################################################################|\n");
  printf("+----------------------------------------------------------------------------------+\n");




  fseek(stdin,0,SEEK_END); //pulisco il buffer
  while (getchar() != '\n'); //attendo che l'utente prema invio

  printf("Due maghi si affrontano in un duello uno-contro-uno su una piana ventosa vicina a \nRavnika, la Città delle Gilde. Gli incantesimi che possono lanciare sono rappresentati \nda un mazzo di carte ciascuno.\n");

  fseek(stdin,0,SEEK_END); //pulisco il buffer
  while (getchar() != '\n'); //attendo che l'utente prema invio

  system("clear");



  do  {
  system("clear");

  printf("\n+----------------------------------------------------------------------------------+\n");
  printf("|                                 MENU' PRINCIPALE                                 |\n");
  printf("+----------------------------------------------------------------------------------+\n");

    printf("\n1) imposta gioco\n2) combatti\n3) termina gioco\n\n");
      scelta=0;// perchè nel caso scelta sia rimasta 1,2 o 3 dai casi precedenti e si inserisce un valore non valido, come una lettera,scelta non si aggiorna e ritorna dentro al menù precedente. la si inizializza a 0 per evitare questo.
      scanf ("%d", &scelta);
      //while((getchar())!='\n')
switch (scelta) {

  case 1:
    imposta_gioco();
    break ;
  case 2:

    combatti();
    break ;
  case 3:

    termina_gioco();
    break ;
  default:

    printf("scelta non valida, inserire un numero da 1 a 3\n");
      fseek(stdin,0,SEEK_END); //pulisco il buffer
      while (getchar() != '\n'); //attendo che l'utente prema invio
    break;

}
} while (scelta !=3);
}
