# Tragic: Un gioco di carte
Template progetto finale Programmazione Procedurale con Laboratorio a.a. 2021/2022.

Aggiungere a questo README eventuali estensioni al progetto assegnato, e scelte progettuali da comunicare.


"In questo modo si simula il fatto che le carte più potenti si possano lanciare solo con il passare dei turni. Implementare a piacere e commentare nel README di Github la scelta effettuata":

Ho implementato una funzione di tipo void denominata "inizializza_carta". 
Essa ha il compito di attribuire ad una carta diverse informazioni, come il tipo, i punti vita ed altro (vedremo poi in seguito). 
Questa funzione viene richiamata due volte nel programma, in particolare quando si crea il mazzo e quando si crea la prima mano.
Essa prende in ingresso tre parametri: il puntatore pmago, il puntatore New ed una variabile di tipo intero numero_carta.
Vediamo ora i tre scopi della funzione "inizializza_carta"

La prima funzione che ha inizializza_carta è quella di attribuire in modo casuale il tipo di carta (tra creatura, rimuovi_creatura, infliggi_danno e guarisci_danno) alla carta passata tramite il puntatore New che rappresenta una carta appena creata e allocata in memoria tramite malloc (nella mano o nel mazzo).
Per far ciò si genera un numero casuale da 1 a 100 e poi tramite una selezione tra le rispettive probabilità si assegna in modo casuale il tipo di carta alla carta New. 

La seconda funzione di inizializza_carta è quella di simulare il fatto che le carte più potenti si possano lanciare solo con il passare dei turni. 
Per far ciò è necessario conoscere, oltre alla carta da inizializzare, anche il numero che essa rappresenta all'interno del mazzo (successivamente vedremo il caso medesimo ma per la creazione della mano).
Viene dunque passato un valore di tipo intero alla funzione denominato numero_carta. Esso varia da 0 fino a n. 
Ho quindi implementato la formula numero delle carte del mazzo / 5 per dividere in 5 fasce le carte del mazzo. 
Siccome il risultato di questa formula (assegnato alla variabile r) potrebbe non essere un numero intero ho inizializzato la variabile r come int. 
Questo permette di di eliminare la parte decimale del risultato, si può capire meglio il perché di questa scelta da un esempio. 
Mettiamo che il numero di carte del mazzo sia 32. 32/5 è uguale a 6,4. Ciò significa che la prima fascia di carte va da 0 a 6,4 mentre la seconda da 6,4 a 12,8 la terza da 12,8 a 19,2 e così via. 
Ciò risulta illogico in quanto non può esistere la carta 6,4 o la carta 12,8. 
Per risolvere il problema r viene inizializzata come int cosi nel momento in cui si assegna il risultato dell' operazione precedente si eliminerà in automatico la parte decimale. 
32/5= 6. 
In questo modo le fasce di carte vanno da 0 a 5, da 6 a 11, da 12 a 17, da 18 a 23 e da 24 a n_carte_mazzo(32). 
Come si vede nel caso il risultato non sia un numero intero l'ultima fascia conterà più carte delle altre.
(In questo modo il gioco sarà più avvincente in quanto il giocatore avrà più carte con pv bassi) 
Vediamo dunque come si procede e come vengono implementate le fasce di carte. 
Sapendo il numero che rappresenta all'interno del mazzo la carta da inizializzare si può assegnare un numero di punti vita a seconda della fascia a cui appartiene. 
Se il suo numero è compreso tra 0 e r allora si assegneranno un numero di punti vita che oscilla in modo casuale a 4 a 8 in quanto essa rappresenta una delle prime carte del mazzo, ovvero una delle carte "più vicine al tavolo" e quindi che si pescheranno solo dopo molti turni. 
Se invece il numero della carta è compreso nella seconda fascia, e quindi tra r e r*2 il numero casuale a cui si assegneranno i pv oscillerà tra due numeri più piccoli dei precedenti.
Più alta sarà la fascia e più piccoli saranno i numeri tra cui potrà oscillare il numero random di pv da assegnare alla carta.
E nel caso in cui si sta creano la mano e non il mazzo?
In questo caso al posto di passare il numero che la carta rappresenta nel mazzo si passa come parametro il numero di carte del mazzo, perché? 
Perché in questo modo la carta da creare nella mano rientrerà sempre nell'ultima fascia, infatti n_carte_mazzo è maggiore di (n_carte_mazzo/5)*4. Vediamo con un esempio:
n_carte_mazzo = 32 
r=n_carte_mazzo/5 ovvero 6 
6*4 = 24 ovvero il numero da cui parte l'ultima fascia
32 > 24 il numero passato in ingresso è sempre maggiore del numero da cui parte l'ultima fascia. 

In questo modo si è costruito un algoritmo flessibile che può applicare una sola formula per tutti i possibili valori che potrebbe assumere n_carte_mazzo.

L'ultima funzione è quella di cambiare le proprietà di una carta a seconda del tipo del mago che si è passato come parametro. 
Tramite un if vado a selezionare le carte da modificare a seconda della classe del mago. 
Se la classe del mago è tipo 0 e la carta è tipo 2 allora raddoppio i pv. 
Se l classe mago è tipo 1 e la carta è tipo 1 allora pv aumentano della metà 
Se la classe del mago è tipo 2 e la carta è tipo 3 allora i pv aumentano di 3.5 volte. 


Nome:Simone

Cognome:Billeri

Matricola:343523
