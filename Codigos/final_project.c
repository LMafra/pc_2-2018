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


#define TRUE 1
#define TEAM_A 5
#define TEAM_B 5
//#define FAN 30

//sem_t gate;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;;
pthread_cond_t gamestart = PTHREAD_COND_INITIALIZER;;
pthread_cond_t bus = PTHREAD_COND_INITIALIZER;;


void * team_a(void *arg);
void * team_b(void *arg);
//void * stadium(void *arg);
//void * fan(void * arg);
//void * hotdog(void *arg);
void * game(void *arg);

int teamA = 0;
int teamB = 0;


int main()
{

    pthread_t a[TEAM_A];
    pthread_t b[TEAM_B];
    pthread_t g;
    
    int i;
    int *id;

    pthread_create(&g, NULL, game, NULL);

    
    for(i = 0; i < TEAM_A; i++)
    {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&a[i], NULL, team_a, NULL);
    }

    
    for(i = 0; i < TEAM_B; i++)
    {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&b[i], NULL, team_b, NULL);
    }

    pthread_join(a[0], NULL);
    return 0;
}



void * team_a(void *a){
    int i = *((int *)a);
    while(TRUE){

        pthread_mutex_lock(&mutex);
        while(teamA == TEAM_A){
            pthread_cond_wait(&bus, &mutex);
        }
        teamA++;

        printf("Chegou jogador %d do Time A \n", i);
        if (teamA == TEAM_A) {
            pthread_cond_signal(&gamestart);
        }
        pthread_mutex_unlock(&mutex);
    }

}

void * team_b(void *b){
    int i = *((int *)b);
    while(TRUE){

        pthread_mutex_lock(&mutex);
        while(teamB == TEAM_B){
            pthread_cond_wait(&bus, &mutex);
        }
        teamB++;

        printf("Chegou jogador %d do Time A \n", i);
        if (teamB == TEAM_B) {
            pthread_cond_signal(&gamestart);
        }
        //pthread_cond_broadcast(&bus);
        pthread_mutex_unlock(&mutex);
    }
    
}

void * game (void *arg){
    while(TRUE){
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&bus,&mutex);
        printf("COMEÇA A PARTIDA \n");
        sleep(90);
        printf("ACABA A PARTIDA \n");
        pthread_mutex_unlock(&mutex);
    }   
}