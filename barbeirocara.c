#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<semaphore.h>

#define Tamanho_Fila 5
#define Numero_Cabines 2

sem_t Novo_Carro; // Representa que um novo Carro chegou
sem_t Filas[Tamanho_Fila]; // Representa as vagas disponiveis no pedagio
sem_t Cabines[Numero_Cabines]; // Representa a quantidade de cabines
sem_t Sinal[Numero_Cabines]; // Representa se uma Cabine esta liberada para receber um novo carro

int i,N,Numero_da_Cabine, Carros_Rodovia=0, N_Cabine[Numero_Cabines]; // variaveis de controle

pthread_t Cabine,Carro ; //trheads das cabines e dos carros

void * cabine(void * N);
void * carro();
void * carro_na_cabine(int Numero_da_Cabine);

void * cabine(void * N){
	while(1){
		Numero_da_Cabine = * (int *) N; // Passa numero da cabine para inteiro e recebe o parametro N
		for (i=0;i<Tamanho_Fila;i++){ // pecorre tamanho da fila,   video ta < mas acho que é <=
			if  (sem_trywait(&Filas[i]) == 0){ // verifica se a fila está vazia, 
				printf(" Pedágio Vazio\n");
				sem_wait(&Novo_Carro); // Esperando novo Carro
				sem_post(&Sinal[Numero_da_Cabine]); // Sinal de Cabine livre
				sem_wait(&Cabines[Numero_da_Cabine]);// Libera a Cabine
			}
			else{
				sem_post(&Novo_Carro); // libera um Carro da fila
				sem_post(&Filas[i]); // Libera um posicão da fila para chegada de um Novo carro	
				sem_wait(&Sinal[Numero_da_Cabine]); // Avisa que a está Cabine Ocupada
				sem_wait(&Cabines[Numero_da_Cabine]); // Bloqueia Cabine
				carro_na_cabine(Numero_da_Cabine);
			}
		}
	}
}

void * carro(){
	for(i=0;i<=Tamanho_Fila;i++){
		if (sem_trywait(&Filas[i]) != 0){ // verifica se existe posições vazias na fila
			for (N=0;N<=Numero_Cabines;N++){ // peccore as cabines
				if (sem_trywait(&Sinal[N]) == 0){ // Verifica se todas as cabines está cheia
  					sem_wait(&Filas[i]); // Aguarda na fila
				}
				else{ // Verifica alguma Cabine Vazia
					printf("Cabine %d Livre.\n", N +1);
					sem_post(&Novo_Carro); // Libera um novo carro
					sem_post(&Filas[i]); // Libera um posição na fila
					sem_wait(&Cabines[N]); // bloqueia a cabine
					sem_wait(&Sinal[N]); // Avisa que cabine está ocupada	
				}			
			}		
		}
		else{
			Carros_Rodovia = Carros_Rodovia+1;
			if (Carros_Rodovia == Tamanho_Fila){ // video ta == mas acho que é <=
				printf("Pedagio Lotado.\n");			
			}	
		}
	}
	Carros_Rodovia =0;
}

void * carro_na_cabine(int Numero_da_Cabine){
	printf("Carro na %d Cabine.\n",Numero_da_Cabine+1);
}

int main(){
	//inicialização dos semaforos
	sem_init(&Novo_Carro,1,0);
	for (i=0;i<Numero_Cabines;i++){
		sem_init(&Filas[i],1,0);
		sem_init(&Sinal[i],1,0);
	}
	// Criação das Threads
	for (i=0; i< Numero_Cabines;i++){
		N_Cabine[i]=i;
		pthread_create(&Cabine,NULL,(void *)cabine,&N_Cabine[i]);
	}
	// Criação de Carros					
	while(1){
		pthread_create(&Carro,NULL,(void *)carro,NULL);
	}
	return 0;
}