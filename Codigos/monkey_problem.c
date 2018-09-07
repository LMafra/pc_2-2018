/*************************************************/
//   Curso: Programação Concorrente - 2/2018     //
//   Professor: Eduardo Alchieri                 //
//   Aluno: Lucas Mafra Chagas                   //
//   Matrícula: 12/0126443                       //
/*************************************************/


#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "unistd.h"
#include "semaphore.h"


#define TRUE 1

#define MONKEY_A 5
#define MONKEY_B 5

pthread_mutex_t turn;
pthread_mutex_t db;
pthread_mutex_t mountain_b__mountain_a;
pthread_mutex_t mountain_a__mountain_b;

int process = 0;


void* mountain_a(void* arg);
void* mountain_b(void* arg);
void crossing_rope_a();
void arriving_mountain_b();
void crossing_rope_b();
void arriving_mountain_a();
void crossing_portal_b();
void crossing_portal_a();


int main()
{
    pthread_mutex_init (&mountain_b__mountain_a, NULL);
    pthread_mutex_init (&mountain_a__mountain_b, NULL);
    pthread_mutex_init (&turn, NULL);
    pthread_mutex_init (&db, NULL);

    pthread_t a[MONKEY_A], b[MONKEY_B];

    int i;
    int * id;

    for(i = 0; i < MONKEY_A; i++)
    {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&a[i], NULL, mountain_a, (void *) (id));
    }
    
    for(i = 0; i < MONKEY_B; i++)
    {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&b[i], NULL, mountain_b, (void *) (id));
    }

    pthread_join(a[0],NULL);
    return 0;

}

void* mountain_a(void* arg){
    int i = *((int *) arg);

    while(TRUE){
        
        pthread_mutex_lock(&turn);
        pthread_mutex_lock(&mountain_a__mountain_b);
        
        process = process + 1;

        if(process == 1)
            pthread_mutex_lock(&db);

        pthread_mutex_unlock(&turn);
        pthread_mutex_unlock(&mountain_a__mountain_b);

        crossing_rope_a(i);

        pthread_mutex_lock(&mountain_a__mountain_b);

        process = process - 1;

        if(process == 0)
            pthread_mutex_unlock(&db);

        pthread_mutex_unlock(&mountain_a__mountain_b);
        arriving_mountain_b(i);
        //crossing_portal_b(i);       
    }
    pthread_exit(0);
}

void* mountain_b(void* arg){
    int i = *((int *) arg);

    while(TRUE){
        
        pthread_mutex_lock(&turn);
        pthread_mutex_lock(&mountain_b__mountain_a);
        
        process = process + 1;

        if(process == 1)
            pthread_mutex_lock(&db);

        pthread_mutex_unlock(&turn);
        pthread_mutex_unlock(&mountain_b__mountain_a);

        crossing_rope_b(i);

        pthread_mutex_lock(&mountain_b__mountain_a);

        process = process - 1;

        if(process == 0)
            pthread_mutex_unlock(&db);

        pthread_mutex_unlock(&mountain_b__mountain_a);
        arriving_mountain_a(i);
        //crossing_portal_a(i);      
    }
    pthread_exit(0);
}

void crossing_rope_a(int i){
    printf("Monkey %d from mountain A is crossing the rope to mountain B. \n", i);
    sleep(rand()%3);
}

void crossing_rope_b(int i){
    printf("Monkey %d from mountain B is crossing the rope to mountain A. \n", i);
    sleep(rand()%3);
}

void arriving_mountain_a(int i){
    printf("Monkey %d from mountain B arrived at mountain A. \n", i);
    sleep(rand()%3);
}

void arriving_mountain_b(int i){
    printf("Monkey %d from mountain A arrived at mountain B. \n", i);
    sleep(rand()%3);
}

void crossing_portal_a(int i){
    printf("Monkey %d from mountain A is crossing the portal back to mountain A. \n", i);
    sleep(rand()%3);
}

void crossing_portal_b(int i){
    printf("Monkey %d from mountain B is crossing the portal back to mountain B. \n", i);
    sleep(rand()%3);
}