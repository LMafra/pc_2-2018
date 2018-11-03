/*************************************************/
//   Curso: Programação Concorrente - 2/2018     //
//   Professor: Eduardo Alchieri                 //
//   Aluno: Lucas Mafra Chagas                   //
//   Matrícula: 12/0126443                       //
/*************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include "unistd.h"

#define CHAIR 5
#define CUSTOMERS 10

sem_t chairs;
sem_t barbers_chair;
sem_t customers;
sem_t barbers;


void * barber(void *arg){
    int i=*((int*) arg);
    
    while(1){
        printf("Taking a nap \n");
        sem_wait(&customers);
        printf("Cutting hair of client %d \n", i);
        sleep(1);
        printf("Finished the haircut of client %d \n", i);
        sem_post(&barbers);
    }
    pthread_exit(0);
}

void * customer(void *arg){
    int i = *((int*) arg);
    while(1){
        if(sem_trywait(&chairs) == 0){
            printf("Client %d sit in a chair \n", i);
            sem_wait(&barbers_chair);
            sem_post(&chairs);
            sleep(1);
            sem_post(&customers);
            printf("Client %d is going to barber's chair \n", i);
            sem_wait(&barbers);
            sem_post(&barbers_chair);
            sleep(1);
        }else{
            sleep(1);
            printf("Client %d didn't find an empty chair \n", i);
            sleep(1);
        }
        pthread_exit(0);
    }

}

int main(){
    pthread_t c[CUSTOMERS+1];
    int i;
    int *person_id;

    sem_init(&barbers_chair, 0, 1);
    sem_init(&chairs, 0, CHAIR);
    sem_init(&customers, 0, 0);
    sem_init(&barbers, 0, 0);

    person_id = (int *) malloc(sizeof(int));
    *person_id = 0;
    pthread_create(&c[0], NULL, barber, (void *) (person_id));
    
    for(i = 1; i < CUSTOMERS; i++){
        person_id =  (int *) malloc(sizeof(int));
        * person_id = i;
        pthread_create(&c[1], NULL, customer, (void *) (person_id));
    }
    pthread_join(c[0], NULL);
    return 0;

}

