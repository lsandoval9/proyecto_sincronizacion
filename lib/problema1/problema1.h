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



// Definir las variables globales
sem_t sem_lectura;                     // Semáforo para controlar el acceso de los lectores
sem_t sem_escritura;                   // Semáforo para controlar el acceso de los escritores
sem_t sem_administracion;              // Semáforo para controlar el acceso de los administradores
int lectores;                          // Contador de lectores activos
int escritores;                        // Contador de escritores activos
int administradores;                   // Contador de administradores activos
pthread_mutex_t mutex_lectores;        // Mutex para proteger el contador de lectores
pthread_mutex_t mutex_escritores;      // Mutex para proteger el contador de escritores
pthread_mutex_t mutex_administradores; // Mutex para proteger el contador de administradores
bool inicializado = false;
/*CODIGO PRINCIPAL*/

void iniciarProblema1()
{
    srand(time(NULL));
    pthread_t hilo_L_E[CANTIDAD_THREAD];
    int tipo_proceso;

    // NUEVOS

    sem_init(&sem_lectura, 0, 1);
    sem_init(&sem_escritura, 0, 1);
    sem_init(&sem_administracion, 0, 1);
    lectores = 0;
    escritores = 0;
    administradores = 0;
    pthread_mutex_init(&mutex_lectores, NULL);
    pthread_mutex_init(&mutex_escritores, NULL);
    pthread_mutex_init(&mutex_administradores, NULL);


    for (size_t i = 0; i < N_LECTORES; i++)
    {
        if (pthread_create(&hilo_L_E[i], NULL, *lector, NULL) != 0)
        {
            perror("Error al crear el hilo lector");
            exit(EXIT_FAILURE);
        }
        lectores++;
    }

    for (size_t i = 0; i < N_ESCRITORES; i++)
    {
        if (pthread_create(&hilo_L_E[i], NULL, *escritor, NULL) != 0)
        {
            perror("Error al crear el hilo escritor");
            exit(EXIT_FAILURE);
        }
        escritores++;
    }

    for (size_t i = 0; i < N_ADMINISTRADORES; i++)
    {
        if (pthread_create(&hilo_L_E[i], NULL, *administrador, NULL) != 0)
        {
            perror("Error al crear el hilo administrador");
            exit(EXIT_FAILURE);
        }

        administradores++;
    }
    

    printf("total procesos lectores: %d\n", lectores);
    printf("total procesos escritores: %d\n", escritores);
    printf("total procesos administradores: %d\n", administradores);

    char aux[100];

    sprintf(aux, "total procesos lectores: %d", lectores);
    append_to_file(FILENAME_PROBLEMA1, aux);

    sprintf(aux, "total procesos escritores: %d", escritores);
    append_to_file(FILENAME_PROBLEMA1, aux);

    sprintf(aux, "total procesos administradores: %d", administradores);
    append_to_file(FILENAME_PROBLEMA1, aux);


    inicializado = true;
    while (true) {}
}

#endif