#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"


static int stato_imposta_gioco = 0; //variabile usata per verificare se il gioco è stato impostato oppure no (1 se si 0 se no)
static int turno=0;  //variabile turno usanta nella funzione combatti (per decidere chi gioca, il mago 1 se assume valore 1 e mago 2 se assume valore 2) e nella funzione turno_mago per riassegnare il valore della variabile
static int termina;  //variabile assegnata a 1 per terminare il turno
static int n_carte_mazzo; //numero delle carte del mazzo
static struct Mago mago1;
static struct Mago mago2;


static void inizializza_mago (struct Mago * pmago);
static void crea_carta (struct Mago* mago, int numero_carta);
static void inizializza_carta(struct Mago* mago, struct Carta *Inew, int numero_carta);
//static void stampa_mazzo(struct Mago * mago); //stampa mazzo, per prova
static void crea_mano(struct Mago* mago);
static void stampa_mano(struct Mago* mago);
static void inserisci_mano(struct Mago* mago, struct Carta* carta_da_inserire);
static void pesca_carta(struct Mago* mago);
static void stampa_campo(struct Mago* mago);
static int  verifica_campo(struct Mago* mago);
static int  verifica_mano(struct Mago* mago);
static int  seleziona_creatura(struct Mago* mago);
static int  gioca_carta(struct Mago* mago, struct Mago* mago_nemico );
static void turno_mago (struct Mago* mago, struct Mago* mago_nemico);
static int  attacca(struct Mago* pmago, struct Mago* pmago_nemico);
static void layout_combatti(struct Mago* mago);
static void layout_imposta_gioco();
static void attendi();
static void layout_turno_terminato();
static void elimina_mazzo(struct Mago* pmago);
static void elimina_campo(struct Mago* pmago);
static void elimina_mano(struct Mago* pmago);
static void resetta_memoria(struct Mago* pmago);
static void vittoria();

//le tre funzioni principali

void imposta_gioco(){
system("clear");
if (stato_imposta_gioco==1){
  resetta_memoria (&mago1);
  resetta_memoria (&mago2);
}
stato_imposta_gioco=1;

  mago1.prima_carta = NULL;
  mago2.prima_carta = NULL;

  layout_imposta_gioco();


printf("\n                           inserire i dati per il primo mago:\n" );
  inizializza_mago(&mago1);


  layout_imposta_gioco();

printf("\n                          inserire i dati per il secondo mago:\n" );
  inizializza_mago(&mago2);


//creiamo il mazzo



  do {
    layout_imposta_gioco();
    printf("\nInserire il numero di carte nel mazzo: \n\n");
    scanf("%d", &n_carte_mazzo);
    if ((n_carte_mazzo <10) || (n_carte_mazzo >40)){
      layout_imposta_gioco();
      printf("\nInserire un numero compreso tra 10 e 40 \n");
      attendi();
    }
  } while (n_carte_mazzo <10 || n_carte_mazzo >40);

  int i=0;

  for(i=0; i<n_carte_mazzo;i++){
    crea_carta (&mago1, i);
    crea_carta (&mago2, i);

}



//ora creiamo le due mani

crea_mano(&mago1);
crea_mano(&mago2);

//stampiano a schermo tutti le informaizoni
layout_imposta_gioco();
printf("\n                             il primo mago si chiama %s\n                            appartiene alla classe ", mago1.nome_mago);
if (mago1.cmago==0)
  printf("Tenbre");
if (mago1.cmago==1)
  printf("Vita");
if (mago1.cmago==2)
  printf("Luce");
printf("\n\n                            possiede un mazzo di %d carte", n_carte_mazzo);
printf("\n\n                          ed ha una mano contenente 6 carte:\n\n");
stampa_mano(&mago1);

attendi();
layout_imposta_gioco();
printf("\n                            il secondo mago si chiama %s\n                             appartiene alla classe ", mago2.nome_mago);
if (mago2.cmago==0)
  printf("Tenbre");
if (mago2.cmago==1)
  printf("Vita");
if (mago2.cmago==2)
  printf("Luce");
printf("\n\n                            possiede un mazzo di %d carte", n_carte_mazzo);
printf("\n\n                          ed ha una mano contenente 6 carte:\n\n");
stampa_mano(&mago2);
attendi();

  }

void combatti(){
termina=0;
    if (stato_imposta_gioco == 0){ //Prima di procedere bisogna aver impostato il gioco almeno una volta
        printf("\nPrima di poter giocare è necessario impostare il gioco\n");
        attendi();
        return;}

    turno = ((rand()%2)+1); //per generare un numero casuale 0 o 1
    fseek(stdin,0,SEEK_END); //pulisco il buffer

do {
    switch (turno) {
      case 1:
        turno_mago(&mago1, &mago2);
break;
      case 2:
        turno_mago(&mago2, &mago1);
break;

      }
  }while(termina==0);
}

void termina_gioco(){
printf("\nArrivederci\n");
resetta_memoria(&mago1);
resetta_memoria(&mago2);

//exit(0);
}

//dichiarazioni dlle funzioni

//funzioni che riguardano il mago
  static void inizializza_mago (struct Mago *pmago) {//inizializza il nome, la classe e i puntivita del mago

int scelta=1;
fseek(stdin,0,SEEK_END); //pulisco il buffer

    pmago->punti_vita=20;
    printf("\n                                          nome:\n");
    fgets(pmago->nome_mago, 256, stdin);


do {
    if (scelta <1 || scelta >3){
      layout_imposta_gioco();
      printf("\nvalore non corretto, inserisci un numero compreso tra 1 e 3\n");
      attendi();}

    layout_imposta_gioco();
    printf("\n                                         classe:\n 1) Tenebre \n 2) Vita \n 3) Luce \n\n");
    scelta=0;
    scanf("%d", &scelta);
    --scelta; //per far coincidere il valore scelto con gli indici degli array
    pmago->cmago= scelta;
    ++scelta; // per ripristinare scelta


   }
      while (scelta <1 || scelta >3);
}

//funzioni che riguardano la creazione del mazzo
  static void crea_carta (struct Mago *pmago, int numero_carta){//crea il singolo nodo della lista dinamica
    struct Carta * New = (struct Carta*) malloc (sizeof(struct Carta));
    inizializza_carta (pmago, New, numero_carta);
    New->carta_successiva=NULL;

    if (pmago->prima_carta == NULL){ //Se non ci sono carte la nuova carta diventa la prima e ultima del mazzo.
        pmago->prima_carta = New;

    }
    else {
    struct Carta * ptemp = pmago->prima_carta;

    while (ptemp -> carta_successiva != NULL)
      ptemp = ptemp -> carta_successiva;

    ptemp -> carta_successiva = New;
  //    ptemp->carta_successiva = New;

    return;
  }
}
  static void inizializza_carta (struct Mago *pmago, struct Carta *Inew, int numero_carta){//inizializza i punti vita e il tipo della carta appena creata, modifica anche i pv a seconda della classe dle mago

  //int numero_carta;

  int ncasuale = ((rand()%100)+1); //Inizializzo il tipo di carta tra le 4 possibili con probabilità casuale 40-25-20-15
  if (ncasuale <= 40)
      (*Inew).tcarta = creatura;
  else if (ncasuale <=65)
      (*Inew).tcarta = rimuovi_creatura;
  else if (ncasuale <=85)
      (*Inew).tcarta = infliggi_danno;
  else
      (*Inew).tcarta = guarisci_danno;

int r;

r=n_carte_mazzo/5; //r sarà il risultato senza parte decimale (perchè di tipo int), quindi successivamente metteremo un else per tutti i casi compresi tra r*4 e n_carte_mazzo

if (numero_carta<r)
  (*Inew).punti_vita = ((rand()%5)+4);  //oscilla da 4 a 8
else if (numero_carta<(r*2))
  (*Inew).punti_vita = ((rand()%4)+3);  //oscilla da 3 a 6
else if (numero_carta<(r*3))
  (*Inew).punti_vita = ((rand()%3)+2);  //oscilla da 2 a 4
else if (numero_carta<(r*4))
  (*Inew).punti_vita = ((rand()%3)+1);  //oscilla da 1 a 3
else
  (*Inew).punti_vita = ((rand()%2)+1);  //oscilla da 1 a 2



//se il mago ha classe tenebra (0) le sue carte infliggi_danno hanno il doppio dei punti_vita,
//se classe vita(1), le sue carte creatura hanno la metà dei punti_vita in più,
//se luce(2) le sue carte guarisci_danno hanno tre volte e mezzo il valore dei punti_vita.

if (pmago->cmago==0 && Inew->tcarta==2){//se classe mago tenebra and tipo carta infliggi_danno
  Inew->punti_vita=Inew->punti_vita*2;}
if (pmago->cmago==1 && Inew->tcarta==0){//se classe mago vita and tipo carta creatura
  Inew->punti_vita=Inew->punti_vita+(Inew->punti_vita/2);}
if (pmago->cmago==2 && Inew->tcarta==3){//se classe mago luce and tipo carta guarisci_danno
  Inew->punti_vita=Inew->punti_vita*3.5;}

}
  static void pesca_carta(struct Mago* pmago) {//prende l'ultima carta del mazzo e la sposta nella mano (se c'è posto, altrimenti viene deallocata)
  struct Carta* ptemp = pmago -> prima_carta;
  if (ptemp == NULL){//nel caso in cui non si hanno più carte nel mazzo il gioco finisce e si richiama la funzione di uscita vittoria
    vittoria();
  }
    else
      if (ptemp -> carta_successiva == NULL) {
        printf("hai pescato l'ultima carta del mazzo:\n");



        if (verifica_mano(pmago)==1){                         //se l'array mano è pieno stampa il messaggio di errore  e poi chiude
        printf("attento! non hai più posto nella mano, la carta viene distrutta");
        return;}

        inserisci_mano(pmago, ptemp -> carta_successiva);   //altrimenti inserisce la carta e stampa a schermo la carta pescata




        printf("\n  +----------+\n" );
        printf("  |          |\n" );
        if (ptemp ->tcarta==0){
          printf("  | creatura |\n" );
          printf("  |          |\n" );}
        else
        if (ptemp ->tcarta==1){
          printf("  | rimuovi  |\n" );
          printf("  | creatura |\n" );}
        else
        if (ptemp ->tcarta==2){
          printf("  | infliggi |\n" );
          printf("  |  danno   |\n" );}
        else
        if (ptemp ->tcarta==3){
          printf("  | rimuovi  |\n" );
          printf("  |  danno   |\n" );}

        printf("  |          |\n" );

        if (ptemp ->punti_vita<=9){
          printf ("  |  %.1f pv  |\n",ptemp ->punti_vita);}
          else{
            printf ("  |  %.1f pv |\n",ptemp ->punti_vita);
            }

        printf("  |          |\n" );
        printf("  +----------+\n" );

        pmago -> prima_carta = NULL;
      }
      else {
        while ((ptemp -> carta_successiva) -> carta_successiva != NULL)
          ptemp = ptemp -> carta_successiva;


          if (verifica_mano(pmago)==1){                         //se l'array mano è pieno stampa il messaggio di errore (da inserisci mano) e poi chiude
          printf("attento! non hai più posto nella mano, la carta viene distrutta");
          return;}

          inserisci_mano(pmago, ptemp -> carta_successiva);   //altrimenti inserisce la carta e stampa a schermo la carta pescata




        printf("hai pescato:\n");
        printf("\n  +----------+\n" );
        printf("  |          |\n" );
        if (ptemp -> carta_successiva->tcarta==0){
          printf("  | creatura |\n" );
          printf("  |          |\n" );}
        else
        if (ptemp -> carta_successiva->tcarta==1){
          printf("  | rimuovi  |\n" );
          printf("  | creatura |\n" );}
        else
        if (ptemp -> carta_successiva->tcarta==2){
          printf("  | infliggi |\n" );
          printf("  |  danno   |\n" );}
        else
        if (ptemp -> carta_successiva->tcarta==3){
          printf("  | rimuovi  |\n" );
          printf("  |  danno   |\n" );}

        printf("  |          |\n" );

        if (ptemp -> carta_successiva->punti_vita<=9){
          printf ("  |  %.1f pv  |\n",ptemp -> carta_successiva->punti_vita);}
          else{
            printf ("  |  %.1f pv |\n",ptemp -> carta_successiva->punti_vita);
            }

        printf("  |          |\n" );
        printf("  +----------+\n" );

        ptemp -> carta_successiva = NULL;

      }
}
  static void crea_mano(struct Mago* pmago) {//serve per riempire la mano ad inizio gioco
  int i=0;
for (i = 0; i < 6; i++){(pmago->mano[i] = NULL);}
for (i = 0; i < 6; i++){
  struct Carta * New = (struct Carta*) malloc (sizeof(struct Carta));
  inizializza_carta (pmago, New, n_carte_mazzo);
  inserisci_mano(pmago, New);
}

}
  static void inserisci_mano(struct Mago* pmago, struct Carta* carta_da_inserire) { //serve per inserire le carte nella mano

  if (verifica_mano(pmago)==1){
    printf("attento! non hai più posto nella mano, la carta viene distrutta");
    free(carta_da_inserire);
  }
  int i= 0;
  for (i = 0; i < 6; i++)
    if (pmago->mano[i] == NULL) {
      pmago->mano[i] = carta_da_inserire;

      break;
    }

}
  static void inserisci_campo(struct Mago* pmago, struct Carta* carta_da_inserire) {//per inserire le carte in campo
  int i= 0;
  if (verifica_campo(pmago)==2) //se il campo è pieno restituisco il messaggio di errore ed elimino la carta
  {
    printf("campo finito, distruggo la carta");
    free(carta_da_inserire);
    return;
  }
  for (i = 0; i < 4; i++)
    if (pmago->campo[i] == NULL) {
      pmago->campo[i] = carta_da_inserire;
      break;
    }

  }

//funzioni che riguardano la stampa a schermo
  /*static void stampa_mazzo(struct Mago* pmago) {
    if (pmago->prima_carta == NULL)
    printf( "Non ci sono carte nel mazzo!");
    else {
    struct Carta* ptemp = pmago->prima_carta;
    while (ptemp != NULL) {
    printf("Tipo carta: ");
    if (ptemp->tcarta==0)
      printf("creatura\n" );

    if (ptemp->tcarta==1)
      printf("rimuovi creatura\n" );

    if (ptemp->tcarta==2)
      printf("infliggi danno\n" );

    if (ptemp->tcarta==3)
      printf("rimuovi creatura\n" );
    printf("Punti vita carta: %.1f\n", ptemp->punti_vita);
    ptemp = ptemp-> carta_successiva;
    }
}
}*/
  static void stampa_mano(struct Mago* pmago) { //per stampare tutte le carte presenti nella mano di pmago
  int i=0;
  struct Carta* pscan=pmago->mano[0];

 for (i = 0; i < 6; i++){
 pscan=pmago->mano[i];

 if (pscan==NULL){  //per far comparire a schermo che quella posizione dell'array mano non ha carte
 printf ("              ");
 continue;
 }
 //printf (" pv %.1f tipo carta ",pscan->punti_vita);  //per far comparire a schermo i punti vita e il tipo di carta collegato ad enum tipo carta
 if (pscan->tcarta==0)
   printf("  +----------+" );

 if (pscan->tcarta==1)
   printf("  +----------+" );

 if (pscan->tcarta==2)
   printf("  +----------+" );

 if (pscan->tcarta==3)
   printf("  +----------+" );
 }

 printf("\n");

 for (i = 0; i < 6; i++){
 pscan=pmago->mano[i];

     if (pscan==NULL){  //per far comparire a schermo che quella posizione dell'array mano non ha carte
     printf ("              ");
     continue;
     }

     if (pscan->tcarta==0)
       printf("  |          |" );

     if (pscan->tcarta==1)
       printf("  |          |" );

     if (pscan->tcarta==2)
       printf("  |          |" );

     if (pscan->tcarta==3)
       printf("  |          |" );
 }

 printf("\n");

 for (i = 0; i < 6; i++){
 pscan=pmago->mano[i];

 if (pscan==NULL){  //per far comparire a schermo che quella posizione dell'array mano non ha carte
  printf ("              ");
 continue;
 }

 if (pscan->tcarta==0)
   printf("  | creatura |" );

 if (pscan->tcarta==1)
   printf("  | rimuovi  |" );

 if (pscan->tcarta==2)
   printf("  | infliggi |" );

 if (pscan->tcarta==3)
   printf("  | rimuovi  |" );
 }

 printf("\n");

 for (i = 0; i < 6; i++){
 pscan=pmago->mano[i];

 if (pscan==NULL){  //per far comparire a schermo che quella posizione dell'array mano non ha carte
 printf ("              ");
 continue;
 }

 if (pscan->tcarta==0)
   printf("  |          |" );

 if (pscan->tcarta==1)
   printf("  | creatura |" );

 if (pscan->tcarta==2)
   printf("  |  danno   |" );

 if (pscan->tcarta==3)
   printf("  |  danno   |" );
 }

 printf("\n");

 for (i = 0; i < 6; i++){
 pscan=pmago->mano[i];

     if (pscan==NULL){  //per far comparire a schermo che quella posizione dell'array mano non ha carte
     printf ("              ");
     continue;
     }
     //printf (" pv %.1f tipo carta ",pscan->punti_vita);  //per far comparire a schermo i punti vita e il tipo di carta collegato ad enum tipo carta
     if (pscan->tcarta==0)
       printf("  |          |" );

     if (pscan->tcarta==1)
       printf("  |          |" );

     if (pscan->tcarta==2)
       printf("  |          |" );

     if (pscan->tcarta==3)
       printf("  |          |" );
 }

 printf("\n");

 for (i = 0; i < 6; i++){
 pscan=pmago->mano[i];
 if (pscan==NULL){
   printf ("              "); }
 else {
   if (pscan->punti_vita<=9){
     printf ("  |  %.1f pv  |",pscan->punti_vita);}
     else{
       printf ("  |  %.1f pv |",pscan->punti_vita);
       }
     }
   }

 printf("\n");

 for (i = 0; i < 6; i++){
 pscan=pmago->mano[i];

     if (pscan==NULL){  //per far comparire a schermo che quella posizione dell'array mano non ha carte
     printf ("              ");
     continue;
     }
     //printf (" pv %.1f tipo carta ",pscan->punti_vita);  //per far comparire a schermo i punti vita e il tipo di carta collegato ad enum tipo carta
     if (pscan->tcarta==0)
       printf("  |          |" );

     if (pscan->tcarta==1)
       printf("  |          |" );

     if (pscan->tcarta==2)
       printf("  |          |" );

     if (pscan->tcarta==3)
       printf("  |          |" );
 }

 printf("\n");

 for (i = 0; i < 6; i++){
 pscan=pmago->mano[i];

 if (pscan==NULL){  //per far comparire a schermo che quella posizione dell'array mano non ha carte
 printf ("              ");
 continue;
 }
 //printf (" pv %.1f tipo carta ",pscan->punti_vita);  //per far comparire a schermo i punti vita e il tipo di carta collegato ad enum tipo carta
 if (pscan->tcarta==0)
   printf("  +----------+" );

 if (pscan->tcarta==1)
   printf("  +----------+" );

 if (pscan->tcarta==2)
   printf("  +----------+" );

 if (pscan->tcarta==3)
   printf("  +----------+" );
 }

 printf("\n");

printf(" 1)            2)            3)            4)            5)            6)\n");



}
  static void stampa_campo(struct Mago* pmago) {//per stampare tutte le carte predenti nel campo di pmago
  int i=0;

  if (verifica_campo(pmago)==0){
    printf ("\nnon ci sono carte nel campo di %s", pmago->nome_mago);
    }

    else{

      printf ("\ncarte giocate da %s", pmago->nome_mago);


      struct Carta* pscan=pmago->campo[0];

      for (i = 0; i < 4; i++){
      pscan=pmago->campo[i];

      if (pscan==NULL){  //per far comparire a schermo che quella posizione dell'array mano non ha carte
      printf ("                     ");
      continue;
      }
      //printf (" pv %.1f tipo carta ",pscan->punti_vita);  //per far comparire a schermo i punti vita e il tipo di carta collegato ad enum tipo carta
      if (pscan->tcarta==0)
       printf("         +----------+" );

      if (pscan->tcarta==1)
       printf("         +----------+" );

      if (pscan->tcarta==2)
       printf("         +----------+" );

      if (pscan->tcarta==3)
       printf("         +----------+" );
      }

      printf("\n");

      for (i = 0; i < 4; i++){
      pscan=pmago->campo[i];

         if (pscan==NULL){  //per far comparire a schermo che quella posizione dell'array mano non ha carte
         printf ("                     ");
         continue;
         }
         //printf (" pv %.1f tipo carta ",pscan->punti_vita);  //per far comparire a schermo i punti vita e il tipo di carta collegato ad enum tipo carta
         if (pscan->tcarta==0)
           printf("         |          |" );

         if (pscan->tcarta==1)
           printf("         |          |" );

         if (pscan->tcarta==2)
           printf("         |          |" );

         if (pscan->tcarta==3)
           printf("         |          |" );
      }

      printf("\n");

      for (i = 0; i < 4; i++){
      pscan=pmago->campo[i];

      if (pscan==NULL){  //per far comparire a schermo che quella posizione dell'array mano non ha carte
      printf ("                     ");
      continue;
      }
      //printf (" pv %.1f tipo carta ",pscan->punti_vita);  //per far comparire a schermo i punti vita e il tipo di carta collegato ad enum tipo carta
      if (pscan->tcarta==0)
       printf("         | creatura |" );

      if (pscan->tcarta==1)
       printf("         | rimuovi  |" );

      if (pscan->tcarta==2)
       printf("         | infliggi |" );

      if (pscan->tcarta==3)
       printf("         | rimuovi  |" );
      }

      printf("\n");

      for (i = 0; i < 4; i++){
      pscan=pmago->campo[i];

      if (pscan==NULL){  //per far comparire a schermo che quella posizione dell'array mano non ha carte
      printf ("                     ");
      continue;
      }

      if (pscan->tcarta==0)
       printf("         |          |" );

      if (pscan->tcarta==1)
       printf("         | creatura |" );

      if (pscan->tcarta==2)
       printf("         |  danno   |" );

      if (pscan->tcarta==3)
       printf("         |  danno   |" );
      }

      printf("\n");

      for (i = 0; i < 4; i++){
      pscan=pmago->campo[i];

         if (pscan==NULL){  //per far comparire a schermo che quella posizione dell'array mano non ha carte
         printf ("                     ");
         continue;
         }
         //printf (" pv %.1f tipo carta ",pscan->punti_vita);  //per far comparire a schermo i punti vita e il tipo di carta collegato ad enum tipo carta
         if (pscan->tcarta==0)
           printf("         |          |" );

         if (pscan->tcarta==1)
           printf("         |          |" );

         if (pscan->tcarta==2)
           printf("         |          |" );

         if (pscan->tcarta==3)
           printf("         |          |" );
      }

      printf("\n");

      for (i = 0; i < 4; i++){
      pscan=pmago->campo[i];
      if (pscan==NULL){
       printf ("                     "); }
      else {
       if (pscan->punti_vita<=9){
         printf ("         |  %.1f pv  |",pscan->punti_vita);}
         else{
           printf ("        |  %.1f pv |",pscan->punti_vita);
           }
         }
       }

      printf("\n");

      for (i = 0; i < 4; i++){
      pscan=pmago->campo[i];

         if (pscan==NULL){  //per far comparire a schermo che quella posizione dell'array mano non ha carte
         printf ("                     ");
         continue;
         }
         //printf (" pv %.1f tipo carta ",pscan->punti_vita);  //per far comparire a schermo i punti vita e il tipo di carta collegato ad enum tipo carta
         if (pscan->tcarta==0)
           printf("         |          |" );

         if (pscan->tcarta==1)
           printf("         |          |" );

         if (pscan->tcarta==2)
           printf("         |          |" );

         if (pscan->tcarta==3)
           printf("         |          |" );
      }

      printf("\n");

      for (i = 0; i < 4; i++){
      pscan=pmago->campo[i];

      if (pscan==NULL){  //per far comparire a schermo che quella posizione dell'array mano non ha carte
      printf ("                     ");
      continue;
      }
      //printf (" pv %.1f tipo carta ",pscan->punti_vita);  //per far comparire a schermo i punti vita e il tipo di carta collegato ad enum tipo carta
      if (pscan->tcarta==0)
       printf("         +----------+" );

      if (pscan->tcarta==1)
       printf("         +----------+" );

      if (pscan->tcarta==2)
       printf("         +----------+" );

      if (pscan->tcarta==3)
       printf("         +----------+" );
      }

      printf("\n");

        printf("       1)                   2)                   3)                   4)\n");

    }
}

//funzioni che riguardano la verifica
  static int  verifica_campo(struct Mago* pmago) {//verifica che in campo ci siano carte, se ci sono carte restituisce 1, se non ci sono restituisce 0, se è tutto pieno restituisce 2
  if (pmago->campo[0]==NULL && pmago->campo[1]==NULL && pmago->campo[2]==NULL && pmago->campo[3]==NULL) // se tutte le posizioni dell'array campo sono vuote
   {return 0;}
  else
  if (pmago->campo[0]==NULL && pmago->campo[1]==NULL && pmago->campo[2]==NULL && pmago->campo[3]==NULL) // se tutte le posizioni dell'array campo sono piene
  {return 2;}
  else
  {return 1;}
}
  static int  verifica_mano(struct Mago* pmago) {//ritorna 0 se ci sono posti disponibili 1 se pieno 2 se tutto vuoto
    if (pmago->mano[0]!=NULL && pmago->mano[1]!=NULL && pmago->mano[2]!=NULL && pmago->mano[3]!=NULL && pmago->mano[4]!=NULL && pmago->mano[5]!=NULL) // se tutte le posizioni dell'array mano sono piene
     {return 1;}
    else
    if (pmago->mano[0]==NULL && pmago->mano[1]==NULL && pmago->mano[2]==NULL && pmago->mano[3]==NULL && pmago->mano[4]==NULL && pmago->mano[5]==NULL)// se tutte le posizioni sono vuote
    {return 2;}
    else
    {return 0;}


  }

//funzioni con fine esclusivamente estetico
  static void layout_imposta_gioco(){
  system("clear");
  printf("\n+----------------------------------------------------------------------------------+\n");
  printf("|                          BENVENUTO, IMPOSTIAMO IL GIOCO!!                        |\n");
  printf("+----------------------------------------------------------------------------------+\n");
}
  static void layout_combatti(struct Mago* pmago) {

  system("clear");
  printf("\n+----------------------------------------------------------------------------------+\n");
  printf("|                                     COMBATTI!                                    |\n");
  printf("+----------------------------------------------------------------------------------+\n");

  printf ("\n                                 è il turno di ");
  puts(pmago->nome_mago);


  printf ("\n                                       %.1f pv\n", pmago->punti_vita);

}
  static void layout_turno_terminato(){
    system("clear");
    printf("\n+----------------------------------------------------------------------------------+\n");
    printf("|##################################################################################|\n");
    printf("|##################################################################################|\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("|              ########## ##      ##  #########  ####  ##  #########               |\n");
    printf("|                  ##     ##      ##  ##     ##  ## ## ##  ##     ##               |\n");
    printf("|                  ##     ##      ##  #########  ##  ####  ##     ##               |\n");
    printf("|                  ##     ##      ##  ##   ##    ##   ###  ##     ##               |\n");
    printf("|                  ##     ##########  ##     ##  ##    ##  #########               |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("| ######## ######## #########  ##      ##  ##  ####  ##    #### ######## ######### |\n");
    printf("|    ##    ##       ##     ##  ####  ####  ##  ## ## ##   ##  ##   ##    ##     ## |\n");
    printf("|    ##    ######   #########  ## #### ##  ##  ##  ####  ##    ##  ##    ##     ## |\n");
    printf("|    ##    ##       ##   ##    ##  ##  ##  ##  ##   ###  ########  ##    ##     ## |\n");
    printf("|    ##    ######## ##     ##  ##      ##  ##  ##    ##  ##    ##  ##    ######### |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("|##################################################################################|\n");
    printf("|##################################################################################|\n");
    printf("+----------------------------------------------------------------------------------+\n");
    attendi();

  }
  static void attendi(){// attende che il giocatore prema un tasto per continuare
    fseek(stdin,0,SEEK_END); //pulisco il buffer
    while (getchar() != '\n'); //attendo che l'utente prema invio
  }

//funzioni che riguardano il combattimento
  static void turno_mago (struct Mago* pmago, struct Mago* pmago_nemico) {//questa funzione si occupa del singolo turno durante il combattimento tra i maghi
//questa funzione entra nel menù combatti del mago, permette di stampare il campo, stampare la mano, giocare le carte attaccare e passare
//attribuisce alla variabile turno un valore intero 1 o 2 che rappresenta il turno di chi deve giocare(se il mago 1 o il mago 2)
//questi valori si alterneranno in modo tale da alternare i turni tra i due giocatori
//questa funzione viene richiamata in loop nella funzione combatti cosi che si da luogo all'alternare dei turni, e quindi al combattimento


int npesca=0, ngioco=0, nattacco=0, passa=0, scelta=1, scelta_termina_partita=0;

do {
    do {
        layout_combatti(pmago); //serve solamente per un layout, funzione esclusivamente grafica

        printf("\n                                        menù\n");
        printf("\n 1) stampa campo \n 2) stampa mano \n 3) pesca carta \n 4) gioca carta \n 5) attacca \n 6) passa turno \n\n 7) torna al menù principale\n\n");
        fseek(stdin,0,SEEK_END); //pulisco il buffer
        scelta=0; //perchè nel caso in cui durante lo scanf si inserisce un valore non valido come una lettera, scelta rimane al valore precedente e entra nel menù rispettivo. inizializzandola si evita questo problema.
        scanf ("%d", &scelta); //scansiono la scelta
        switch (scelta) {

          case 1: //stampa campo
              layout_combatti(pmago);
              printf("\n");
              stampa_campo(pmago);
              stampa_campo(pmago_nemico);
              attendi();
            break;
          case 2: //stampa mano
              layout_combatti(pmago);
              stampa_mano(pmago);
              attendi();
            break;
          case 3: //pesca carta
                if (npesca==1){
                  layout_combatti(pmago);
                  printf("\npuoi pescare solo una carta per turno\n");
                  attendi();
                  break;
                }
              layout_combatti(pmago);
              printf("\n                                     pesca carta:\n");
              pesca_carta(pmago);
              npesca=1;
              attendi();

            break;
          case 4://gioca carta
                if (ngioco==1){
                  layout_combatti(pmago);
                  printf("\npuoi giocare solo una carta per turno\n");
                  attendi();
                  break;
                }
              layout_combatti(pmago);
              ngioco=gioca_carta(pmago, pmago_nemico);  //puntatori al primo e al secondo mago, in questo caso pmago rappresenta il mago con cui si sta giocando e pmago_nemico rappresenta il mago nemico

            break;
          case 5:
                if (nattacco==1){
                  layout_combatti(pmago);
                  printf("\npuoi attaccare solo una volta per turno\n");
                  attendi();
                  break;
                }
              layout_combatti(pmago);
              nattacco=attacca(pmago, pmago_nemico);

            break;
          case 6:
              passa=1;
              layout_turno_terminato();
              break;
          case 7:
              do {
                layout_combatti(pmago);
                printf("\ntornando al menù precedente terminerai la partita: sei sicuro di procedere?\n");
                printf("\n 1) si, torna al menù precedente\n 2) no, continua partira\n");
                scelta_termina_partita=0;
                scanf("%d", &scelta_termina_partita);

                if (scelta_termina_partita<1 || scelta_termina_partita>2){
                  layout_combatti(pmago);
                  printf("\nscelta non valida, inserire un numero compreso tra 1 e 2");
                  attendi();
                }
              }while (scelta_termina_partita<1 || scelta_termina_partita>2);

              switch(scelta_termina_partita){
              case 1:
                resetta_memoria(pmago);
                resetta_memoria(pmago_nemico);
                stato_imposta_gioco=0;
                termina=1; //variabile globale, se 1 la interrompe
                return;
              case 2:
                continue;  //se si decide di continuare, il comando continue ti riporta all'inizio del secondo do
              }

          default:
            printf ("\nscelta non valida, inserire un numero da 1 a 7");
            attendi();
            break;
          }
        }while (scelta <1 || scelta>7);
       if (npesca==1 && ngioco==1 && nattacco==1){
         layout_combatti(pmago);
         printf("\nhai esaurito le mosse possibii in questo turno\n");
         attendi();
         layout_turno_terminato();
       }
     }while ((npesca==0 || ngioco==0 || nattacco==0)&&passa==0);//finche uno di questi è uguale a 0


if (turno==1){
  turno=2;
  return;
  }

else{
  turno=1;
  return;
  }
}
  static int seleziona_creatura(struct Mago* pmago){ //il suo scopo è quello di permettere di selezionare l'indice dell'array campo alla quale corrisponde una creatura(e restituisce tale valore), evita errori o imput sbagliati,
  int creatura_da_scegliere;
  do {
    layout_combatti(pmago);

    stampa_campo(pmago);

    printf("\n\n5) torna al menù precedente\n\n");

    creatura_da_scegliere=0;

    scanf("%d", &creatura_da_scegliere);
    --creatura_da_scegliere; //per far si che creatura_da_scegliere coincida con gli indici dell'array (da 0 a 3)

    if (creatura_da_scegliere==4){
      return 4;
    }

    if (creatura_da_scegliere <0 || creatura_da_scegliere >4){
        layout_combatti(pmago);
        printf("\nscelta non valida, inserire un numero da 1 a 5\n");
        attendi();
        continue;}

    if (pmago->campo[creatura_da_scegliere]==NULL){
        layout_combatti(pmago);
        printf("\nscelta non valida, non puoi scegliere una posizione vuota\n");
        attendi();}

  } while ((creatura_da_scegliere <0 || creatura_da_scegliere >4)||pmago->campo[creatura_da_scegliere]==NULL);
  return creatura_da_scegliere;
}
  static int gioca_carta(struct Mago* pmago, struct Mago* pmago_nemico) {//il suo scopo è quello di far giocare una carta presente nella mano del mago, a seconda di che carta è implementa varie possibilità

  int carta_freccia; //è la carta scelta dalla mano per essere giocata
  int carta_bersaglio=1;// è la carta colpita dalla carta freccia (nel si scelga l'opzione rimuovi creatura, infliggi danno e guarisci danno)

  if (verifica_mano(pmago)==2){//se nella mano non ci sono carte restituisce un messaggio di errore
    printf("non puoi giocare nessuna carta, la mano è vuota");
    attendi();
    return 0;
  }


  do { //ciclo che fa scegliere all'utente la carta fraccia
      layout_combatti(pmago);//serve solamente per un layout, funzione esclusivamente grafica
      printf("\n                            scegli quale carta giocare: \n\n");

      stampa_mano(pmago);//per stampare a video le carte (che poi il giocatore potrà scegliere)

      printf ("\n7) torna al menù precedente\n\n");

      carta_freccia=0;
      scanf("%d", &carta_freccia);//per far si che lutente possa scegliere la carta freccia (guardado quelle stampate a schermo poco prima)
      --carta_freccia; //per far si che carta_freccia coincida con gli indici dell'array (da 0 a 5)

      if (carta_freccia==6){
        return 0;
      }

      if (carta_freccia <0 || carta_freccia >6){
        layout_combatti(pmago);//serve solamente per un layout, funzione esclusivamente grafica
        printf("\ncarta_freccia non valida, inserire un numero da 1 a 6\n");
        attendi();
        continue;
      }
      if (pmago->mano[carta_freccia]==NULL){
        layout_combatti(pmago);//serve solamente per un layout, funzione esclusivamente grafica
        printf("\nscelta non valida, non puoi scegliere una posizione vuota\n");
        attendi();
      }
  } while ((carta_freccia <0 || carta_freccia >6)||pmago->mano[carta_freccia]==NULL);


      switch (pmago->mano[carta_freccia]->tcarta) { //determina automaticamente il tipo di carta che si è giocata
        case 0:  //se è una carta di tipo creatura
        layout_combatti(pmago); //serve solamente per un layout, funzione esclusivamente grafica

        printf ("\n hai scelto una carta creatura:\n\n la creatura è stata inserita in campo\n");
              inserisci_campo(pmago, pmago->mano[carta_freccia]);
              pmago->mano[carta_freccia]=NULL; //prima nella mano c'era il puntatore della carta carta_freccia, ora viene trasferito in campo (e di conseguenza eliminato dalla mano)
              attendi();

          break;
        case 1:  //se è una carta di tipo rimuovi creatura
        layout_combatti(pmago); //serve solamente per un layout, funzione esclusivamente grafica

          if (verifica_campo(pmago_nemico)==0){//se non ci sono carte nel campo nemico non si può giocare la carta
            layout_combatti(pmago); //serve solamente per un layout, funzione esclusivamente grafica
            printf("\nil campo nemico è vuoto, non puoi giocare la tua carta\n");
            attendi();
            return 0;}


          layout_combatti(pmago);

          printf ("\n hai scelto una carta rimuovi creatura:");
          printf ("\n selezionare la creatura da rimuovere\n");


          carta_bersaglio=seleziona_creatura(pmago_nemico); //carta obiettivo è la carta su cui deve agire la carta scelta (int carta_bersaglio rappresenta l'indice dell'array campo in cui si trova il puntatore della carta, viene fornito dalla funzione seleziona cretura)
          //la funzione selezione creatura stampa il campo, permette di selezionare la carta bersagio presente sul campo e restituisce l'indice del suo array assegnandolo a carta obiettivo
          if (carta_bersaglio==4){return 0;}//nel caso in cui si è scelto di tornare al menù precedente

          free(pmago_nemico->campo[carta_bersaglio]); //rimuove la carta creatura che è stata presa come bersaglio
          pmago_nemico->campo[carta_bersaglio]=NULL;

          free(pmago->mano[carta_freccia]); //si rimuove anche la carta_freccia perchè è stata usata
          pmago->mano[carta_freccia]=NULL;

          layout_combatti(pmago);
          printf("la carta è stata rimossa con successo!\n");

          attendi();

          break;
        case 2:  //se è una carta di tipo infliggi danno
          fseek(stdin,0,SEEK_END); //pulisco il buffer
          int scelta_danno=1;

          do{//menù per far scegliere all'utente se arrecare danni ad una creatura nemica o al mago nemico
            layout_combatti(pmago);//serve solamente per un layout, funzione esclusivamente grafica
            printf ("\nhai scelto una carta infliggi danno:");
            printf ("\na chi vuoi ingliggere danno?\n");
            printf ("\n 1) ad una creatura nemica\n 2) al mago %s\n\n 3)torna al menù precedente", pmago_nemico->nome_mago);
            scanf ("%d",&scelta_danno);
            if (scelta_danno==3){
              return 0;
            }
            if (scelta_danno!=1 && scelta_danno !=2){
              layout_combatti(pmago);
              printf ("\n          scelta non valida, inserire un numero compreso tra 1 e 2\n");
              attendi();
              }
          }while (scelta_danno!=1 && scelta_danno !=2);


          switch (scelta_danno) {  //switch per decidere se arrecare dano ad una carta nemica o al mago nemico
            case 1://caso in cui si volgia arrecare danno alla creatura
                  layout_combatti(pmago);//serve solamente per un layout, funzione esclusivamente grafica
                  printf("attacco a creatura:\n");

                  carta_bersaglio=0;
                  if (verifica_campo(pmago_nemico)==0){//se il campo nemico è vuoto torna indietro in quanto non si puo giocare la carte
                    layout_combatti(pmago);//serve solamente per un layout, funzione esclusivamente grafica
                    printf("\nnon puoi giocare la tua carta, il campo nemico è vuoto\n");
                    attendi();
                    return 0;}

                  carta_bersaglio=seleziona_creatura(pmago_nemico);
                  if (carta_bersaglio==4){return 0;}//nel caso in cui si è scelto di tornare al menù precedente

                  if (pmago_nemico->campo[carta_bersaglio]->punti_vita<=pmago->mano[carta_freccia]->punti_vita){ //nel caso in cui la creatura rimane a 0 punti viene eliminata
                    free (pmago_nemico->campo[carta_bersaglio]); //dealloco lo spazio in memoria
                    pmago_nemico->campo[carta_bersaglio]=NULL; //elimino il puntatore in campo
                    layout_combatti(pmago);
                    printf ("\ndevastante! i punti vita della creatura scendono a 0, \nl'hai eliminata!\n");
                  }

                  else {//sennò calcolo i punti rimanenti con una sottrazione e li assegno come nuovo valore pv della carta bersagio
                  pmago_nemico->campo[carta_bersaglio]->punti_vita = pmago_nemico->campo[carta_bersaglio]->punti_vita-pmago->mano[carta_freccia]->punti_vita;
                  layout_combatti(pmago);
                  printf ("\ndevastante! i punti vita della creatura scendono a %.1f\n", pmago_nemico->campo[carta_bersaglio]->punti_vita);
                  }

                free(pmago->mano[carta_freccia]); //si rimuove anche la carta carta_freccia perchè è stata usata
                pmago->mano[carta_freccia]=NULL;
                attendi();

              break;

            case 2: //caso in cui si voglia arrecare danno al mago nemico
            layout_combatti(pmago);
              printf("\n                      attacco a mago\n");

              if (pmago_nemico->punti_vita<=pmago->mano[carta_freccia]->punti_vita){//nel caso in cui viene ucciso il mago nemico
              pmago_nemico->punti_vita=0;
              vittoria();
              break;}

              layout_combatti(pmago);
              pmago_nemico->punti_vita = pmago_nemico->punti_vita-pmago->mano[carta_freccia]->punti_vita;
              fseek(stdin,0,SEEK_END); //pulisco il buffer
              printf("\ngran colpo!\n");
              printf("%s rimane a %.1f punti vita", pmago_nemico->nome_mago, pmago_nemico->punti_vita);

              free(pmago->mano[carta_freccia]); //si rimuove anche la carta carta_freccia perchè è stata usata
              pmago->mano[carta_freccia]=NULL;
              attendi();

          }
          break;
        case 3:  //se è una carta di tipo guarisci danno
        layout_combatti(pmago);
          printf ("hai scelto una carta guarisci danno\n");
          int scelta_guarisci;
          do{
            printf ("\na chi vuoi incrementare i punti vita?\n\n");    //per decidere se guarire il danno alla propria creatura o a se stessi
            printf (" 1) una tua creatura \n 2) a te stesso\n \n\n 3) torna al menù precedente");
            scelta_guarisci=0;
            scanf ("%d",&scelta_guarisci);
            if (scelta_guarisci==3){
              return 0;}
            if (scelta_guarisci!=1 && scelta_guarisci !=2){
              layout_combatti(pmago);
              printf ("scelta non valida, inserire un numero compreso tra 1 e 2\n");
              attendi();
            }
          }while (scelta_guarisci!=1 && scelta_guarisci !=2);

          switch (scelta_guarisci) {  //switch //per decidere se guarire il danno alla propria creatura o a se stessi
            case 1://caso in cui si volgia incrementare i pv alla creatura

                  if (verifica_campo(pmago)==0){ //nel caso in cui il campo sia vuoto non si può giocare la carta guarisci su una creatura (perche non ve ne sono)
                    layout_combatti(pmago);
                    printf("\nil tuo campo è vuoto, non puoi giocare la tua carta\n");
                    attendi();
                    return 0;}

                  layout_combatti(pmago);
                  printf("\nselezionare la creatura da guarire:\n");
                  int carta_bersaglio;

                  carta_bersaglio=seleziona_creatura(pmago);
                  if (carta_bersaglio==4){return 0;}//nel caso in cui si è scelto di tornare al menù precedente

                    {pmago->campo[carta_bersaglio]->punti_vita = pmago->campo[carta_bersaglio]->punti_vita+pmago->mano[carta_freccia]->punti_vita;
                    layout_combatti(pmago);
                    printf("ora la tua creatura ha %.1f punti vita\n", pmago->campo[carta_bersaglio]->punti_vita);

                    free(pmago->mano[carta_freccia]); //si rimuove la carta carta_freccia perchè è stata usata
                    pmago->mano[carta_freccia]=NULL;
                    attendi();

                    }
                    break;


            case 2: //nel caso si vogliano incrementare i pv di se stessi
              layout_combatti(pmago);
              pmago->punti_vita = pmago->punti_vita+pmago->mano[carta_freccia]->punti_vita;
              printf("ora hai %.1f punti vita", pmago->punti_vita);

              free(pmago->mano[carta_freccia]); //si rimuove la carta carta_freccia perchè è stata usata
              pmago->mano[carta_freccia]=NULL;

              attendi();

            break;
          }
          break;
        }
      return 1;
    }
  static int attacca(struct Mago* pmago, struct Mago* pmago_nemico) {//il suò scopo è permettere di ataccare con una carta presente nel campo del mago
  int carta_freccia=1;

  if (verifica_campo(pmago)==0){//andiamo a verificare se nel proprio campo ci sono carte, se non ci sono è impossibile attaccare, si ritorna indietro.
    layout_combatti(pmago);
    printf("\nnon hai carte in campo, non puoi attaccare\n");
    attendi();
    return 0;}

  printf("\nseleziona la creatura con cui attaccare\n");

  carta_freccia=seleziona_creatura(pmago);//richiamo la funzione seleziona creatura per assegnare alla variabile carta_freccia l'indice dell'array corrispettivo alla creatura del proprio campo scelta
  if (carta_freccia==4){return 0;}//nel caso in cui si è scelto di tornare al menù precedente

  int scelta_danno=1;

  do{
    layout_combatti(pmago);//serve solamente per un layout, funzione esclusivamente grafica
    printf ("\n                             attacca!:");
    printf ("\n                     seleziona chi attaccare:\n");    //per decidere se arrecare dano ad una carta nemica o al mago nemico
    printf ("\n 1) una creatura nemica\n 2) il mago %s", pmago_nemico->nome_mago);
    scanf ("%d",&scelta_danno);
    if (scelta_danno!=1 && scelta_danno !=2){
      layout_combatti(pmago);
      printf ("\nscelta non valida, inserire un numero compreso tra 1 e 2\n");
      attendi();


    }
  }while (scelta_danno!=1 && scelta_danno !=2);

  switch (scelta_danno) {  //switch per decidere se arrecare dano ad una carta nemica o al mago nemico
    case 1://caso in cui si volgia arrecare danno alla creatura
          layout_combatti(pmago);//serve solamente per un layout, funzione esclusivamente grafica
          printf("\n                               attacco a creatura:\n");


          if (verifica_campo(pmago_nemico)==0){ //nel caso in cui il campo nemico fosse vuoto restituisce un messaggio di errore e torna al menu precedente
            layout_combatti(pmago);//serve solamente per un layout, funzione esclusivamente grafica
            printf("il campo nemico è vuoto, non puoi giocare la tua carta\n");
            attendi();
            return 0;}

          int carta_bersaglio=0;
          carta_bersaglio=seleziona_creatura(pmago_nemico);
          if (carta_bersaglio==4){return 0;}//nel caso in cui si è scelto di tornare al menù precedente

          //a questo punto le creature si scontreranno andando a perdere ognuna i rispettivi pv dell'altra

          int temp; //temp è una variabile di supporto
          temp=pmago_nemico->campo[carta_bersaglio]->punti_vita;//assegno i pv della carta bersagio a una variabile temporanea

          if (pmago_nemico->campo[carta_bersaglio]->punti_vita<=pmago->campo[carta_freccia]->punti_vita){ //nel caso in cui la creatura bersaglio rimane a 0 punti viene eliminata
            free (pmago_nemico->campo[carta_bersaglio]);
            pmago_nemico->campo[carta_bersaglio]=NULL;
            layout_combatti(pmago);
            printf ("\ndevastante! i punti vita della creatura nemica scendono a 0, \nl'hai eliminata!\n");
            attendi();
          }

            else {//sennò calcolo i punti rimanenti con una sottrazione
              pmago_nemico->campo[carta_bersaglio]->punti_vita = pmago_nemico->campo[carta_bersaglio]->punti_vita-pmago->campo[carta_freccia]->punti_vita;
              layout_combatti(pmago);
              printf ("\ndevastante! i punti vita della creatura nemica scendono a %.1f\n", pmago_nemico->campo[carta_bersaglio]->punti_vita);
              attendi();
          }

          if (pmago->campo[carta_freccia]->punti_vita<=temp){ //nel caso in cui la creatura freccia rimane a 0 punti viene eliminata
            free (pmago->campo[carta_freccia]);
            pmago->campo[carta_freccia]=NULL;
            layout_combatti(pmago);
            printf ("\nla tua creatura non sopporta il colpo avversario e rimane a 0 punti vita\n");
            attendi();
          ;}

            else {//sennò calcolo i punti rimanenti con una sottrazione
              pmago->campo[carta_freccia]->punti_vita = pmago->campo[carta_freccia]->punti_vita-temp;
              layout_combatti(pmago);
              printf ("\nla tua creatura viene colpita e scende a %.1f punti vita\n", pmago->campo[carta_bersaglio]->punti_vita);
              attendi();
            }
          return 1;
          break;



    case 2: //caso in cui si voglia arrecare danno al mago nemico
    layout_combatti(pmago);
      printf("\n                          attacco a mago\n");

      if (pmago_nemico->punti_vita<=pmago->campo[carta_freccia]->punti_vita){//nel caso in cui il mago attaccato rimanga a 0 pv finisce la partita
      pmago_nemico->punti_vita=0;
      vittoria();
      }

      else{
      layout_combatti(pmago);
      pmago_nemico->punti_vita = pmago_nemico->punti_vita-pmago->campo[carta_freccia]->punti_vita;
      fseek(stdin,0,SEEK_END); //pulisco il buffer
      printf("\ngran colpo! %s rimane a %.1f punti vita", pmago_nemico->nome_mago, pmago_nemico->punti_vita);
      attendi();
      }

      if (pmago->campo[carta_freccia]->punti_vita<=pmago_nemico->punti_vita){ //nel caso in cui la creatura freccia rimane a 0 punti viene eliminata
        free (pmago->campo[carta_freccia]);
        pmago->campo[carta_freccia]=NULL;
        layout_combatti(pmago);
        printf ("\nla tua creatura non sopporta il colpo avversario e rimane a 0 punti vita\n");
        attendi();
      ;}

        else {//sennò calcolo i punti rimanenti con una sottrazione (questo accade nel caso in cui il mago rimane a 0 punti e perciò la partita finisce)
          pmago->campo[carta_freccia]->punti_vita = pmago->campo[carta_freccia]->punti_vita-pmago_nemico->punti_vita;
          layout_combatti(pmago);
          printf ("\nla tua creatura viene colpita e scende a %.1f punti vita\n", pmago->campo[carta_freccia]->punti_vita);
          attendi();
        }
      return 1;
      break;
  }
  return 1;
}

//funzioni chiamate per terminare il gioco
  static void elimina_mazzo(struct Mago* pmago) {

  while (pmago -> prima_carta != NULL){
      struct Carta* ptemp = pmago -> prima_carta;
      if (ptemp -> carta_successiva == NULL) {
        //elimina la carta del mazzo a contatto con il tavolo
        free(pmago -> prima_carta);
        pmago -> prima_carta = NULL;
      }
      else {
        while ((ptemp -> carta_successiva) -> carta_successiva != NULL)
          ptemp = ptemp -> carta_successiva;

          //elimina la carta del mazzo a contatto con il cielo
          free(ptemp -> carta_successiva);
          ptemp -> carta_successiva = NULL;
      }
    }
}
  static void elimina_campo(struct Mago* pmago) {
  int i;
  for (i = 0; i < 4; i++){
    free(pmago->campo[i]);
    pmago->campo[i]=NULL;
  }
}
  static void elimina_mano(struct Mago* pmago) {
  int i;
  for (i = 0; i < 6; i++){
    free(pmago->mano[i]);
    pmago->mano[i]=NULL;
  }
  }
  static void resetta_memoria(struct Mago* pmago){//racchiude in se le tre funzioni precedenti in modo tale da agevolare la scrittura del codice
    elimina_mazzo(pmago);
    elimina_campo(pmago);
    elimina_mano(pmago);
    //pmago->nome_mago=NULL;
    //pmago->punti_vita=NULL;
    //pmago->cmago=NULL;
  }
  static void vittoria() {//è la funzione che viene chiamata nel momento in cui si vince, a seconda di vittoria per uccisione nemica o perchè siano finite le carte del mazzo implementa diverse opzioni
    if (mago1.punti_vita==0){//nel caso in cui il primo mago sia a 0 punti
      printf("\nil colpo è micidiale! %s è stremato e non riesce a contrattaccare:", mago1.nome_mago);
      printf("\ni suoi punti vita sono a zero e %s si aggiudica la vittoria", mago2.nome_mago);
      attendi();
      resetta_memoria(&mago1);
      resetta_memoria(&mago2);
      stato_imposta_gioco=0;//per far si che prima di rigiocare l'utente debba rimpostare il gioco
      termina=1;
      return;
    }
    if (mago2.punti_vita==0){//nel caso in cui il secondo mago sia a 0 punti
      printf("\nil colpo è micidiale! %s è stremato e non riesce a contrattaccare:", mago2.nome_mago);
      printf("\ni suoi punti vita sono a zero e %s si aggiudica la vittoria", mago1.nome_mago);
      attendi();
      resetta_memoria(&mago1);
      resetta_memoria(&mago2);
      stato_imposta_gioco=0;//per far si che prima di rigiocare l'utente debba rimpostare il gioco
      termina=1;
      return;
    }

    if (mago1.punti_vita>mago2.punti_vita){//nel caso in cui siano finte le carte si richiama questa funzione, vince chi ha più pv, caso in cui pv mago1 sono maggiori di pv mago 2
      printf("\nvince %s", mago1.nome_mago);
      printf("\ncon %.1f pv rimasti", mago1.punti_vita);
      printf("\nperde %s", mago2.nome_mago);
      printf("\ncon %.1f pv rimasti", mago2.punti_vita);
      attendi();
      resetta_memoria(&mago1);
      resetta_memoria(&mago2);
      stato_imposta_gioco=0;//per far si che prima di rigiocare l'utente debba rimpostare il gioco
      termina=1;
      return;

    }
    else
    if (mago2.punti_vita>mago1.punti_vita){//nel caso in cui siano finte le carte si richiama questa funzione, vince chi ha più pv, caso in cui pv mago2 sono maggiori di pv mago 1
      printf("\nvince %s", mago2.nome_mago);
      printf("\ncon %.1f pv rimasti", mago2.punti_vita);
      printf("\nperde %s", mago1.nome_mago);
      printf("\ncon %.1f pv rimasti", mago1.punti_vita);
      attendi();
      resetta_memoria(&mago1);
      resetta_memoria(&mago2);
      stato_imposta_gioco=0;//per far si che prima di rigiocare l'utente debba rimpostare il gioco
      termina=1;
      return;
      }
      else//caso in cui i due maghi hanno punti diversi da 0 e uguali tra loro
      printf("\nla partita termina con un pareggio\n");
      attendi();
      resetta_memoria(&mago1);
      resetta_memoria(&mago2);
      stato_imposta_gioco=0;//per far si che prima di rigiocare l'utente debba rimpostare il gioco
      termina=1;
      return;
  }
