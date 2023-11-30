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

extern sem_t sem_cola_lectores, sem_lector, sem_contador_lectores;
extern sem_t sem_contador_escritores, sem_escritor;
extern int contador_lectores;
extern int contador_escritores;
extern int contador_administradores;
extern bool inicializado;

#ifndef PROCESOS_H

#define PROCESOS_H


// procedimiento para procesos lectores
void *lectores(void *args)
{

    while (!inicializado) {}

    sem_wait(&sem_cola_lectores);
    sem_wait(&sem_lector);
    sem_wait(&sem_contador_lectores);
    contador_lectores++;
    if (contador_lectores == 1)
    {
        sem_wait(&sem_escritor);
    }
    sem_post(&sem_contador_lectores);
    sem_post(&sem_lector);
    sem_post(&sem_cola_lectores);
    //append_to_file(FILENAME_PROBLEMA1, "leyendo datos...");
    printf("leyendo datos...\n");
    sem_wait(&sem_contador_lectores);
    contador_lectores--;
    if (contador_lectores == 0)
    {
        sem_post(&sem_escritor);
    }
    sem_post(&sem_contador_lectores);
}

// procedimiento para procesos escritores

void *escritores(void *args)
{
    while (!inicializado) {}

    sem_wait(&sem_contador_escritores);
    contador_escritores++;
    if (contador_escritores == 1)
    {
        sem_wait(&sem_lector);
    }
    sem_post(&sem_contador_escritores);
    sem_wait(&sem_escritor);
    //append_to_file(FILENAME_PROBLEMA1, "escribiendo datos...");
    printf("escribiendo datos...\n");
    sem_post(&sem_escritor);
    sem_wait(&sem_contador_escritores);
    contador_escritores--;
    if (contador_escritores == 0)
    {
        sem_post(&sem_lector);
    }
    sem_post(&sem_contador_escritores);
}

// proceso administrador

void *administrador(void *args)
{

    while (!inicializado) {}

    sem_wait(&sem_lector);
    sem_wait(&sem_escritor);
    //append_to_file(FILENAME_PROBLEMA1, "administrando...");
    printf("administrando...\n");
    sem_post(&sem_escritor);
    sem_post(&sem_lector);
}


#endif