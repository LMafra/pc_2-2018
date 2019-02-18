// [Estudo dirigido] Questão 2 da prova utilizando barreiras
// YAN VICTOR DOS SANTOS
// PROGRAMAÇÃO CONCORRENTE - 1/2018
// CÓDIGO FEITO POR MIM! E
// A lógica foi desenvolvida juntamente com a aluna Dayanne (aluna também da disciplina)

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"
#include "string.h"


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define QTD_A_C 5
#define QTD_B_D 14

pthread_barrier_t barreira;
pthread_mutex_t lock;
int flag=0;
void * mostra_A_C(void *arg){
	int i = *(int *)(arg);

		printf(KRED "A");
		pthread_barrier_wait(&barreira); //liberar para B

		pthread_barrier_wait(&barreira); //printando B
		printf(KBLU "C");
		pthread_barrier_wait(&barreira); // libera D
		
	
}

void * mostra_B_D(void *arg){
	int i = *(int *)(arg);

		pthread_barrier_wait(&barreira); //printando A
		printf(KYEL "B");
		pthread_barrier_wait(&barreira); // liberar para C
		
		pthread_barrier_wait(&barreira); 
		printf(KGRN "D");
		pthread_barrier_wait(&barreira);

}

int main(){
	pthread_t a[QTD_A_C + QTD_B_D];
	int i;
	int *id_thread;	
	printf("\n============================= INICIO =============================\n");

	pthread_barrier_init(&barreira, NULL, QTD_A_C + QTD_B_D);
	
	for(i=0; i<QTD_A_C; i++){
		id_thread = (int *) malloc(sizeof(int));
		*id_thread = i;
		pthread_create(&a[i], NULL, mostra_A_C, (void *) (id_thread));
	}
	for(i=0; i<QTD_B_D; i++){
		*id_thread = QTD_A_C+i;
		pthread_create(&a[QTD_A_C+i], NULL, mostra_B_D, (void *) (id_thread));
	}
	pthread_join(a[0],NULL);
	printf("\n");

	return 0;
}
