/*PROYECTO SISTEMAS OPERATIVOS. PROBLEMA 1*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>


typedef struct Proceso
{
    long id;
    long operaciones;
} Proceso;


#include "../config/problema1_config.h"
#include "../utilities/file_utility.h"
#include "procesos_problema1.h"

#ifndef PROBLEMA1_H

#define PROBLEMA1_H

// Definir las variables globales
sem_t sem_lectura;                     // Semáforo para controlar el acceso de los lectores
sem_t sem_escritura;                   // Semáforo para controlar el acceso de los escritores
sem_t sem_administracion;              // Semáforo para controlar el acceso de los administradores
int lectores = 0;                          // Contador de lectores activos
int escritores = 0;                        // Contador de escritores activos
int administradores = 0;                   // Contador de administradores activos

int lectores_activos = 0;                 // Contador de lectores activos
int escritores_activos = 0;               // Contador de escritores activos
int administradores_activos = 0;          // Contador de administradores activos

pthread_mutex_t mutex_lectores;        // Mutex para proteger el contador de lectores
pthread_mutex_t mutex_escritores;      // Mutex para proteger el contador de escritores
pthread_mutex_t mutex_administradores; // Mutex para proteger el contador de administradores
bool inicializado = false;



/**
 * @brief Función que inicializa el problema 1
 */
void iniciarProblema1()
{
    srand(time(NULL));
    pthread_t hilo_administrador[N_ADMINISTRADORES];
    pthread_t hilo_lector[N_LECTORES];
    pthread_t hilo_escritor[N_ESCRITORES];

    // NUEVOS

    sem_init(&sem_lectura, 0, N_LECTORES);
    sem_init(&sem_escritura, 0, N_ESCRITORES);
    sem_init(&sem_administracion, 0, N_ADMINISTRADORES);
    lectores = 0;
    escritores = 0;
    administradores = 0;
    pthread_mutex_init(&mutex_lectores, NULL);
    pthread_mutex_init(&mutex_escritores, NULL);
    pthread_mutex_init(&mutex_administradores, NULL);

    // inicializar los lectores
    for (size_t i = 0; i < N_LECTORES; i++)
    {
        Proceso *data = malloc(sizeof(Proceso));
        data->id = i;
        if (pthread_create(&hilo_lector[i], NULL, *lector, (void *) data) != 0)
        {
            perror("Error al crear el hilo lector");
            exit(EXIT_FAILURE);
        }
        lectores++;
        
    }

    // inicializar los escritores
    for (size_t i = 0; i < N_ESCRITORES; i++)
    {
        Proceso *data = malloc(sizeof(Proceso));
        data->id = i;
        if (pthread_create(&hilo_escritor[i], NULL, *escritor, data) != 0)
        {
            perror("Error al crear el hilo escritor");
            exit(EXIT_FAILURE);
        }
        escritores++;
        
    }

    // inicializar los administradores
    for (size_t i = 0; i < N_ADMINISTRADORES; i++)
    {
        Proceso *data = malloc(sizeof(Proceso));
        data->id = i;
        if (pthread_create(&hilo_administrador[i], NULL, *administrador, data) != 0)
        {
            perror("Error al crear el hilo administrador");
            exit(EXIT_FAILURE);
        }

        administradores++;
        
    }

    printf("total procesos lectores: %d\n", lectores);
    printf("total procesos escritores: %d\n", escritores);
    printf("total procesos administradores: %d\n", administradores);

    inicializado = true;
    
    for (size_t i = 0; i < N_LECTORES; i++)
    {
        pthread_join(hilo_lector[i], NULL);
    }

    for (size_t i = 0; i < N_ESCRITORES; i++)
    {
        pthread_join(hilo_escritor[i], NULL);
    }

    for (size_t i = 0; i < N_ADMINISTRADORES; i++)
    {
        pthread_join(hilo_administrador[i], NULL);
    }

    // liberar memoria

    sem_destroy(&sem_lectura);
    sem_destroy(&sem_escritura);
    sem_destroy(&sem_administracion);
    pthread_mutex_destroy(&mutex_lectores);
    pthread_mutex_destroy(&mutex_escritores);
    pthread_mutex_destroy(&mutex_administradores);


}

#endif