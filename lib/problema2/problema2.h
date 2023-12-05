
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

#include "../config/problema2_config.h"

#include "../utilities/resources_utility.h"

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
    bool esperando;
} Jugador;

/**
 * @brief estructura que representa las estadisticas de una partida
 * @param reordenadas_total numero de veces que se ha reordenado el tablero
 * @param cartas_jugar_total numero de cartas que se han jugado
 * @param cartas_esperar_total numero de cartas que se han esperado
 */
typedef struct EstadisticasPartida
{
    long reordenadas_total;
    long cartas_jugar_total;
    long cartas_esperar_total;
} EstadisticasPartida;

// prototipos de funciones
/**
 * @brief funcion que representa la ejecucion de un jugador
 * @param data estructura que contiene la informacion del jugador
 */
void *jugador(void *data);
void *jefeMesa(void *data);

// *** Variables globales ***

// semaforo
sem_t mazo;
sem_t jugadores;
sem_t mutex_jefe;
sem_t jugadores_disponibles;
sem_t sem_jugadores;

int jugando = 0;

int cartas[10];

int n_esperando;

bool jugadores_jugando_bool[NUM_JUGADORES];

int jugadores_esperando = 0;

int jugadores_jugando = NUM_JUGADORES;

// Estadisticas
EstadisticasPartida estadisticas = {0, 0, 0};

int carta_actual = MAX_CARTAS - 1;
int cartas_disponibles = MAX_CARTAS;

// variables auxiliares
bool empezar_juego = false;

// mutext
pthread_mutex_t mutex_esperando = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cartas_disponibles = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_reordenando = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_jugadores_disponibles = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_jugadores_esperando = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_mazo = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_jugadores = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reordenando = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_jugando = PTHREAD_MUTEX_INITIALIZER;

// Mutex para estadisticas
pthread_mutex_t mutex_reordenadas_total = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cartas_jugar_total = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cartas_esperar_total = PTHREAD_MUTEX_INITIALIZER;

// mutex estadisticas del jugador
pthread_mutex_t mutex_cartas_jugar = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cartas_esperar = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_ordenadas = PTHREAD_MUTEX_INITIALIZER;

int total_reordenamiento;

/**
 * archivos de cabecera. Se coloca aqui para evitar errores de compilacion
 * debido a que las variables globales deben ser declaradas antes de ser usadas en los archivos de cabecera
 */
#include "jugadores_problema2.h"
#include "jefe_mesa_problema2.h"

void empezar_juego_problema2()
{
    while (!empezar_juego)
    {
    }
}

// Funcion principal
void iniciarProblema2()
{

    pthread_t *jugadores_t = (pthread_t *)malloc(sizeof(pthread_t) * NUM_JUGADORES);
    pthread_t jefe_t;
    sem_init(&mazo, 0, MAX_CARTAS);
    sem_init(&mutex_jefe, 0, 0);
    sem_init(&jugadores_disponibles, 0, 0);

    // inicializar mutex axiliares

    pthread_mutex_lock(&mutex_jugadores_esperando);
    pthread_mutex_lock(&mutex_reordenando);

    // Inicializar cartas
    for (int i = 0; i < MAX_CARTAS; i++)
    {
        cartas[i] = rand() / (RAND_MAX / 2);
    }

    for (long i = 0; i < NUM_JUGADORES; i++)
    {
        Jugador *data = malloc(sizeof(Jugador));
        data->id = i;
        data->jugadas = 0;
        data->cartas_jugar = 0;
        data->cartas_esperar = 0;
        jugadores_jugando_bool[i] = false;
        if (0 != pthread_create(&jugadores_t[i], NULL, jugador, (void *)data))
        {
            printf("Error al crear el hilo del jugador %ld\n", i);
            exit(-1);
        }
    }

    void *status_jefe;
    if (0 != pthread_create(&jefe_t, NULL, jefeMesa, NULL))
    {
        printf("Error al crear el hilo del jefe de mesa\n");
        exit(-1);
    }

    empezar_juego = true;

    pthread_join(jefe_t, &status_jefe);
}

void *jugador(void *args)
{

    empezar_juego_problema2();

    Jugador *data = (Jugador *)args;

    while (true)
    {

        pthread_mutex_lock(&reordenando);
        pthread_mutex_unlock(&reordenando);
        pthread_mutex_lock(&mutex_jugadores);

        if (!jugadores_jugando_bool[data->id])
        {
            jugadores_jugando_bool[data->id] = true;
            printf("Jugador %ld ingresa al turno\n", data->id);
        }

        pthread_mutex_unlock(&mutex_jugadores);

        pensar_jugada(data);

        tomar_carta(data);
    }

    pthread_exit(NULL);
}

void *jefeMesa(void *arg)
{

    empezar_juego_problema2();

    while (true)
    {

        pensar_reordenamiento();
        sem_wait(&mutex_jefe);

        reordenar_tablero();

        pthread_mutex_lock(&reordenando);

        pthread_mutex_lock(&mutex_mazo);
        total_reordenamiento++;
        printf("Total de reordenamientos: %i \n", total_reordenamiento);
        for (int i = 0; i < MAX_CARTAS; i++)
        {
            int aux = elegir_proxima_carta();
            colocar_carta_en_mazo(aux, i);
        }
        carta_actual = MAX_CARTAS - 1;
        cartas_disponibles = MAX_CARTAS;
        for (int i = 0; i < NUM_JUGADORES; i++)
        {
            int aux;
            sem_post(&jugadores_disponibles);
            jugadores_jugando_bool[i] = false;
            sem_getvalue(&jugadores_disponibles, &aux);
            printf("Jugadores habilitados: %d\n", aux);
        }
        jugadores_jugando = NUM_JUGADORES;
        pthread_mutex_unlock(&mutex_mazo);
        pthread_mutex_unlock(&reordenando);
    }

    pthread_exit(NULL);
}

#endif // PROBLEMA2_H