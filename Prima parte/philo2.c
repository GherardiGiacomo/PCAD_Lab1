#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUMERO_FILOSOFI 5
#define TOT_MANGIATO 5

//aggiungere spiegazione della regola del buttafuori per evitare il deadlock (se un filosofo non riesce a prendere la bacchetta destra, rilascia la sinistra)

pthread_mutex_t bacchette[NUMERO_FILOSOFI];
int mangiato[NUMERO_FILOSOFI] = {0}; // Array di contatori, serve per contare quante volte ha mangiato ogni filosofo

void* filosofo(void* n) {
    int id = *(int*)n;
    for (int i = 0; i < TOT_MANGIATO; i++) {
        usleep((rand() % 3 + 1) * 1000000); //il filosofo pensa tra 1 e 3 secondi
        printf("Filosofo %d: sta pensando\n", id);
        
        usleep((rand() % 3 + 1) * 1000000);

       if(pthread_mutex_trylock(&bacchette[id])==0){  //il filosofo PROVA a prendere la bacchetta sinistra
        if(pthread_mutex_trylock(&bacchette[(id+1)%NUMERO_FILOSOFI])==0){ //il filosofo PROVA a prendere la bacchetta destra

        printf("Filosofo %d: ha la sua bacchetta sinistra\n", id);
        printf("Filosofo %d: ha la sua bacchetta destra\n", id);
        printf("\nFilosofo %d: sta mangiando\n\n", id);
        
        usleep(1000000); //faccio "mangiare" il filosofo per 1 secondo

        mangiato[id]++;
        printf("\nFilosofo %d: ha mangiato %d volte\n\n", id, mangiato[id]); // Stampo il numero di volte che il filosofo ha mangiato
        pthread_mutex_unlock(&bacchette[id]);
        pthread_mutex_unlock(&bacchette[(id+1)%NUMERO_FILOSOFI]);
        printf("Filosofo %d: ha rilasciato le sue due bacchette\n", id);
        }
        else pthread_mutex_unlock(&bacchette[id]);
        }
    }
    return NULL;

}

int main() {
    srand(time(NULL));
    pthread_t filosofi[NUMERO_FILOSOFI];
    int id[NUMERO_FILOSOFI];

    for (int i = 0; i < NUMERO_FILOSOFI; i++) {
        if(pthread_mutex_init(&bacchette[i], NULL) != 0) {
            printf("Errore nell'inizializzazione del mutex\n");
            exit(1);
        }
        id[i] = i;
        if( pthread_create(&filosofi[i], NULL, filosofo, &id[i]) != 0) {
            printf("Errore nella creazione del thread\n");
            exit(1);
        }
    }
    for (int i = 0; i < NUMERO_FILOSOFI; i++) {
        if(pthread_join(filosofi[i], NULL) != 0) {  //aspetta che il filosofo i-esimo abbia finito di mangiare
            printf("Errore nell'attesa del thread\n");
            exit(1);
        }
    }
    return 0;
}