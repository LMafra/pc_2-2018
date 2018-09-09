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

#define MONKEY_A 10
#define MONKEY_B 10

pthread_mutex_t turn;
pthread_mutex_t entry;
pthread_mutex_t mountain_b_to_mountain_a;
pthread_mutex_t mountain_a_to_mountain_b;

int mountainA = 0;
int mountainB = 0;


void* mountain_a(void* arg);
void* mountain_b(void* arg);
void crossing_rope_a();
void arriving_mountain_b();
void crossing_rope_b();
void arriving_mountain_a();
//void crossing_portal_b();
//void crossing_portal_a();


int main()
{
    pthread_mutex_init (&mountain_b_to_mountain_a, NULL);
    pthread_mutex_init (&mountain_a_to_mountain_b, NULL);
    pthread_mutex_init (&turn, NULL);
    pthread_mutex_init (&entry, NULL);

    pthread_t monkey[MONKEY_A + MONKEY_B];

    int i;
    int * id;

    for(i = 0; i < MONKEY_A + MONKEY_B; i++)
    {
        id = (int *) malloc(sizeof(int));
        *id = i;

        
        if (i%2 == 0) {
            
            if ( pthread_create(&monkey[i], NULL, mountain_a, (void *) (id))) {
                return -1;
            }
        }
        else if ( pthread_create(&monkey[i], NULL, mountain_b, (void *) (id)))
        {
            return -1;
        }
        id++;
    }
    

    
    for(i = 0; i < MONKEY_A + MONKEY_B; i++)
    {  
        if (pthread_join(monkey[i], NULL)) {
            return -1;
        }
    }
    
    return 0;

}

void* mountain_a(void* arg){
    int i = *((int *) arg);

    while(TRUE){
        
        pthread_mutex_lock(&turn);
        pthread_mutex_lock(&mountain_a_to_mountain_b);
        
        mountainA = mountainA + 1;

        if(mountainA == 1)
            pthread_mutex_lock(&entry);

        pthread_mutex_unlock(&mountain_a_to_mountain_b);
        pthread_mutex_unlock(&turn);
        crossing_rope_a(i);

        pthread_mutex_lock(&mountain_a_to_mountain_b);

        mountainA = mountainA - 1;

        if(mountainA == 0)
            pthread_mutex_unlock(&entry);

        pthread_mutex_unlock(&mountain_a_to_mountain_b);
        arriving_mountain_b(i);
        //crossing_portal_b(i);       
    }
    pthread_exit(0);
}

void* mountain_b(void* arg){
    int i = *((int *) arg);

    while(TRUE){
        
        pthread_mutex_lock(&turn);
        pthread_mutex_lock(&mountain_b_to_mountain_a);
        
        mountainB = mountainB + 1;

        if(mountainB == 1)
            pthread_mutex_lock(&entry);

        pthread_mutex_unlock(&mountain_b_to_mountain_a);
        pthread_mutex_unlock(&turn);
        crossing_rope_b(i);

        pthread_mutex_lock(&mountain_b_to_mountain_a);

        mountainB = mountainB - 1;

        if(mountainB == 0)
            pthread_mutex_unlock(&entry);

        pthread_mutex_unlock(&mountain_b_to_mountain_a);
        arriving_mountain_a(i);
        //crossing_portal_a(i);      
    }
    pthread_exit(0);
}

void crossing_rope_a(int i){
    printf("Monkey %d from mountain A is crossing the rope to mountain B. \n", i);
    sleep(rand()%5);
}

void crossing_rope_b(int i){
    printf("Monkey %d from mountain B is crossing the rope to mountain A. \n", i);
    sleep(rand()%5);
}

void arriving_mountain_a(int i){
    printf("Monkey %d from mountain B arrived at mountain A. \n", i);
    sleep(rand()%5);
}

void arriving_mountain_b(int i){
    printf("Monkey %d from mountain A arrived at mountain B. \n", i);
    sleep(rand()%5);
}

void crossing_portal_a(int i){
    printf("Monkey %d from mountain A is crossing the portal back to mountain A. \n", i);
    sleep(rand()%5);
}

void crossing_portal_b(int i){
    printf("Monkey %d from mountain B is crossing the portal back to mountain B. \n", i);
    sleep(rand()%5);
}