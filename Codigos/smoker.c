#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

sem_t table[2];
sem_t fala_chefia;


char* smoker_types[3] = { "matches & tobacco", "matches & paper", "tobacco & paper" };


void * smoker(void * arg);
void * chefia(void * arg);

void * smoker(void * arg){
    int i = *((int *)arg);
    
    while(1){
        
    printf(KMAG "Fumante [%d]: Quero %s \n", i, smoker_types[i]);
        sem_wait(&table[i]);
        
        
        printf(KGRN "Fumante [%d]: TÔ FUMANDO, PORRA!\n \a", i);
        sleep(3);

        printf(KBLU "Fumante [%d]: ACABEI! FALA CHEFIA, PODE COLOCAR MAIS \n", i);
        sem_post(&fala_chefia);
        
    }

}

void * chefia(void * arg){
    int i = *((int *)arg);
    int item;

    while(1){
        sem_wait(&fala_chefia);
        item = rand() % 3;

        printf(KRED "TÁ AÍ, MEU CONSAGRADO, PEGA ESSE %s \n", smoker_types[item]);

        sem_post(&table[item]);
    }    

}

int main(int argc, char const *argv[])
{
    int i;
    int *id;
        
    
    for( i = 0; i < 3; i++)
    {
        sem_init(&table[i], 0, 0);
    }
    
    sem_init(&fala_chefia, 0, 1);

    pthread_t smokers[2], consagrado;
    id = (int *)  malloc(sizeof(int));
    *id = i;
    pthread_create(&consagrado, NULL, chefia, (void * )(id));

    for (i =0; i< 3; i++)
    
    {
        id = (int *)  malloc(sizeof(int));
        *id = i;
        pthread_create(&smokers[i], NULL, smoker, (void * )(id));
    }
    pthread_join(smokers[0],NULL);
    return 0;
}
