/*Programma di simulazione di un sistema cellulare.
 *
 * Creato da Perrone Pasquale in data 01-2022 (formato mm-yy)
 *
 * Riassunto traccia: creare un sistema di cellule in una scacchiera nxn n=45 in cui le cellule
 * possono essere vive (*) o morte (-) a seconda del passaggio.
 * Ad ogni passaggio: una cellula è VIVA se ha 3 cellule vicine vive, MORTA se ha 5+ cellule
 * vicine vive e IMMUTATA se ha 1,2,4 cellule vicine vive.
 * Visualizzare i passi 2,3,4,6,8,20,30,45.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 45 //Dimensione scacchiera

//Definizione funzioni

//Funzioni numeriche
int positive_modulo(int i,int n);

//Funzioni di gestione della scacchiera
void copia_scacchiera(char mittente[N][N],char destinatario[N][N]);
void stampa_scacchiera(char scacchiera[N][N]);
void modifica_cellule(char scacchiera[N][N],char scacchiera_modifica[N][N]);
void scacchiera_casualizzata(char scacchiera[N][N],int seed);

int main(){
    int i,j,seed=0;
    char scacchiera[N][N],scacchiera_modifica[N][N];

    //Inizializzazione array
    for(i=0;i<N;i++)

        for(j=0;j<N;j++){
            scacchiera[i][j] = ' ';
            scacchiera_modifica[i][j] = ' ';
        }

    //Scelta della configurazione iniziale
    printf("Programma di simulazione di un sistema di cellule in C. Le configurazioni di cellule disponibili sono le seguenti: \n"
           "1: Le 7 cellule centrali nella riga centrale sono vive.\n2: Le 7 cellule centrali nella colonna centrale sono vive.\n"
           "3: Le cellule centrali (22,22),(22,23),(23,22),(23,23) sono vive.\n 4: Casuale\n 5: Seed\n"
           "Che configurazione iniziale si desidera visualizzare?\n Num: ");
    scanf("%d",&i);
    getchar();

    //Inizializzazione configurazione
    switch(i){
        case 1:
            for(j=19;j<26;j++) scacchiera[0][j] = '*';
            break;
        case 2:
            for(i=19;i<26;i++) scacchiera[i][22] = '*';
            break;
        case 3:
            for(i=22;i<24;i++) for(j=22;j<24;j++) scacchiera[i][j] = '*';
            break;
        case 5:
            printf("Inserire il seed: ");
            scanf("%d",&seed);
            while(seed == 0){
                printf("0 non è un seed valido. Inserire un altro seed: ");
                scanf("%d",&seed);
            }
            //Senza break per sfruttare il fatto che lo switch continua per vari case finchè non trova un break
        case 4:
            scacchiera_casualizzata(scacchiera,seed);
            break;
        default:
           printf("Error in input"); exit(1000);
    }

    //Stampa primo passo
    printf("\nPASSO n. 1 - Posizioni iniziali\n\n");
    stampa_scacchiera(scacchiera);

    //Passi
    for(int passo=2;passo<=N;passo++){
        //Copiare scacchiera in scacchiera_modificata
        copia_scacchiera(scacchiera,scacchiera_modifica);
        //Modificare le cellule
        modifica_cellule(scacchiera,scacchiera_modifica);
        //Copiare scacchiera modificata in scacchiera
        copia_scacchiera(scacchiera_modifica,scacchiera);
        //Printare scacchiera
        if(passo < 5 || passo == 6 || passo == 8 || passo == 20 || passo == 30 || passo == 45) {
            printf("\nPASSO n. %d\n\n",passo);
            stampa_scacchiera(scacchiera);
            system("pause");
        }
    }
    return 0;
}

int positive_modulo(int i, int n) { //Copiato da stack overflow per avere un return solo positivo nei resti
    return (i % n + n) % n;
}

void copia_scacchiera(char mittente[N][N], char destinatario[N][N]){
    //Copia i dati della scacchiera da mittente a destinatario
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            destinatario[i][j] = mittente[i][j];
}

void stampa_scacchiera(char scacchiera[N][N]) {
    //Stampa la scacchiera
    for (int i = 0; i < N; i++) printf("%.45s\n", scacchiera[i]);
}

void modifica_cellule(char scacchiera[N][N],char scacchiera_modifica[N][N]){
    //Controlla la configurazione corrente delle cellule e modifica il loro stato da vive a morte e conserva tali
    //informazioni in scacchiera_modifica
    int vive,istart,iend,jstart,jend;
    for(int i=0;i<N;i++) {
        //Controlliamo se siamo alla prima o ultima riga
        if (i == 0) istart = i; else istart = i-1;
        if (i == N-1) iend = i; else iend = i+1;
        for (int j = 0; j < N; j++) {
            //Azzeriamo il numero di cellule vive
            vive = 0;
            //Controlliamo se siamo alla prima o ultima colonna
            if (j == 0) jstart = j; else jstart = j-1;
            if (j == N - 1) jend = j; else jend = j+1;
            //Ciclo di conteggio. i/jstart e i/jend stabiliscono dove partire così da ignorare righe/colonne inesistenti
            for (int a = istart; a <= iend; a++)
                for (int b = jstart; b <= jend; b++)
                    if (a != i || b != j)
                        if (scacchiera[a][b] == '*')
                            vive++;
            //Modifica stato di vita e morte
            if (vive == 3) scacchiera_modifica[i][j] = '*';
            else if (vive >= 5) scacchiera_modifica[i][j] = ' ';
        }
    }
}

void scacchiera_casualizzata(char scacchiera[N][N],int seed) {
    /*L'algoritmo di seguito genera un pattern pseudo-casuale con rand() seedato tramite time o con seed
    * Con size sceglie il numero di ripetizioni di esso
    * E' limitata a metà per impedire al programma di creare pattern troppo grandi
    * Con direction sceglie se continuare in su (0), giù (1), verso sinistra (2) o verso destra (3)
    * Con x e y sceglie le coordinate in cui iniziare e tiene conto delle coordinate su cui inizia
    * ogni ripetizione
    * expansion stabilisce quanto deve espandersi su ogni riga e colonna a seconda della direzione
    * In alcune operazione si utilizza il resto di 45 per assicurarsi che i valori siano sempre nella zona
    * 0-44 quando potrebbero andare offset (-1,45,etc)
    * Decommentare i 3 printf per una descrizione dell'esecuzione da parte dell'algoritmo stesso
    */
    int size, direction, x, y, expansion, i, j;
    char dirs[][5] = {"Up","Down","Left","Right"};
    if (seed == 0) seed = time(NULL);
    printf("\n\nSEED: %d\n\n", seed);
    srand(seed++); //++ per cambiare il seed al rand() dopo ogni operazione ma tenerlo costante tra le esecuzioni
    size = rand() % 10;
    srand(seed++);
    direction = rand() % 4;
    srand(seed++);
    x = rand() % 45;
    srand(seed++);
    y = rand() % 45;
    printf("\n\nSize: %d\nDirection: %s\n\n",size,dirs[direction]);
    for (i = 0; i < size; i++) {
        if (i != 0) { //Sarebbe superfluo nella prima iterazione del ciclo
            if (direction == 0) x = positive_modulo(x-1,45);
            else if (direction == 1) x = (x + 1) % 45;
            else if (direction == 2) y = positive_modulo(y-1,45);
            else if (direction == 3) y = (y + 1) % 45;
        }
        scacchiera[x][y] = '*';
        srand(seed++);
        expansion = rand()%10;
        printf("Ciclo n. %d\nCoordinate di partenza: %d, %d\nExpansion: %d\n",i+1,x,y,expansion);
        for (j = 1; j <=expansion; j++) {
            if (direction < 2) { //espande su riga se il for principale continua in su e giù, quindi su colonna
                scacchiera[x][(y + j) % 45] = '*';
                scacchiera[x][positive_modulo(y-j,45)] = '*';
                printf("Piazzate due cellule alle coordinate %d %d e %d %d\n",x,(y+j)%45,x,positive_modulo(y-j,45));
            } else { //espande su colonna se il for principale continua in su e giù, quindi su riga
                scacchiera[(x + j) % 45][y] = '*';
                scacchiera[positive_modulo(x-j,45)][y] = '*';
                printf("Piazzate due cellule alle coordinate %d %d e %d %d\n",(x+j)%45,y,positive_modulo(x-j,45),y);
            }
        }
        //Randomizza la coordinata non dipendente da direzione in relazione all'espansione
        srand(seed++);
        if(expansion != 0) {
            expansion = (rand() % expansion * 2) - expansion; //imposta expansion and un numero tra -expansion e +expansion
            j = (direction < 2) ? y+expansion : x+expansion; //cambia x o y affinchè la prossima cellula generata sia connessa ad una
            if (direction < 2) y = positive_modulo(j, 45); //generata precedentemente
            else x = positive_modulo(j, 45);
        } //Non eseguiamo su 0 perchè sennò il programma crasherebbe a rand()%espansion*2 per divisione per 0
    }
    printf("\n");
}