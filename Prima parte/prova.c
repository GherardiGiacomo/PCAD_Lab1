#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_FILOSOFI 5
#define NUM_VOLTE 5

pthread_mutex_t bacchette[NUM_FILOSOFI];
int mangiato[NUM_FILOSOFI] = {0}; // Aggiungi un array di contatori

void* filosofo(void* num) {
    int id = *(int*)num;
    for (int i = 0; i < NUM_VOLTE; i++) {
        printf("Filosofo %d: sta pensando\n", id);
        usleep(1000000); //faccio "pensare" il filosofo n per un po' di tempo

        pthread_mutex_lock(&bacchette[id]);
        pthread_mutex_lock(&bacchette[(id+1)%NUM_FILOSOFI]);
        printf("Filosofo %d: ha la sua bacchetta sinistra\n", id);
        printf("Filosofo %d: ha la sua bacchetta destra\n", id);
        printf("\nFilosofo %d: sta mangiando\n\n", id);
        usleep(1000000); //faccio "mangiare" il filosofo n per un po' di tempo

        mangiato[id]++; // Incrementa il contatore
        printf("Filosofo %d: ha mangiato %d volte\n", id, mangiato[id]); // Stampa il numero di volte che il filosofo ha mangiato
        pthread_mutex_unlock(&bacchette[id]);
        pthread_mutex_unlock(&bacchette[(id+1)%NUM_FILOSOFI]);
        printf("Filosofo %d: ha rilasciato le sue due bacchette\n", id);
    }
    return NULL;
}

int main() {
    pthread_t filosofi[NUM_FILOSOFI];
    int id[NUM_FILOSOFI];

    for (int i = 0; i < NUM_FILOSOFI; i++) {
        pthread_mutex_init(&bacchette[i], NULL);
        id[i] = i;
        pthread_create(&filosofi[i], NULL, filosofo, &id[i]);
    }

    for (int i = 0; i < NUM_FILOSOFI; i++) {
        pthread_join(filosofi[i], NULL);
    }

    return 0;
}