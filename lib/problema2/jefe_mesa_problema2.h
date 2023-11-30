
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#ifndef JEFE_MESA_PROBLEMA2_H

#define JEFE_MESA_PROBLEMA2_H

// variables externas
extern sem_t mazo, reordenando, mutex_mazo;
extern int num_jugadores;
extern bool reordenado;
extern bool jefe_esperando;
extern int cartas[10];
extern int esperando;
extern int jugando;
extern int cartas_disponibles;

// mutex

extern pthread_mutex_t mutex_esperando;
extern pthread_mutex_t mutex_cartas_disponibles;
extern pthread_mutex_t mutex_reordenando;

void pensar_reordenamiento();
void reordenar_tablero();
int elegir_proxima_carta();
void colocar_carta_en_mazo(int carta, int i);

void reordenar_tablero()
{
    sem_wait(&mutex_mazo);
    for (int i = 0; i < MAX_CARTAS; i++)
    {
        int carta = elegir_proxima_carta();
        colocar_carta_en_mazo(carta, i);
    }
    
    printf("El jefe de mesa ha terminado de reordenar el tablero\n");
    sem_post(&mutex_mazo);
    pthread_mutex_lock(&mutex_esperando);
    esperando = 0;
    pthread_mutex_unlock(&mutex_esperando);
    pthread_mutex_unlock(&mutex_reordenando);
    printf("El jefe de mesa ha terminado de reordenar el tablero DESPUES MUTEX\n");
    
}

void pensar_reordenamiento()
{
    printf("Jefe de mesa pensando reordenamiento\n");
    sem_wait(&mutex_jefe);
    printf("El jefe de mesa ha terminado de pensar el reordenamiento\n");
}

int elegir_proxima_carta()
{
    return rand() % 2;
}

void colocar_carta_en_mazo(int carta, int i)
{
    cartas[i] = carta;
    sem_post(&mazo);
    pthread_mutex_lock(&mutex_cartas_disponibles);
    cartas_disponibles++;
    pthread_mutex_unlock(&mutex_cartas_disponibles);
}

#endif // JEFE_MESA_PROBLEMA2_H