#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>

#include "../config/problema2_config.h"

#ifndef JUGADORES_PROBLEMA2_H

#define JUGADORES_PROBLEMA2_H

// external typedef
typedef struct Jugador Jugador;

// estadisticas del jugador
extern struct EstadisticasPartida estadisticas;

// mutex estadisticas
extern pthread_mutex_t mutex_cartas_jugar_total;
extern pthread_mutex_t mutex_cartas_esperar_total;

// mutex jugadores
extern pthread_mutex_t mutex_cartas_jugar;
extern pthread_mutex_t mutex_cartas_esperar;
extern pthread_mutex_t mutex_jugando;

// variables externas
extern bool empezar_juego;
extern int jugadores_jugando;
extern int cartas_disponibles;
extern int carta_actual;
extern bool jugadores_jugando_bool[NUM_JUGADORES];
// mutex

extern pthread_mutex_t mutex_cartas_disponibles;
extern pthread_mutex_t mutex_reordenando;
extern pthread_mutex_t mutex_mazo;
extern sem_t mutex_jefe;

void pensar_jugada();
void tomar_carta(struct Jugador *data);
void jugar();
void esperar_reordenamiento();

/**
 * @brief retorna un numero aleatorio entre 0 y 1
 */
void tomar_carta(struct Jugador *data)
{

    int aux;
    sem_wait(&mutex_mazo);

    if (cartas_disponibles <= 0)
    {
        aux = -1;
    }
    else
    {
        printf("Hay %d cartas disponibles\n", cartas_disponibles);
        aux = cartas[carta_actual];
        carta_actual--;
        cartas_disponibles--;
    }

    pthread_mutex_lock(&mutex_jugando);
    if (aux == -1)
    {
        printf("Jugador %ld no pudo tomar carta\n", data->id);
    }
    else
    {   
        printf("Jugador %ld tomo carta %d\n", data->id, aux);
        if (aux == CARTA_JUGAR)
        {
            estadisticas.cartas_jugar_total++;
            data->cartas_jugar++;
            printf("El jugador %ld ha jugado %d cartas para jugar\n", data->id, data->cartas_jugar);
            printf("Se han jugado %d cartas para jugar en total\n", estadisticas.cartas_jugar_total);
            
        }
        else if (aux == CARTA_ESPERAR)
        {
            estadisticas.cartas_esperar_total++;
            data->cartas_esperar++;
            printf("El jugador %ld ha jugado %d cartas para esperar\n", data->id, data->cartas_esperar);
            printf("Se han jugado %d cartas para esperar en total\n", estadisticas.cartas_esperar_total);
        }
    }

    sem_post(&mutex_mazo);
    if (aux == CARTA_JUGAR)
    {
        jugar(data);
        pthread_mutex_unlock(&mutex_jugando);
    }
    else
    {
        printf("Jugador %ld esperando reordenamiento\n", data->id);
        esperar_reordenamiento(data);
    }
}

void pensar_jugada(struct Jugador *data)
{
    printf("Jugador %ld pensando jugada\n", data->id);
    recursos(PROBLEMA2_RECURSOS);
}

void jugar(struct Jugador *data)
{
    printf("Jugador %ld jugando\n", data->id);
    recursos(PROBLEMA2_RECURSOS);
}

void esperar_reordenamiento(struct Jugador *data)
{

    pthread_mutex_lock(&mutex_jugadores);
    jugadores_jugando--;
    jugadores_jugando_bool[data->id] = false;
    printf("Jugadores en partida: %d\n", jugadores_jugando);
    pthread_mutex_unlock(&mutex_jugando);
    if (jugadores_jugando == 0)
    {
        printf("Se da la señal de reordenamiento\n");
        sem_post(&mutex_jefe);
        pthread_mutex_unlock(&reordenando);
    }
    pthread_mutex_unlock(&mutex_jugadores);
    sem_wait(&jugadores_disponibles);
    printf("Pasa el jugador %ld\n", data->id);
}

#endif // JUGADORES_PROBLEMA2_H