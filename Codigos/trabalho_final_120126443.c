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


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define TRUE 1
#define TEAM_A 11
#define TEAM_B 11
#define FAN 40
#define STADIUM_SEAT 20
#define HOTDOGS 10


sem_t seat;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gameStart = PTHREAD_COND_INITIALIZER;
pthread_cond_t gameEnd = PTHREAD_COND_INITIALIZER;
pthread_cond_t bus = PTHREAD_COND_INITIALIZER;
pthread_cond_t hotdogger = PTHREAD_COND_INITIALIZER;
pthread_cond_t buyers = PTHREAD_COND_INITIALIZER;


void * team_a(void *arg);
void * team_b(void *arg);
void * fan(void * arg);
void * hotdog(void *hd);
void * game(void *arg);
void team_a_arrived(int teamA);
void team_b_arrived(int teamB);
void team_a_left();
void team_b_left();
void game_start();
void game_end();
void cooking_hotdog();
void fan_enter(int i);
void fan_exit(int i);
void stadium_full(int i);
void waiting_hotdog(int i);
void hotdog_over(int i);
void buyind_hotdog(int i, int hotdogs);
void cooking_waiting(int hotdogs);


int teamA = 0;
int teamB = 0;
int hotdogs = 0;


int main()
{

    pthread_t player[TEAM_A + TEAM_B];
    pthread_t g;
    pthread_t f[FAN];
    pthread_t h;


    
    int i;
    int *id;


    sem_init(&seat, 0, STADIUM_SEAT);
    pthread_create(&g, NULL, game, NULL);
    pthread_create(&h, NULL, hotdog, NULL);


    for(i = 0; i < FAN; i++)
    {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&f[i], NULL, fan, (void *) (id));
    }

    
    for(i = 0; i < TEAM_A; i++)
    {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&player[i], NULL, team_a, (void *) (id));
    }

    
    for(i = 0; i < TEAM_B; i++)
    {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&player[i], NULL, team_b, (void *) (id));
    }

    
    for( i = 0; i < TEAM_A + TEAM_B; i++)
    {
        if(pthread_join(player[i], NULL)){
            return -1;
        }
    }
    return 0;
}



void * game (void *arg){
    while(TRUE){
        sleep(1);
        pthread_mutex_lock(&lock);
        
        
        pthread_cond_wait(&gameStart,&lock);
        if (teamA == TEAM_A && teamB == TEAM_B) {
            sleep(2);
            game_start();
            pthread_mutex_unlock(&lock);

            sleep(2);
        
            pthread_mutex_lock(&lock);
            
            sleep(5);
        
            game_end();
        
            pthread_cond_broadcast(&gameEnd);
        }
        pthread_mutex_unlock(&lock);
    }
}


void * team_a(void *player){
    int i = *((int *)player);
    while(TRUE){

        sleep(rand()%(i+5));
        pthread_mutex_lock(&lock);

        while(teamA == TEAM_A){ 
            pthread_cond_wait(&bus, &lock);
        }

        if(teamA != TEAM_A){
            teamA++;
            team_a_arrived(teamA);
        }

        pthread_cond_broadcast(&bus);
        
        
        pthread_mutex_unlock(&lock);
        //sleep(1);
        
        pthread_mutex_lock(&lock);
        
        if (teamA == TEAM_A) { //Enquanto o time não estiver completo, não avisa que vai começar a partida
            pthread_cond_signal(&gameStart);
        }

        pthread_mutex_unlock(&lock);

        sleep(2);

        pthread_mutex_lock(&lock);
        
        pthread_cond_wait(&gameEnd, &lock);


        if(teamA != 0){
            teamA = 0;
            team_a_left();
        }
        pthread_mutex_unlock(&lock);
        pthread_exit(0);
    }
}

void * team_b(void *player){
    int i = *((int *)player);
    while(TRUE){
        sleep(rand()%(i+5));
        pthread_mutex_lock(&lock);
        
        while(teamB == TEAM_B){
            pthread_cond_wait(&bus, &lock);
        }

        if(teamB != TEAM_B){
            teamB++;
            team_b_arrived(teamB);
        }

        pthread_cond_broadcast(&bus);

        pthread_mutex_unlock(&lock);
        sleep(1);
        pthread_mutex_lock(&lock);
        
        if (teamB == TEAM_B) { //Enquanto o time não estiver completo, não avisa que vai começar a partida
            pthread_cond_signal(&gameStart);
        }

        pthread_mutex_unlock(&lock);

        sleep(2);

        pthread_mutex_lock(&lock);
        
        pthread_cond_wait(&gameEnd, &lock);
        if(teamB != 0){
            teamB = 0;
            team_b_left();
        }
        pthread_mutex_unlock(&lock);
        pthread_exit(0);
    }  
}


void * fan(void *f){
    int i = *((int *)f);
    
    sleep(rand()%(i+10));
    if (sem_trywait(&seat) == 0) { // Torcedores entrando no estádio

        fan_enter(i);
        
        sleep(rand()%(i+10));
        while(!hotdogs){ //Todo torcedor que entra, compra um dogão. Se não tiver, espera ter dogão
            waiting_hotdog(i);
            pthread_cond_wait(&buyers, &lock);
        }
        hotdogs--;
        buyind_hotdog(i, hotdogs);
        if(hotdogs == 0){ //Avisa que o dogão acabou
            hotdog_over(i);
            pthread_cond_signal(&hotdogger);
        }

        pthread_cond_wait(&gameEnd, &lock);
        sem_post(&seat);
        fan_exit(i);
        pthread_exit(0);
    }else{ //Caso não tenha mais lugares, eles vão embora
        //sleep(1);
        stadium_full(i);
        pthread_exit(0);
    }
    
}

void * hotdog(void *arg){
    while(TRUE){
        sleep(1);
        pthread_mutex_lock(&lock);
        while (hotdogs != 0) { //Se tiver dogão, fica de boa
            cooking_waiting(hotdogs);
            pthread_cond_wait(&hotdogger, &lock);    
        }
        //sleep(2);
        cooking_hotdog(); //Cozinha os dogão
        //sleep(2);
        hotdogs += HOTDOGS;
        pthread_cond_broadcast(&buyers);   //Avisa que tem dogão 
        pthread_mutex_unlock(&lock); 
    }
}

void team_a_arrived(int teamA){
    printf(KBLU "Chegou o %dº jogador do Time A \n", teamA);
}

void team_a_left(){
    printf(KBLU "Time A saiu do estádio \n");
}

void team_b_left(){
    printf(KWHT "Time B saiu do estádio \n");
}

void team_b_arrived(int teamB){
    printf(KWHT "Chegou o %dº jogador do Time B \n", teamB);
}

void game_start(){
    printf(KRED "COMEÇA A PARTIDA \n");
}

void game_end(){
    printf(KRED"ACABA A PARTIDA \n");
}

void fan_enter(int i){
    printf(KCYN "Torcedor %d entrou no estádio \n", i);
}

void fan_exit(int i){
    printf( KCYN "Torcedor %d foi embora do estádio \n", i);
}

void stadium_full(int i){
    printf(KYEL "Torcedor %d tentou entrar no estádio, mas já tá lotado \n", i);
}

void waiting_hotdog(int i){
    printf(KMAG "Torcedor %d ficou esperando o cachorro-quente \n", i);
}

void buyind_hotdog(int i, int hotdogs){
    printf(KMAG "Torcedor %d comprou um cachorro-quente, agora tem %d cachorro-quente \n", i, hotdogs);
}

void cooking_hotdog(){
    printf(KGRN "Tio do cachorro-quente foi cozinhar\n");
}

void cooking_waiting(int hotdogs){
    printf(KGRN "Vou aguardar, tem %d cachorro-quente\n", hotdogs);
}

void hotdog_over(int i){
    printf(KMAG "Torcedor %d avisou o tio do cachorro-quente que acabou \n", i);
}