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

// variables externas
extern sem_t mazo, reordenando, mutex_mazo;
extern int num_jugadores;
extern int cartas[MAX_CARTAS];
extern int cartas_disponibles;

extern bool jefe_esperando;
extern pthread_mutex_t mutex_jugadores;
// mutex

extern pthread_mutex_t mutex_cartas_disponibles;
extern pthread_mutex_t mutex_reordenando;

void pensar_jugada();
int tomar_carta(struct Jugador *data);
void jugar();

/**
 * @brief retorna un numero aleatorio entre 0 y 1
 */
int tomar_carta(struct Jugador *data)
{

    if (reordenado) {
        pthread_mutex_lock(&mutex_reordenando);
    }

    sem_wait(&mazo);
    sem_wait(&mutex_mazo);
    printf("Jugador %ld tomando carta del mazo\n", data->id);
    int aux = cartas[cartas_disponibles - 1];
    data->carta = 0;
    pthread_mutex_lock(&mutex_cartas_disponibles);
    cartas_disponibles--;
    pthread_mutex_unlock(&mutex_cartas_disponibles);
    if (cartas_disponibles == 0)
    {
        printf("Ya no hay cartas disponibles\n");
        sem_post(&mutex_jefe);
    }
    sem_post(&mutex_mazo);

    return aux;
}

void pensar_jugada(struct Jugador data)
{
    printf("Jugador %ld pensando jugada\n", data.id);
    if (reordenado) {
        printf("Jugador %ld reordenando\n", data.id);
        pthread_mutex_lock(&mutex_reordenando);
    }
}

void jugar(struct Jugador data)
{

    if (data.carta == CARTA_ESPERAR)
    {
        printf("Jugador %ld esperando\n", data.id);
        pthread_mutex_lock(&mutex_jugadores);
    }
    else
    {
        printf("Jugador %ld jugando\n", data.id);
    }
}

#endif // JUGADORES_PROBLEMA2_H