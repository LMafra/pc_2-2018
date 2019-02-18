
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include "unistd.h"

#define QTD_A_B 5

sem_t semaforo;

void mostra_A_B(void *arg);


int main(){
	pthread_t a[QTD_A_B];
	int i;
	int *id_thread;	
	printf("\n============================= INICIO =============================\n");

	sem_init(&semaforo, 0 , QTD_A_B);
	
	for(i=0; i<QTD_A_B; i++){
		id_thread = (int *) malloc(sizeof(int));
		*id_thread = i;
		pthread_create(&a[i], NULL, mostra_A_B, (void *)(id_thread));
	}

	pthread_join(a[0],NULL);
	printf("\n");

	return 0;
}

void mostra_A_B(void *arg){
    int i = *(int *)(arg);
    
    int y = 0;

    y++;

    printf("A");

    if (y == QTD_A_B) {
        sem_post(&semaforo);
    }

    sem_wait(&semaforo);

    printf("B"); 

    y--;
       
    
}