
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#ifndef JEFE_MESA_PROBLEMA2_H

#define JEFE_MESA_PROBLEMA2_H


// estadisticas

extern struct EstadisticasPartida estadisticas;

// mutex estadisticas
extern pthread_mutex_t mutex_reordenadas_total;

// variables externas
extern sem_t mazo, reordenando, mutex_mazo;
extern int num_jugadores;
extern bool reordenado;

extern bool jefe_esperando;
extern bool reordenamiento_terminado;
extern int cartas[10];
extern int n_disponibles;
extern int jugando;
extern int cartas_disponibles;
extern int sem_jugadores_value;

// mutex

extern pthread_mutex_t mutex_esperando;
extern pthread_mutex_t mutex_cartas_disponibles;
extern pthread_mutex_t mutex_reordenando;
extern sem_t mutex_jefe;

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
    pthread_mutex_lock(&mutex_cartas_disponibles);
    cartas_disponibles = MAX_CARTAS;
    pthread_mutex_unlock(&mutex_cartas_disponibles);
    printf("El jefe de mesa ha terminado de reordenar el tablero\n");

    pthread_mutex_lock(&mutex_reordenadas_total);
    estadisticas.reordenadas_total++;
    printf("El jefe de mesa ha reordenado el tablero %d veces\n", estadisticas.reordenadas_total);
    pthread_mutex_unlock(&mutex_reordenadas_total);

    sem_post(&mutex_mazo);
 
    printf("El jefe de mesa ha terminado de reordenar el tablero. Por lo tanto, dara lugar a que los jugadores que esperaban vuelvan a jugar\n");
    
}

void pensar_reordenamiento()
{
    printf("Jefe de mesa pensando reordenamiento\n");
    sem_wait(&mutex_jefe);
    printf("El jefe de mesa ha terminado de pensar el reordenamiento\n");
}

int elegir_proxima_carta()
{
    int aux = rand() % 2;

    printf("El jefe de mesa ha elegido la carta %d\n", aux);

    return aux;
}

void colocar_carta_en_mazo(int carta, int i)
{
    cartas[i] = carta;
    sem_post(&mazo);
    printf("El jefe de mesa ha colocado la carta %d en el mazo\n", cartas[i]);
}

#endif // JEFE_MESA_PROBLEMA2_H