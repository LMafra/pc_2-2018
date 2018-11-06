#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define NP 3
#define NA 5
#define NF 4

#define VAGAS 5

void * professor(void * meuid);
void * aluno (void * meuid);
void * funcionario (void * meuid);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t prof_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t func_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t alu_cond = PTHREAD_COND_INITIALIZER;

int ocupadas = 0;
int prof = 0;
int alu = 0;
int func = 0;


void main(argc, argv)
int argc;
char *argv[];
{

  int erro;
  int i, n, m;
  int *id;

  pthread_t tP[NP];
  for (i = 0; i < NP; i++)
  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tP[i], NULL, professor, (void *) (id));

    if(erro)
    {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }


  pthread_t tA[NA];
  for (i = 0; i < NA; i++)
  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tA[i], NULL, aluno, (void *) (id));

    if(erro)
    {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }
  pthread_t tF[NF];
  for (i = 0; i < NF; i++){
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tF[i], NULL, funcionario, (void *) (id));
    if(erro)
    {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }
  pthread_join(tP[0],NULL);
}	

void * professor (void* pi){
  while(1){
    //sleep(5);
    pthread_mutex_lock(&mutex);
    printf("professor %d: vou estacionar \n", *(int *)(pi));
        
    prof++;
    while(ocupadas == VAGAS) {
      //printf("professor %d: vou aguardar uma vaga \n", *(int *)(pi));
      pthread_cond_wait(&prof_cond,&mutex);
    }
    prof--;
    ocupadas++;
    printf("professor %d: estacionei, num vagas ocupadas = %d, prof = %d, func = %d, alu = %d\n", *(int *)(pi),ocupadas,prof,func,alu);
    pthread_mutex_unlock(&mutex);

    sleep(10); 
  
    pthread_mutex_lock(&mutex);
    ocupadas--; 
    printf("professor %d: sai, num vagas ocupadas = %d\n", *(int *)(pi),ocupadas);

    pthread_cond_signal(&prof_cond);
    pthread_cond_signal(&func_cond);
    pthread_cond_signal(&alu_cond);
        

    pthread_mutex_unlock(&mutex);
    sleep(5);
  }
  
}


void * funcionario (void* pi){
  while(1){
    //sleep(5);
    pthread_mutex_lock(&mutex);
    printf("funcionario %d: vou estacionar \n", *(int *)(pi));
    func++;
    while(ocupadas == VAGAS || prof > 0) {
      //printf("funcionario %d: vou aguardar uma vaga \n", *(int *)(pi));
      pthread_cond_wait(&func_cond,&mutex);
    }
    func--;
    ocupadas++;
    printf("funcionario %d: estacionei, num vagas ocupadas = %d, prof = %d, func = %d, alu = %d\n", *(int *)(pi),ocupadas,prof,func,alu);
    pthread_mutex_unlock(&mutex);

    sleep(10); 

    pthread_mutex_lock(&mutex);  
    ocupadas--; 
    printf("funcionario %d: sai, num vagas ocupadas = %d\n", *(int *)(pi),ocupadas);

    pthread_cond_signal(&prof_cond);
    pthread_cond_signal(&func_cond);
    pthread_cond_signal(&alu_cond);

    pthread_mutex_unlock(&mutex);
    sleep(5);
  }
  
}


void * aluno (void* pi){
  while(1){
    //sleep(5);
    pthread_mutex_lock(&mutex);
    printf("aluno %d: vou estacionar \n", *(int *)(pi));
     
    alu++;
    while(ocupadas == VAGAS || prof > 0 || func > 0) {
      //printf("aluno %d: vou aguardar uma vaga \n", *(int *)(pi));
      pthread_cond_wait(&alu_cond,&mutex);
    }
    alu--;
    ocupadas++;
    printf("aluno %d: estacionei, num vagas ocupadas = %d, prof = %d, func = %d, alu = %d\n", *(int *)(pi),ocupadas,prof,func,alu);
    pthread_mutex_unlock(&mutex);

    sleep(10); 
  
    pthread_mutex_lock(&mutex);
    ocupadas--; 
    printf("aluno %d: sai, num vagas ocupadas = %d\n", *(int *)(pi),ocupadas);


    pthread_cond_signal(&prof_cond);
    pthread_cond_signal(&func_cond);
    pthread_cond_signal(&alu_cond);
    
    pthread_mutex_unlock(&mutex);
    sleep(5);
  }
  
}
