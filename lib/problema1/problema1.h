/*PROYECTO SISTEMAS OPERATIVOS. PROBLEMA 1*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h> 

#include "../config/problema1_config.h"

#ifndef _PROBLEMA1_H

#define _PROBLEMA1_H


/*------ PROCEDIMIENTOS PARA LOS PROCESOS ------*/


sem_t sem_cola_lectores, sem_lector, sem_contador_lectores;
sem_t sem_contador_escritores, sem_escritor; 
int contador_lectores; 
int contador_escritores; 
int contador_administradores;

//procedimiento para procesos lectores
void *lectores(void *args)
{
	sem_wait(&sem_cola_lectores); 
		sem_wait(&sem_lector); 
			sem_wait(&sem_contador_lectores); 
				contador_lectores ++; 
				if(contador_lectores == 1)
				{
					sem_wait(&sem_escritor); 
				}
			sem_post(&sem_contador_lectores); 
		sem_post(&sem_lector); 
	sem_post(&sem_cola_lectores); 
	printf("leyendo datos...\n"); 
	sem_wait(&sem_contador_lectores);
		contador_lectores --; 
		if(contador_lectores == 0)
		{
			sem_post(&sem_escritor); 
		}
	sem_post(&sem_contador_lectores);
}

//procedimiento para procesos escritores

void *escritores(void *args)
{
	sem_wait(&sem_contador_escritores); 
		contador_escritores ++; 
		if(contador_escritores == 1)
		{
			sem_wait(&sem_lector); 
		}
	sem_post(&sem_contador_escritores);
	sem_wait(&sem_escritor); 
		printf("escribiendo...\n"); 
	sem_post(&sem_escritor); 
		sem_wait(&sem_contador_escritores); 
		contador_escritores --; 
		if(contador_escritores == 0)
		{
			sem_post(&sem_lector); 
		}
	sem_post(&sem_contador_escritores);
}

//proceso administrador

void *administrador(void *args)
{
	sem_wait(&sem_lector);
	sem_wait(&sem_escritor); 
		printf("administrando...\n"); 
	sem_post(&sem_escritor); 
	sem_post(&sem_lector);
}

/*CODIGO PRINCIPAL*/

void problema2() {

    srand(time(NULL));
	pthread_t hilo_L_E[CANTIDAD_THREAD];
	int tipo_proceso; 
		sem_init(&sem_cola_lectores, 0, 1);
		sem_init(&sem_lector, 0, 1);
		sem_init(&sem_contador_lectores, 0, 1);
		sem_init(&sem_contador_escritores, 0, 1);
		sem_init(&sem_escritor, 0, 1);
		contador_lectores = 0; 
		contador_escritores = 0; 
		contador_administradores = 0; 
		for(int i = 0; i < CANTIDAD_THREAD; i ++)
		{
			tipo_proceso = ((i+1) + rand())%3; 
			if(tipo_proceso == 1)
			{
				pthread_create(&hilo_L_E[i], NULL, *lectores, NULL); 
				contador_lectores ++; 
			}else if(tipo_proceso == 2)
			{
				pthread_create(&hilo_L_E[i], NULL, *escritores, NULL); 
				contador_escritores ++; 
			}else{
				pthread_create(&hilo_L_E[i], NULL, *administrador, NULL); 
				contador_administradores ++; 
			}
		}
		for(int i = 0; i < CANTIDAD_THREAD; i ++)
		{
			pthread_join(hilo_L_E[i], NULL); 
		} 
		
		printf("total procesos lectores: %d\n", contador_lectores); 
		printf("total procesos escritores: %d\n", contador_escritores);
		printf("total procesos administradores: %d\n", contador_administradores); 
}





//NOTA: FALTA UN CICLO PARA LOS DÍAS, Y SOLO FALTA FALTA LAS FUNCIONES PARA ESCRIBIR EN UN ARCHIVO DE TEXTO, LA ESTRUCTURA YA ESTÁ HECHA.
// LA CANTIDAD DE PROCESOS SE CONFIGURAN POR CONSTANTE, EN UN RATO SUBO LA NUEVA VERSIÓN :v 

#endif