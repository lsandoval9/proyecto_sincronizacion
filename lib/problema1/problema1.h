/*PROYECTO SISTEMAS OPERATIVOS. PROBLEMA 1*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>

#include "../config/problema1_config.h"
#include "../utilities/file_utility.h"
#include "procesos_problema1.h"

#ifndef PROBLEMA1_H

#define PROBLEMA1_H


typedef struct InfoProceso
{
    int id;
    int tipo;
} InfoProceso;

/*------ PROCEDIMIENTOS PARA LOS PROCESOS ------*/

sem_t sem_cola_lectores, sem_lector, sem_contador_lectores;
sem_t sem_contador_escritores, sem_escritor;
int contador_lectores;
int contador_escritores;
int contador_administradores;
bool inicializado = false;



/*CODIGO PRINCIPAL*/

void iniciarProblema1()
{
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
    for (int i = 0; i < CANTIDAD_THREAD; i++)
    {
        tipo_proceso = ((i + 1) + rand()) % 3;
        if (tipo_proceso == 1)
        {
            pthread_create(&hilo_L_E[i], NULL, *lectores, NULL);
            contador_lectores++;
        }
        else if (tipo_proceso == 2)
        {
            pthread_create(&hilo_L_E[i], NULL, *escritores, NULL);
            contador_escritores++;
        }
        else
        {
            pthread_create(&hilo_L_E[i], NULL, *administrador, NULL);
            contador_administradores++;
        }
    }

    printf("total procesos lectores: %d\n", contador_lectores);
    printf("total procesos escritores: %d\n", contador_escritores);
    printf("total procesos administradores: %d\n", contador_administradores);

    inicializado = true;
    while (true) {}
}

// NOTA: FALTA UN CICLO PARA LOS DÍAS, Y SOLO FALTA FALTA LAS FUNCIONES PARA ESCRIBIR EN UN ARCHIVO DE TEXTO, LA ESTRUCTURA YA ESTÁ HECHA.
//  LA CANTIDAD DE PROCESOS SE CONFIGURAN POR CONSTANTE, EN UN RATO SUBO LA NUEVA VERSIÓN :v

#endif