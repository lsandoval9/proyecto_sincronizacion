#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>

#ifndef JUGADORES_PROBLEMA2_H

#define JUGADORES_PROBLEMA2_H
#define CARTA_ESPERAR 0
#define CARTA_JUGAR 1

// external typedef
typedef Jugador;

// variables externas
extern sem_t mazo, reordenando, mutex_mazo;
extern int num_jugadores;

extern cartas_disponibles;

extern bool jefe_esperando;

void pensar_jugada();
int tomar_carta();
void jugar();

/**
 * @brief retorna un numero aleatorio entre 0 y 1
 */
int tomar_carta(struct Jugador data)
{

    sem_wait(&mazo);
    sem_wait(&mutex_mazo);
    int aux = rand() % 2; // TODO: tomar carta del arreglo de cartas
    data.carta = aux;
    cartas_disponibles--;
    if (cartas_disponibles == 0)
    {
        sem_post(&mutex_jefe);
    }
    sem_post(&mutex_mazo);

    return aux;
}

void pensar_jugada(struct Jugador data)
{
    printf("Jugador %ld pensando jugada\n", data.id);
    esperando--;
    wait(&reordenando);
}

void jugar(struct Jugador data)
{

    if (data.carta == CARTA_ESPERAR)
    {
        printf("Jugador %ld esperando\n", data.id);
    }
    else
    {
        printf("Jugador %ld jugando\n", data.id);
    }
}

#endif // JUGADORES_PROBLEMA2_H