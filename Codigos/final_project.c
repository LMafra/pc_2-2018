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

#define PLAYER_A 5
#define PLAYER_B 5
#define FAN 30

sem_t stadium;

void * team_a(void *arg);
void * team_b(void *arg);
void * stadium(void *arg);
void * hotdog(void *arg);





void * team_a(void * arg){
    int i = *((int *)a);
    while(TRUE){
        /* code */
    }

}