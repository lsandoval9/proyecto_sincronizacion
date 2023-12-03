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
#include "../utilities/sleep_thread_utility.h"


typedef struct Jugador Jugador;

extern sem_t sem_cola_lectores, sem_lector, sem_contador_lectores;
extern sem_t sem_contador_escritores, sem_escritor;
extern int contador_lectores;
extern int contador_escritores;
extern int contador_administradores;
extern bool inicializado;
extern int lectores_activos;
extern int escritores_activos;
extern int administradores_activos;

// Definir las variables globales
extern sem_t sem_lectura;
extern sem_t sem_escritura;
extern sem_t sem_administracion;
extern int lectores;
extern int escritores;
extern int administradores;
extern pthread_mutex_t mutex_lectores;
extern pthread_mutex_t mutex_escritores;
extern pthread_mutex_t mutex_administradores;

long long lecturas = 0;
long long escrituras = 0;
long long administraciones = 0;

#ifndef PROCESOS_H

#define PROCESOS_H

void esperar_inicio()
{
    while (!inicializado) {}
}

/**
 * @brief Función que ejecuta cada hilo lector
 */
void *lector(void *arg)
{

    Proceso *proceso = (Proceso *)arg;

    esperar_inicio();

    while (true)
    {

        sleep_thread(PROBLEMA1_WAIT_TIME);

        printf("$ Lector %ld esperando a que no haya administradores ni escritores\n", proceso->id);
        sem_wait(&sem_administracion);
        pthread_mutex_lock(&mutex_lectores); // Bloquear el mutex de lectores
        lectores_activos++;                  // Incrementar el contador de lectores
        if (lectores_activos == 1)
        { // Si es el primer lector
            printf("$ Lector %ld esperando a que no haya escritores\n", proceso->id);
            sem_wait(&sem_escritura); // Esperar a que no haya escritores
            printf("$ Lector %ld continuando\n", proceso->id);
        }
        pthread_mutex_unlock(&mutex_lectores); // Desbloquear el mutex de lectores
        sem_post(&sem_administracion);         // Permitir que haya administradores

        pthread_mutex_lock(&mutex_lectores); // Bloquear el mutex de lectores
        lecturas++;
        proceso->operaciones++;
        printf("$ Lector %ld leyendo... %lld operaciones de lectura en total\n", proceso->id, lecturas);
        lectores_activos--; // Decrementar el contador de lectores
        if (lectores_activos == 0)
        {                             // Si es el último lector
            sem_post(&sem_escritura); // Permitir que haya escritores
            printf("$ Lector %ld permitiendo escritores\n");
        }
        pthread_mutex_unlock(&mutex_lectores); // Desbloquear el mutex de lectores
    }
    return NULL; // Finalizar el hilo
}

/**
 * @brief Función que ejecuta cada hilo escritor
 */
void *escritor(void *arg)
{

    Proceso *proceso = (Proceso *)arg;

    esperar_inicio();

    while (true)
    {
        sleep_thread(PROBLEMA1_WAIT_TIME);
        printf("* Escritor %ld esperando a que no haya administradores ni lectores\n", proceso->id);
        sem_wait(&sem_administracion);         // Esperar a que no haya administradores
        pthread_mutex_lock(&mutex_escritores); // Bloquear el mutex de escritores
        escritores_activos++;                  // Incrementar el contador de escritores
        if (escritores_activos == 1)
        { // Si es el primer escritor
            printf("* Escritor %ld esperando a que no haya lectores\n", proceso->id);
            sem_wait(&sem_lectura); // Esperar a que no haya lectores
            printf("* Escritor %ld continuando\n", proceso->id);
        }
        pthread_mutex_unlock(&mutex_escritores); // Desbloquear el mutex de escritores

        sem_post(&sem_administracion);           // Permitir que haya administradores

        pthread_mutex_lock(&mutex_escritores); // Bloquear el mutex de escritores
        escrituras++;
        printf("* Escritor escribiendo... %lld operaciones de escritura en total\n", escrituras);
        escritores_activos--; // Decrementar el contador de escritores
        if (escritores_activos == 0)
        { // Si es el último escritor
            printf("* Escritor %ld permitiendo lectores\n", proceso->id);
            sem_post(&sem_lectura); // Permitir que haya lectores
        }
        pthread_mutex_unlock(&mutex_escritores); // Desbloquear el mutex de escritores
    }
    return NULL; // Finalizar el hilo
}

/**
 * @brief Función que ejecuta cada hilo administrador
 */
void *administrador(void *arg)
{

    Proceso *proceso = (Proceso *)arg;

    esperar_inicio();

    while (true)
    {
        sleep_thread(PROBLEMA1_WAIT_TIME);
        // Repetir indefinidamente
        // Esperar a que no haya otro administrador
        pthread_mutex_lock(&mutex_administradores); // Bloquear el mutex de administradores
        administradores_activos++;                  // Incrementar el contador de administradores
        if (administradores_activos == 1)
        { // Si es el primer administrador
            sem_wait(&sem_administracion);
            printf("# Administrador %ld continuando\n", proceso->id);
        }
        administraciones++;
        printf("# Administrador %ld administrando... %lld operaciones de administracion en total\n", proceso->id, administraciones);
        printf("# Hay %d administradores de forma concurrente en la seccion critica\n", administradores_activos);
        pthread_mutex_unlock(&mutex_administradores); // Desbloquear el mutex de administradores
                                                      // Permitir que haya otro administrador

        pthread_mutex_lock(&mutex_administradores); // Bloquear el mutex de administradores
        administradores_activos--;                  // Decrementar el contador de administradores
        if (administradores_activos == 0)
        { // Si es el último administrador
            printf("# Administrador %ld permitiendo lectores y escritores\n", proceso->id);
            sem_post(&sem_administracion);
        }
        pthread_mutex_unlock(&mutex_administradores); // Desbloquear el mutex de administradores
    }
    return NULL; // Finalizar el hilo
}

#endif