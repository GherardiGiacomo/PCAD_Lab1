#include<my_barrier.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

my_barrier barrier;

 void *thread_func(void *arg) {
    int tid = *(int *)arg;
    printf("Thread %d: before barrier\n", tid);
    pthread_my_barrier_wait(&barrier);
    printf("Thread %d: after barrier\n", tid);
    pthread_exit(NULL);
}

unsigned int pthread_my_barrier_init(my_barrier *mb, unsigned int v){
    if(v=0) return -1;
    mb->vinit = v;
    
    pthread_mutex_init(&mb->lock, NULL);
    pthread_cond_init(&mb->varcond, NULL);
    
    return 0;
}


unsigned int pthread_my_barrier_wait(my_barrier *mb){
     pthread_mutex_lock(&mb->lock);
    mb->val++;

    if (mb->val >= mb->vinit) {
        mb->val = 0;
        pthread_cond_broadcast(&mb->varcond);
    } else {
        pthread_cond_wait(&mb->varcond, &mb->lock);
    }

    pthread_mutex_unlock(&mb->lock);

    return 0;
}

int main(){
pthread_t threads[10];
    int thread_ids[10];

    unsigned int v = 10;  // Numero di thread che devono aspettare alla barriera
    pthread_my_barrier_init(&barrier, v);

    for (int i = 0; i < 10; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}