#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"
#include "semaphore.h"

#define TRUE 1

#define NU 10 
#define CARTAS 5 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pombo_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t usuario_cond = PTHREAD_COND_INITIALIZER;
                   
int contador = 0;                     

void * pombo(void *arg);
void * usuario(void *arg);

int main() {
	pthread_t r[NU];
	pthread_t p;

	int i;
        int *id;
        pthread_create(&p, NULL, pombo, NULL);    
    	for (i = 0; i < NU ; i++) {
          id = (int *) malloc(sizeof(int));
          *id = i;
	  pthread_create(&r[i], NULL, usuario,  (void *) (id));
	}

	

	pthread_join(p, NULL);
        
	return 0;
}

void* usuario(void *a) {
	int i = *((int *) a);
//        printf("usuario\n");
	while(TRUE) {
                 printf("Usuário %d vai escrever uma carta.\n",i);               
		 sleep(5);
                 pthread_mutex_lock(&mutex);
                 while(contador == CARTAS){
                        pthread_cond_wait(&usuario_cond,&mutex);
                 }

                 contador++;
		 printf("Usuário %d colocou uma carta na mochila do pombo.\n",i);
		 sleep(1);

		 if (contador == CARTAS) {        
		     pthread_cond_signal(&pombo_cond);   
		 }
                 pthread_mutex_unlock(&mutex);
		 
	}
}

void* pombo(void *arg) {
	
	while(TRUE) {

                 pthread_mutex_lock(&mutex);
		 //while(contador < CARTAS){
			pthread_cond_wait(&pombo_cond,&mutex);
                 
                 //}

	         printf("Pombo levando as cartas.\n");
                 sleep(10);
		 contador = 0;
                 printf("Pombo voltou.\n");
                 pthread_cond_broadcast(&usuario_cond);   
		 pthread_mutex_unlock(&mutex);
	}
}
