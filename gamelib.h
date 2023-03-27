#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Dichiarazione delle funzioni da chiamare in main.c
// Definizione dei tipi necessari alla libreria



enum Classe_mago {Tenebre,Vita,Luce};
enum Tipo_carta {creatura,rimuovi_creatura,infliggi_danno,guarisci_danno};

struct Mago {
	char nome_mago[30];
	float punti_vita;
  enum Classe_mago cmago;
	struct Carta* prima_carta;   //rappresenta la carta a contatto con il tavolo	
	struct Carta* mano [6];
	struct Carta* campo [4];
};


struct Carta {
  enum Tipo_carta tcarta;
  float punti_vita;
	struct Carta *carta_successiva;
};

void imposta_gioco();
void combatti();
void termina_gioco();
