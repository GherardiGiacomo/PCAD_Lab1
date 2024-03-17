#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_PHILOSOPHERS 5
#define NUM_CHOPSTICKS 5

pthread_mutex_t chopsticks[NUM_CHOPSTICKS];
int chopstick_available[NUM_CHOPSTICKS];
char chopstick_names[NUM_CHOPSTICKS] = {'A', 'B', 'C', 'D', 'E'};

void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left_chopstick = id;
    int right_chopstick = (id + 1) % NUM_CHOPSTICKS;
    
    srand(time(NULL) + id); // Inizializzazione del generatore di numeri casuali per ciascun filosofo
    
    for (int i = 0; i < 5; i++) {
        // Introduce un ritardo casuale prima di iniziare a pensare
        usleep(rand() % 1000000);
        printf("Filosofo %d: sta pensando\n", id);
        
        // Acquisizione delle bacchette
        pthread_mutex_lock(&chopsticks[right_chopstick]);
        if (!chopstick_available[right_chopstick]) {
            printf("Filosofo %d: Errore - Bacchetta %c già in uso da un altro filosofo\n", id, chopstick_names[right_chopstick]);
            pthread_mutex_unlock(&chopsticks[right_chopstick]);
            continue; // Il filosofo passa al prossimo ciclo
        }
        pthread_mutex_lock(&chopsticks[left_chopstick]);
        if (!chopstick_available[left_chopstick]) {
            printf("Filosofo %d: Errore - Bacchetta %c già in uso da un altro filosofo\n", id, chopstick_names[left_chopstick]);
            pthread_mutex_unlock(&chopsticks[right_chopstick]);
            continue; // Il filosofo passa al prossimo ciclo
        }
        
        // Se entrambe le bacchette sono disponibili, il filosofo le prende
        chopstick_available[right_chopstick] = 0; // Imposta la bacchetta destra come non disponibile
        chopstick_available[left_chopstick] = 0; // Imposta la bacchetta sinistra come non disponibile
        printf("Filosofo %d: ha preso le sue bacchette %c e %c\n", id, chopstick_names[right_chopstick], chopstick_names[left_chopstick]);
        
        printf("Filosofo %d: sta mangiando\n", id);
        usleep(rand() % 1000000); // Attende un tempo casuale prima di smettere di mangiare
        
        // Rilascio delle bacchette
        pthread_mutex_unlock(&chopsticks[left_chopstick]);
        pthread_mutex_unlock(&chopsticks[right_chopstick]);
        chopstick_available[right_chopstick] = 1; // Rilascia la bacchetta destra
        chopstick_available[left_chopstick] = 1; // Rilascia la bacchetta sinistra
        printf("Filosofo %d: ha rilasciato le sue due bacchette\n", id);
    }

    return NULL;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Inizializzazione dei lock per le bacchette
    for (int i = 0; i < NUM_CHOPSTICKS; i++) {
        pthread_mutex_init(&chopsticks[i], NULL);
        chopstick_available[i] = 1; // Inizialmente tutte le posate sono disponibili
    }

    // Creazione dei thread per i filosofi
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Attende che i filosofi finiscano di mangiare
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Rilascia i lock
    for (int i = 0; i < NUM_CHOPSTICKS; i++) {
        pthread_mutex_destroy(&chopsticks[i]);
    }

    return 0;
}
