#include "my_barrier.h"
#include <stdio.h>
#include <stdlib.h>


unsigned int pthread_my_barrier_init(my_barrier *mb, unsigned int v){
    if (v==0) return -1;
    mb->vinit = v;
    mb->val = 0;
    pthread_mutex_init(&mb->lock, NULL);
    pthread_cond_init(&mb->varcond, NULL);
    return 0;
}


unsigned int pthread_my_barrier_wait(my_barrier *mb){
    pthread_mutex_lock(&mb->lock);
    mb->val++;
    if (mb->val == mb->vinit){
        mb->val = 0;
        pthread_cond_broadcast(&mb->varcond);
    } else pthread_cond_wait(&mb->varcond, &mb->lock);
    pthread_mutex_unlock(&mb->lock);
    return 0;
}

int main (){
    my_barrier prova;
    if(pthread_my_barrier_init(&prova, 1) == -1){
        printf("Errore nell'inizializzazione della barriera\n");
        exit(1);
    }

    if(pthread_my_barrier_wait(&prova) == -1){
        printf("Errore nell'attesa della barriera\n");
        exit(1);
    }

    return 0;
}