#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

#ifndef JUGADORES_PROBLEMA2_H

#define JUGADORES_PROBLEMA2_H
#define CARTA_ESPERAR 0
#define CARTA_JUGAR 1

// external typedef
typedef Jugador;

// variables externas
extern sem_t mazo, reordenando, mutex_mazo;
extern int num_jugadores;

extern jefe_esperando;

void pensar_jugada();
int sacar_carta_de_mazo();
void jugar();




/**
 * @brief retorna un numero aleatorio entre 0 y 1
*/
int sacar_carta_de_mazo(struct Jugador data)
{

    sem_wait(&mutex_mazo);
    sem_wait(&mazo);
    int aux = rand() % 2;
    data.carta = aux;
    sem_post(&mutex_mazo);

    return aux;
}

void pensar_jugada(struct Jugador data)
{
    printf("Jugador %d pensando jugada\n", num_jugadores);
}

void jugar(struct Jugador data)
{
    

}

#endif // JUGADORES_PROBLEMA2_H