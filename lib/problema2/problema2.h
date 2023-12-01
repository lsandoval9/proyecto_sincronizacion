
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

#include "../config/problema2_config.h"
#include "../utilities/file_utility.h"

#include "../utilities/sleep_thread_utility.h"

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

/**
 * @brief estructura que representa las estadisticas de una partida
 * @param reordenadas_total numero de veces que se ha reordenado el tablero
 * @param cartas_jugar_total numero de cartas que se han jugado
 * @param cartas_esperar_total numero de cartas que se han esperado
 */
typedef struct EstadisticasPartida
{
    int reordenadas_total;
    int cartas_jugar_total;
    int cartas_esperar_total;
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
sem_t reordenando;
sem_t jugadores;
sem_t mutex_mazo;
sem_t mutex_jefe;

sem_t jugadores_disponibles;

sem_t sem_jugadores;

// contador jugadores
int num_jugadores = 0;

int n_disponibles;

int jugando = 0;

int cartas[10];

// Estadisticas
EstadisticasPartida estadisticas = {0, 0, 0};

/**
 * 0: carta para esperar
 * 1: carta para jugar
 */
int carta_actual = 0;

int cartas_disponibles = MAX_CARTAS;

int n_esperando = 0;

int sem_jugadores_value;

bool reordenado = false;
bool reordena = false;
bool jefe_esperando = false;

bool empezar_jugadores = false;

bool reordenamiento_terminado = false;

// mutext
pthread_mutex_t mutex_esperando = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cartas_disponibles = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_reordenando = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_jugadores_disponibles = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_jugadores_esperando = PTHREAD_MUTEX_INITIALIZER;

// Mutex para estadisticas
pthread_mutex_t mutex_reordenadas_total = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cartas_jugar_total = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cartas_esperar_total = PTHREAD_MUTEX_INITIALIZER;

// mutex estadisticas del jugador
pthread_mutex_t mutex_cartas_jugar = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cartas_esperar = PTHREAD_MUTEX_INITIALIZER;

/**
 * archivos de cabecera. Se coloca aqui para evitar errores de compilacion
 * debido a que las variables globales deben ser declaradas antes de ser usadas en los archivos de cabecera
 */
#include "jugadores_problema2.h"
#include "jefe_mesa_problema2.h"

// Funcion principal
void iniciarProblema2()
{

    append_to_file(FILENAME_PROBLEMA2, "Iniciando problema 2");

    num_jugadores = NUM_JUGADORES;
    pthread_t *jugadores_t = (pthread_t*) malloc(sizeof(pthread_t) * NUM_JUGADORES);
    pthread_t jefe_t;
    sem_init(&mazo, 0, MAX_CARTAS);
    sem_init(&reordenando, 0, 0);
    sem_init(&mutex_jefe, 0, 0);
    sem_init(&mutex_mazo, 0, 1);
    sem_init(&jugadores_disponibles, 0, 0);
    sem_init(&sem_jugadores, 0, 0);

    sem_jugadores_value = (int*) malloc(sizeof(int));

    // inicializar mutex axiliares

    pthread_mutex_lock(&mutex_jugadores_esperando);
    pthread_mutex_lock(&mutex_reordenando);

    // Inicializar cartas
    for (int i = 0; i < MAX_CARTAS; i++)
    {
        cartas[i] = rand() % 2;
    }

    for (long i = 0; i < NUM_JUGADORES; i++)
    {
        Jugador *data = malloc(sizeof(Jugador));
        data->id = i;
        data->jugadas = 0;
        data->cartas_jugar = 0;
        data->cartas_esperar = 0;
        cartas_disponibles = MAX_CARTAS;
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

    empezar_jugadores = true;

    pthread_join(jefe_t, &status_jefe);
}

void *jugador(void *args)
{

    while (!empezar_jugadores)
    {
    }

    append_to_file(FILENAME_PROBLEMA2, "Iniciando jugador");

    Jugador *data = (struct Jugador *)args;

    char jugadorStr[50];
    sprintf(jugadorStr, "Jugador %ld", data->id);
    append_to_file(FILENAME_PROBLEMA2, jugadorStr);

    while (true)
    {

        sleep_thread(PROBLEMA2_WAIT_TIME);
        pensar_jugada(data);

        sleep_thread(PROBLEMA2_WAIT_TIME);
        tomar_carta(data);

        printf("La carta del jugador %ld es %d\n", data->id, data->carta);

        if (data->carta == CARTA_ESPERAR)
        {
           /* pthread_mutex_lock(&mutex_jugadores_disponibles);
            n_esperando++;
            pthread_mutex_unlock(&mutex_jugadores_disponibles);
            printf("**************** El valor de esperando es %d ****************\n", n_esperando);
            printf("**************** El valor de num_jugadores es %d ****************\n", num_jugadores);*/
            sem_getvalue(&sem_jugadores, &sem_jugadores_value);


           // if (n_esperando == NUM_JUGADORES)
            if(sem_jugadores_value == NUM_JUGADORES)
            {
                printf("Ya no hay jugadores jugando\n");
                sem_post(&mutex_jefe);
            }
        }

        jugar(data);
    }

    pthread_exit(NULL);
}

void *jefeMesa(void *arg)
{

    while (!empezar_jugadores) {}

    while (true)
    {

        sleep_thread(PROBLEMA2_WAIT_TIME);

        pensar_reordenamiento();

        reordena = true;

        sleep_thread(PROBLEMA2_WAIT_TIME);

        reordenar_tablero();

        sem_getvalue(&sem_jugadores, &sem_jugadores_value);
        for (int i = 0; i < sem_jugadores_value; i++)
        {
            sem_post(&sem_jugadores);
        }
      /*  pthread_mutex_lock(&mutex_jugadores_disponibles);
        n_esperando = 0;
        pthread_mutex_unlock(&mutex_jugadores_disponibles);*/
        reordena = false;
    }

    pthread_exit(NULL);
}

#endif // PROBLEMA2_H