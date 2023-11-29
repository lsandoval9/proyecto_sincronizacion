
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

#include "../config/problema2_config.h"
#include "../utilities/file_utility.h"

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
sem_t mutex_jugadores;
sem_t mutex_jefe;

// mutex auxiliares

sem_t sem_x;

// contador jugadores
int num_jugadores = 0;

int esperando = 0;

int jugando = 0;

int cartas[10];

/**
 * 0: carta para esperar
 * 1: carta para jugar
 */
int carta_actual = 0;

int cartas_disponibles = MAX_CARTAS;

bool reordenado = true;
bool jefe_esperando = false;

bool empezar_jugadores = false;

// mutext
pthread_mutex_t mutex_esperando = PTHREAD_MUTEX_INITIALIZER;


/**
 * archivos de cabecera. Se coloca aqui para evitar errores de compilacion
 * debido a que las variables globales deben ser declaradas antes de ser usadas en los archivos de cabecera
 */
#include "jugadores_problema2.h"
#include "jefe_mesa_problema2.h"

// Funcion principal
void iniciarProblema2()
{

    appendToFile(FILENAME_PROBLEMA2, "Iniciando problema 2");


    num_jugadores = NUM_JUGADORES;
    pthread_t *jugadores_t = malloc(sizeof(pthread_t) * NUM_JUGADORES);
    pthread_t jefe_t;
    sem_init(&mazo, 0, 10);
    sem_init(&reordenando, 0, 0);
    sem_init(&mutex_jefe, 0, 0);
    sem_init(&mutex_mazo, 0, 1);

    // inicializar mutex axiliares
    sem_init(&sem_x, 0, 1);

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

    while(true) {}

}


void *jugador(void *args)
{

    
    while (!empezar_jugadores) {}

    appendToFile(FILENAME_PROBLEMA2, "Iniciando jugador");

    Jugador *data = (struct Jugador *) args;

    char jugadorStr[50];
    sprintf(jugadorStr, "Jugador %ld", data->id);
    appendToFile(FILENAME_PROBLEMA2, jugadorStr);

    while (true)
    {
        pensar_jugada(*data);

        tomar_carta(*data);

        if (data->carta == CARTA_ESPERAR)
        {
            
            pthread_mutex_lock(&mutex_esperando);
            esperando++;
            pthread_mutex_unlock(&mutex_esperando);
            if (esperando == num_jugadores)
            {
                sem_post(&mutex_jefe);
            }
        }

        jugar(*data);
    }

    pthread_exit(NULL);
    return;
}

void *jefeMesa(void *arg)
{
    
    while (!empezar_jugadores) {}

    while (true)
    {

        pensar_reordenamiento();

        reordenar_tablero();
    }

    pthread_exit(NULL);
}

#endif // PROBLEMA2_H