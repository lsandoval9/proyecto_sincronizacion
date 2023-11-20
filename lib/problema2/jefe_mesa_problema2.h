
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

void pensar_reordenamiento();
void reordenar_tablero();
int elegir_proxima_carta();
void colocar_carta_en_mazo(int carta, int i);

void reordenar_tablero()
{
    jefe_esperando = true;

    // TODO
    for (int i = 0; i < MAX_CARTAS; i++)
    {
        int carta = elegir_proxima_carta();
        colocar_carta_en_mazo(carta, i);
    }

    for (int i = 0; i < esperando; i++)
    {
        sem_post(&reordenando);
    }
}

void pensar_reordenamiento()
{

    // !el Jefe piensa los cambios mientras hayan jugadores no en espera o cartas en el mazo
    sem_wait(&mutex_jefe);
    printf("Jefe de mesa pensando reordenamiento\n");

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
    cartas_disponibles++;
}

#endif // JEFE_MESA_PROBLEMA2_H