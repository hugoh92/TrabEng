#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<semaphore.h>

#define Tamanho_Fila 10
#define Numero_Cabines 5
#define Tipos_Veiculos 7

sem_t sem_Veiculos; //representa os veiculos
sem_t sem_Fila; //representa a fila de veiculos
sem_t sem_Cabine[Numero_Cabines]; // representa as cabines do pedagio
sem_t sem_Cabine_check[Numero_Cabines]; // represta se carro passou ou nao
sem_t sem_Veiculos_Fila[Numero_Cabines]; //representa o num de carros na fila

sem_t sem_escreve, sem_ler;
int visor, amb=0, viat=0;

void* f_cabine(void * v){
	int id = *(int*)v;

	while(1) {
		sem_wait(&sem_escreve);  // espera veiculo
		visor = id;  //mostra que está livre
		sem_post(&sem_ler); //avisa que esta acordado
		sem_wait(&sem_Veiculos_Fila[id]); //espera veiculo passar
		if (Tipos_Veiculos == 1){
			usleep(random()1000*1); //passa na cabine
		}else if (Tipos_Veiculos==2){
			usleep(random()1000*2);
		}
		else if (Tipos_Veiculos==3){
			

		}
		// aqui tem que ter um if (Tipos_Veiculos == 1){} de acordo com o tipo de veiculo o tmepo muda
		usleep(random()1000*1); //passa na cabine
		sempost($sem_Cabine_check[id]) //avisa que carro passou
	}

}