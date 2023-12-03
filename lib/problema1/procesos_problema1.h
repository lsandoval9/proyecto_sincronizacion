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
extern sem_t sem_administracion_lectura;      // Semáforo para controlar el acceso de los administradores a la lectura
extern sem_t sem_administracion_escritura;    // Semáforo para controlar el acceso de los administradores a la escritura
extern int lectores;
extern int escritores;
extern int administradores;
extern pthread_mutex_t mutex_lectores;
extern pthread_mutex_t mutex_escritores;
extern pthread_mutex_t mutex_administradores;

extern pthread_mutex_t mutex_administracion_lectura;
extern pthread_mutex_t mutex_administracion_escritura;

extern pthread_mutex_t mutex_lectores_activos;
extern pthread_mutex_t mutex_escritores_activos;

extern pthread_mutex_t mutex_escribiendo;

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

        
        pthread_mutex_lock(&mutex_administracion_lectura); 
        if (administradores_activos > 0)
        { // Si hay un administrador
            printf("# Lector %ld esperando a que no haya administradores\n", proceso->id);
            sem_wait(&sem_administracion_lectura);
        }
        pthread_mutex_unlock(&mutex_administracion_lectura);

        pthread_mutex_lock(&mutex_escritores_activos);
        if (escritores_activos > 0)
        { // Si hay un escritor
            printf("# Lector %ld esperando a que no hayan escritores\n", proceso->id);
            sem_wait(&sem_escritura);
            printf("# Lector %ld continuando\n", proceso->id);
        }
        lectores_activos++;
        lecturas++;
        pthread_mutex_unlock(&mutex_escritores_activos);
        sem_post(&sem_escritura);

        printf("# Lector %ld leyendo... %lld operaciones de lectura en total\n", proceso->id, lecturas);

        pthread_mutex_lock(&mutex_escritores);
        lectores_activos--;
        if (lectores_activos == 0)
        { // Si es el último escritor
            printf("# Escritor %ld permitiendo lectores\n", proceso->id);
            sem_post(&sem_escritura);
        }
        pthread_mutex_unlock(&mutex_escritores);

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


        pthread_mutex_lock(&mutex_administracion_escritura);
        if (administradores_activos > 0)
        { // Si hay un administrador
            printf("# Escritor %ld esperando a que no haya administradores\n", proceso->id);
            sem_wait(&sem_administracion_escritura);
            printf("# Escritor %ld continuando\n", proceso->id);
        }
        pthread_mutex_unlock(&mutex_administracion_escritura);

        pthread_mutex_lock(&mutex_lectores_activos);
        if (lectores_activos > 0)
        { // Si es el primer escritor
            printf("# Escritor %ld esperando a que no hayan lectores\n", proceso->id);
            sem_wait(&sem_lectura);
        }
        pthread_mutex_unlock(&mutex_lectores_activos);
        sem_post(&sem_lectura);

        pthread_mutex_lock(&mutex_escribiendo);
        escrituras++;
        escritores_activos++;
        printf("# Escritor %ld escribiendo... %lld operaciones de escritura en total\n", proceso->id, escrituras);
        pthread_mutex_unlock(&mutex_escribiendo);


        pthread_mutex_lock(&mutex_escritores);
        escritores_activos--;
        if (escritores_activos == 0)
        { // Si es el último escritor
            printf("# Escritor %ld permitiendo lectores\n", proceso->id);
            sem_post(&sem_escritura);
        }
        pthread_mutex_unlock(&mutex_escritores);
       
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