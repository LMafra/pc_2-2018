#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define MATRIX 40

typedef int semaphore;
semaphore mutex = 1;
semaphore empty = MATRIX;
semaphore full = 0;

pthread_mutex_t mutex_matrix;
pthread_t threads_matrix[MATRIX];
pthread_barrier_t barrier;

sem_t free_pos;
sem_t filled_pos;

int matriz[MATRIX][MATRIX];
int parcial[MATRIX];

void showMatrix(){
    int i,j;
    printf("Matriz: \n");
    for(i=0;i<MATRIX;i++){
        for(j=0;j<MATRIX;j++){
            printf(" %d ",matriz[i][j]);
        }
        printf("\n");
    }
}

void showPartial(){
    int i;
    printf("Vetor: ");
    for(i=0;i<MATRIX;i++){
        printf(" %d ",parcial[i]);        
    }
    printf("\n");
}

void showSum() {
    int i,sum;
    for(i=0;i<MATRIX;i++){
        sum += parcial[i];
    }
    printf("Soma na matriz: %d \n",sum);
}

void * calcular(void * arg){
    int id = *((int*)arg);
    int i,sum=0;

    for(i=0;i<MATRIX;i++){
        sum += matriz[id][i];
    }

    parcial[id] = sum;

    pthread_barrier_wait(&barrier);

    if(id == 0){
        showPartial();
        showSum();
    }
}

int main(){

    pthread_barrier_init(&barrier, NULL, MATRIX);
    pthread_t threads_matrix[MATRIX];

    int *id;
    int i,j;

    pthread_mutex_init(&mutex_matrix,NULL);

    for(i=0;i<MATRIX;i++){
        for(j=0;j<MATRIX;j++){
            matriz[i][j] = 1;
        }
    }

    for (i = 0; i < MATRIX; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&threads_matrix[i], NULL, calcular, (void *)(id));
    }

    for(i = 0; i < MATRIX; i++)
    {
        if(pthread_join(threads_matrix[i], NULL))
        {
            printf("Could not join thread %d\n", i);
            return -1;
        }
    }

    return 0;
}