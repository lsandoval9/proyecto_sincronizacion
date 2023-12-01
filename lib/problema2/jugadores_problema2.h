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

// variables externas
extern sem_t mazo, reordenando, mutex_mazo;
extern int num_jugadores;
extern int cartas[MAX_CARTAS];
extern int cartas_disponibles;

extern bool jefe_esperando;
extern bool reordena;
extern bool reordenamiento_terminado;
extern sem_t sem_jugadores;
// mutex

extern pthread_mutex_t mutex_cartas_disponibles;
extern pthread_mutex_t mutex_reordenando;
extern sem_t mutex_jefe;

void pensar_jugada();
int tomar_carta(struct Jugador *data);
void jugar();
void esperar_reordenamiento();

/**
 * @brief retorna un numero aleatorio entre 0 y 1
 */
int tomar_carta(struct Jugador *data)
{

    if (reordena) {

        //cambios aqui
        pthread_mutex_lock(&mutex_jugadores_disponibles);
        n_esperando++;
        pthread_mutex_unlock(&mutex_jugadores_disponibles);

        sem_wait(&sem_jugadores);

    }

    sem_wait(&mazo);
    sem_wait(&mutex_mazo);
    int aux = cartas[cartas_disponibles - 1];
    data->carta = aux;
    printf("Jugador %ld tomo carta %d\n", data->id, aux);
    printf("Cartas disponibles: %d\n", cartas_disponibles-1);
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

void pensar_jugada(struct Jugador *data)
{
    printf("Jugador %ld pensando jugada\n", data->id);


    if (reordena) {
        printf("Jugador %ld en espera reordenamiento\n", data->id);

        //cambios aqui
        pthread_mutex_lock(&mutex_jugadores_disponibles);
        n_esperando++;
        pthread_mutex_unlock(&mutex_jugadores_disponibles);

        sem_wait(&sem_jugadores);
    }
}

void jugar(struct Jugador *data)
{

    if (data->carta == CARTA_ESPERAR)
    {
        printf("Jugador %ld procede a esperar reordenamiento debido a su CARTA_ESPERA\n", data->id);
        sem_wait(&sem_jugadores);
        printf("Jugador %ld termino de esperar para jugar CARTA_ESPERA\n", data->id);
        

        data->cartas_esperar++;
        printf("Jugador %ld tiene %d cartas esperar\n", data->id, data->cartas_esperar);

        pthread_mutex_lock(&mutex_cartas_esperar);
        estadisticas.cartas_esperar_total += 1;
        printf("Hay %d cartas esperar en total\n", estadisticas.cartas_esperar_total);
        pthread_mutex_unlock(&mutex_cartas_esperar);

        /*
        bool es_ultimo = false;// inicia la variable
        while (algo) {
            
            post(semaforo);//aqui agarra y va dando permisos a los jugadores q estaban en espera
            sem_getvalue(&semaforo, &actual); // obtiene el valor actual y lo guarda en actual
            //si el jactual es el ultimo, entonces significa que termino de or ldarle permiso a los jal jultimo jugador para jque jueguee
            if (actual = ultimo) //cambia la bandera y sale del whileo) {
                es_ultimo = true;
            }
        }
        // JEFE ******
        
        // JUGADOR ******
        // salen todos a la vez
        while (!es_ultimo) {}
        */
    }
    else
    {
        printf("Jugador %ld jugando\n", data->id);

        pthread_mutex_lock(&mutex_cartas_jugar);
        data->cartas_jugar++;
        pthread_mutex_unlock(&mutex_cartas_jugar);
        printf("Jugador %ld tiene %d cartas jugar\n", data->id, data->cartas_jugar);

        pthread_mutex_lock(&mutex_cartas_jugar);
        estadisticas.cartas_jugar_total++;
        printf("Hay %d cartas jugar en total\n", estadisticas.cartas_jugar_total);
        pthread_mutex_unlock(&mutex_cartas_jugar);

    }
}



#endif // JUGADORES_PROBLEMA2_H