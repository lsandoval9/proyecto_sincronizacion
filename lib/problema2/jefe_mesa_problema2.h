
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#ifndef JEFE_MESA_PROBLEMA2_H

#define JEFE_MESA_PROBLEMA2_H


// variables
extern bool empezar_juego;
extern int cartas[10];
// estadisticas

extern struct EstadisticasPartida estadisticas;

extern int sem_jugadores_value;

// mutex

extern pthread_mutex_t mutex_esperando;
extern pthread_mutex_t mutex_cartas_disponibles;
extern pthread_mutex_t mutex_reordenando;
extern sem_t mutex_jefe;
extern sem_t jugadores_disponibles;

void pensar_reordenamiento();
void reordenar_tablero();
int elegir_proxima_carta();
void colocar_carta_en_mazo(int carta, int i);

void reordenar_tablero()
{
    printf("Jefe de mesa reordenando tablero\n");
    sleep_thread(PROBLEMA2_WAIT_TIME);
}

void pensar_reordenamiento()
{
    printf("Jefe de mesa pensando reordenamiento\n");
    sleep_thread(PROBLEMA2_WAIT_TIME);
}

int elegir_proxima_carta()
{
    int carta = rand() / (RAND_MAX / 2);
    printf("Jefe de mesa eligiendo carta %d\n", carta);
    return carta;
}

void colocar_carta_en_mazo(int carta, int i)
{
    cartas[i] = carta;
}

#endif // JEFE_MESA_PROBLEMA2_H