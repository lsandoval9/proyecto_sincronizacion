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
#include "procesos_problema1.h"

#ifndef PROBLEMA1_H

#define PROBLEMA1_H

// Definir las variables globales
sem_t sem_lectura;                     // Semáforo para controlar el acceso de los lectores
sem_t sem_escritura;                   // Semáforo para controlar el acceso de los escritores
sem_t sem_administracion;              // Semáforo para controlar el acceso de los administradores
sem_t sem_administracion_lectura;      // Semáforo para controlar el acceso de los administradores a la lectura
sem_t sem_administracion_escritura;    // Semáforo para controlar el acceso de los administradores a la escritura
int lectores = 0;                          // Contador de lectores activos
int escritores = 0;                        // Contador de escritores activos
int administradores = 0;                   // Contador de administradores activos

int lectores_activos = 0;                 // Contador de lectores activos
int escritores_activos = 0;               // Contador de escritores activos
int administradores_activos = 0;          // Contador de administradores activos

pthread_mutex_t mutex_lectores = PTHREAD_MUTEX_INITIALIZER;        // Mutex para proteger el contador de lectores
pthread_mutex_t mutex_escritores = PTHREAD_MUTEX_INITIALIZER;       // Mutex para proteger el contador de escritores
pthread_mutex_t mutex_administradores = PTHREAD_MUTEX_INITIALIZER;  // Mutex para proteger el contador de administradores
pthread_mutex_t mutex_administracion_lectura = PTHREAD_MUTEX_INITIALIZER;  // Mutex para proteger el contador de administradores
pthread_mutex_t mutex_administracion_escritura = PTHREAD_MUTEX_INITIALIZER;  // Mutex para proteger el contador de administradores
pthread_mutex_t mutex_lectores_activos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_escritores_activos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_escribiendo = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_operacion = PTHREAD_MUTEX_INITIALIZER;
 pthread_mutex_t mutex_lectura = PTHREAD_MUTEX_INITIALIZER;
 pthread_mutex_t mutex_escritura = PTHREAD_MUTEX_INITIALIZER;

 pthread_mutex_t mutex_aux_lectura  = PTHREAD_MUTEX_INITIALIZER;
 pthread_mutex_t mutex_aux_escritura = PTHREAD_MUTEX_INITIALIZER;

 pthread_mutex_t mutex_administracion_lectura_espera = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_administracion_escritura_espera = PTHREAD_MUTEX_INITIALIZER;

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

    sem_init(&sem_lectura, 0, 1);
    sem_init(&sem_escritura, 0, 1);
    sem_init(&sem_administracion, 0, 1);
    sem_init(&sem_administracion_lectura, 0, 0);
    sem_init(&sem_administracion_escritura, 0, 0);
    lectores = 0;
    escritores = 0;
    administradores = 0;
    pthread_mutex_init(&mutex_lectores, NULL);
    pthread_mutex_init(&mutex_escritores, NULL);
    pthread_mutex_init(&mutex_administradores, NULL);

    pthread_mutex_lock(&mutex_lectores_activos);
    pthread_mutex_lock(&mutex_escritores_activos);

    pthread_mutex_lock(&mutex_administracion_lectura_espera);
    pthread_mutex_lock(&mutex_administracion_escritura_espera);

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