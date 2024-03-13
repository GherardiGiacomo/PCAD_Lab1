#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t bacchette[10];

void *filosofo(void *arg) {
    int id = *(int *)arg;
    int bacchetta_sx = id * 2;
    int bacchetta_dx = (id * 2 + 1) % 10;
    
    for (int i = 0; i < 5; i++) {
        printf("Filosofo %d: sta pensando\n", id);
        usleep(1000000); //faccio "pensare" il filosofo n per un po' di tempo
        
        pthread_mutex_lock(&bacchette[bacchetta_sx]);
        printf("Filosofo %d: ha preso la sua bacchetta di sinistra\n", id);
        pthread_mutex_lock(&bacchette[bacchetta_dx]);
        printf("Filosofo %d: ha preso la sua bacchetta di destra\n", id);
        
        printf("Filosofo %d: sta mangiando\n", id);
        usleep(1000000); //faccio "mangiare" il filosofo n per un po' di tempo
        
        pthread_mutex_unlock(&bacchette[bacchetta_dx]);
        printf("Filosofo %d: ha posato la sua bacchetta destra\n", id);
        pthread_mutex_unlock(&bacchette[bacchetta_sx]);
        printf("Filosofo %d: ha posato la sua bacchetta sinistra\n", id);
    }

return NULL;
}


int main() {
    pthread_t filosofi[5];
    int id[5];

    for (int i=0; i<5;i++){ //creo i thread
        id[i] = i;
        pthread_create(&filosofi[i],NULL,filosofo,&id[i]);
    }

    for (int i=0; i<5;i++){ //creo lock bacchette
        pthread_join(filosofi[i],NULL);
    }

    for (int i=0; i<10;i++){ //rilascio i lock
        pthread_mutex_destroy(&bacchette[i]);
    }

    return 0;
    }