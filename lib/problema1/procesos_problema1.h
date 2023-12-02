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

extern sem_t sem_cola_lectores, sem_lector, sem_contador_lectores;
extern sem_t sem_contador_escritores, sem_escritor;
extern int contador_lectores;
extern int contador_escritores;
extern int contador_administradores;
extern bool inicializado;

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
    while (!inicializado)
    {
    }
}

/**
 * @brief Función que ejecuta cada hilo lector
 */
void *lector(void *arg)
{

    esperar_inicio();

    while (true)
    {

        sleep_thread(PROBLEMA1_WAIT_TIME);

        printf("Lector esperando a que no haya administradores ni escritores\n");
        sem_wait(&sem_administracion);
        pthread_mutex_lock(&mutex_lectores); // Bloquear el mutex de lectores
        lectores++;                          // Incrementar el contador de lectores
        if (lectores == 1)
        { // Si es el primer lector
            printf("Lector esperando a que no haya escritores\n");
            sem_wait(&sem_escritura); // Esperar a que no haya escritores
            printf("Lector continuando\n");
        }
        pthread_mutex_unlock(&mutex_lectores); // Desbloquear el mutex de lectores
        sem_post(&sem_administracion);         // Permitir que haya administradores

        lecturas++;
        printf("$ Lector leyendo... %lld operaciones de lectura en total\n", lecturas);

        pthread_mutex_lock(&mutex_lectores); // Bloquear el mutex de lectores
        lectores--;                          // Decrementar el contador de lectores
        if (lectores == 0)
        {                             // Si es el último lector
            sem_post(&sem_escritura); // Permitir que haya escritores
            printf("Lector permitiendo escritores\n");
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

    esperar_inicio();

    while (true)
    {

        sleep_thread(PROBLEMA1_WAIT_TIME);
        printf("Escritor esperando a que no haya administradores ni lectores\n");
        sem_wait(&sem_administracion);         // Esperar a que no haya administradores
        pthread_mutex_lock(&mutex_escritores); // Bloquear el mutex de escritores
        escritores++;                          // Incrementar el contador de escritores
        if (escritores == 1)
        { // Si es el primer escritor
            printf("Escritor esperando a que no haya lectores\n");
            sem_wait(&sem_lectura); // Esperar a que no haya lectores
            printf("Escritor continuando\n");
        }
        pthread_mutex_unlock(&mutex_escritores); // Desbloquear el mutex de escritores
        sem_post(&sem_administracion);           // Permitir que haya administradores

        sem_wait(&sem_escritura); // Esperar a que no haya otro escritor

        escrituras++;
        printf("* Escritor escribiendo... %lld operaciones de escritura en total\n", escrituras);

        sem_post(&sem_escritura); // Permitir que haya otro escritor

        pthread_mutex_lock(&mutex_escritores); // Bloquear el mutex de escritores
        escritores--;                          // Decrementar el contador de escritores
        if (escritores == 0)
        { // Si es el último escritor
            printf("Escritor permitiendo lectores\n");
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

    esperar_inicio();

    while (true)
    {

        sleep_thread(PROBLEMA1_WAIT_TIME);
        // Repetir indefinidamente
        sem_wait(&sem_administracion);              // Esperar a que no haya otro administrador
        pthread_mutex_lock(&mutex_administradores); // Bloquear el mutex de administradores
        administradores++;                          // Incrementar el contador de administradores
        if (administradores == 1)
        { // Si es el primer administrador
            printf("Administrador esperando a que no haya lectores ni escritores\n");
            sem_wait(&sem_lectura);   // Esperar a que no haya lectores
            sem_wait(&sem_escritura); // Esperar a que no haya escritores
            printf("Administrador continuando\n");
        }
        pthread_mutex_unlock(&mutex_administradores); // Desbloquear el mutex de administradores
        sem_post(&sem_administracion);                // Permitir que haya otro administrador

        administraciones++;
        printf("# Administrador administrando... %lld operaciones de administracion en total\n", administraciones);

        pthread_mutex_lock(&mutex_administradores); // Bloquear el mutex de administradores
        administradores--;                          // Decrementar el contador de administradores
        if (administradores == 0)
        { // Si es el último administrador
            printf("Administrador permitiendo lectores y escritores\n");
            sem_post(&sem_lectura);   // Permitir que haya lectores
            sem_post(&sem_escritura); // Permitir que haya escritores
        }
        pthread_mutex_unlock(&mutex_administradores); // Desbloquear el mutex de administradores
    }
    return NULL; // Finalizar el hilo
}

#endif