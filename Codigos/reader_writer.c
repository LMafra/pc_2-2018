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

#define NW 9
#define NR 18


pthread_mutex_t turn;
pthread_mutex_t mutex;
pthread_mutex_t db;
int process = 0;


void* reader(void *arg);
void* writer(void *arg);
void reading_reader();
void using_reader();
void thinking_writer();
void writing_writer();

int main()
{
    pthread_mutex_init (&mutex, NULL);
    pthread_mutex_init (&turn, NULL);
    pthread_mutex_init (&db, NULL);

    pthread_t r[NR], w[NW];

    int i;
    int * id;

    for(i = 0; i < NR; i++)
    {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&r[i], NULL, reader, (void *) (id));
    }
    
    for(i = 0; i < NW; i++)
    {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&w[i], NULL, writer, (void *) (id));
    }

    pthread_join(r[0],NULL);
    return 0;
}

void* reader(void *arg){
    int i = *((int *) arg);
    
    while(TRUE){
        
        pthread_mutex_lock(&turn);
        pthread_mutex_lock(&mutex);
        
        process = process + 1;

        if(process == 1)
            pthread_mutex_lock(&db);

        pthread_mutex_unlock(&turn);
        pthread_mutex_unlock(&mutex);

        reading_reader(i);

        pthread_mutex_lock(&mutex);

        process = process -1;

        if(process == 0)
            pthread_mutex_unlock(&db);

        pthread_mutex_unlock(&mutex);
        using_reader(i);       
    }
    pthread_exit(0);


}

void* writer(void *arg){
    int i = *((int *) arg);

    while(TRUE){
        thinking_writer(i);
        pthread_mutex_lock(&turn);
        pthread_mutex_lock(&db);
        writing_writer(i);
        pthread_mutex_unlock(&db);
        pthread_mutex_unlock(&turn);
    }
    pthread_exit(0);
}



void reading_reader(int i){
    printf("Reader %d is reading the text. \n", i);
    sleep(rand()%3);
}

void using_reader(int i){
    printf("Reader %d is using the text.\n", i);
    sleep(rand()%3);
}

void thinking_writer(int i){
    printf("Writer %d is thinking about writing the text.\n", i);
    sleep(rand()%3);
}

void writing_writer(int i){
    printf("Writer %d is writing the text.\n", i);
    sleep(rand()%3);
}


