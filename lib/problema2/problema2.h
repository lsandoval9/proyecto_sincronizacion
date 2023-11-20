
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

#ifndef PROBLEMA2_H

// Definir constante de proteccion
#define PROBLEMA2_H

// Definir constantes generales
#define MAX_CARTAS 10

/**
 * @brief estructura que representa la informacion de un jugador
 */
typedef struct Jugador
{
    long id;
    int carta;
    long jugadas;
    long cartas_jugar;
    long cartas_esperar;

} Jugador;

// prototipos de funciones
void *jugador(void *arg);
void *jefeMesa(void *arg);

// *** Variables globales ***

// semaforo
sem_t mazo, reordenando, jugadores;
sem_t mutex_mazo;

// contador jugadores
int num_jugadores = 0;
/**
 * 0: carta para esperar
 * 1: carta para jugar
 */
int carta_actual = 0;

bool reordenado = false;
bool jefe_esperando = false;

/**
 * archivos de cabecera. Se coloca aqui para evitar errores de compilacion
 * debido a que las variables globales deben ser declaradas antes de ser usadas en los archivos de cabecera
 */
#include "jugadores_problema2.h"
#include "jefe_mesa_problema2.h"

// Funcion principal
void iniciarProblema2(int jugadores)
{

    if (jugadores < 1)
    {
        printf("El numero de jugadores debe ser mayor a 0\n");
        exit(-1);
    }

    pthread_t *jugadores_t = malloc(sizeof(pthread_t) * jugadores);
    pthread_t jefe_t;
    sem_init(&mazo, 0, 10);
    sem_init(&reordenando, 0, 0);

    for (long i = 0; i < jugadores; i++)
    {
        Jugador *data = malloc(sizeof(Jugador));
        data->id = i;
        printf("Creando jugador %ld\n", i);
        data->jugadas = 0;
        data->cartas_jugar = 0;
        data->cartas_esperar = 0;
        if (0 != pthread_create(&jugadores_t[i], NULL, jugador, (void *)data))
        {
            printf("Error al crear el hilo del jugador %ld\n", i);
            exit(-1);
        }
        pthread_join(jugadores_t[i], NULL);
    }

    if (0 != pthread_create(&jefe_t, NULL, jefeMesa, NULL))
    {
        printf("Error al crear el hilo maestro\n");
        exit(-1);
    }
    pthread_join(jefe_t, NULL);
}

void *jugador(void *arg)
{

    Jugador *data = (struct Jugador *)arg;

    while (true)
    {
        pensar_jugada(*data);

        

    
        if (data->carta == CARTA_ESPERAR)
        {
            while (!reordenado)
            {
                printf("Jugador %ld esperando reordenamiento\n", data->id);
            }
        }

        jugar(*data);
    }

    pthread_exit(NULL);
}

void *jefeMesa(void *arg)
{
}

#endif // PROBLEMA2_H