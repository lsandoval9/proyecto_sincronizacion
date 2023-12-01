

# PROYECTO: Sincronización entre Procesos
## Sistemas Operativos - Prof. Mirella Herrera


| Integrantes | Cedula |
| ------ | ------ |
| Luis A. Sandoval | 26.781.082 |
| Gustavo Herrera | 27.852.734 |
| Jose David Lopez | 30.077.008 |

[Enlace al repositorio](https://github.com/lsandoval9/proyecto_sincronizacion)

# Descripcion de las partes del proyecto


## Problema 1

Este problema se basa en sincronizar varios hilos que acceden a una base de datos compartida. Los hilos pueden realizar tres tipos de operaciones: leer, escribir y administrar. Estas operaciones tienen diferentes niveles de prioridad y restricciones de acceso.

Se debe evitar que los lectores lean mientras un escritor esta escribiendo, y que los escritores escriban mientras un lector esta leyendo. Los administradores pueden leer y escribir, pero no pueden hacerlo mientras un lector o escritor esta leyendo o escribiendo.

## Problema 2

Este problema se basa en sincronizar varios procesos jugadores con un proceso jefe de mesa, el cual, es el encargado de rellenar el mazo cuando se quede sin cartas. El proceso jefe de mesa tiene un mazo de cartas, el cual, tiene un numero maximo de cartas. El proceso jefe tambien es el encargado de reordenar el tablero.

Se debe proteger la seccion critica, la cual esta conformada principalmente por el mazo, el numero de jugadores y el tablero, el cual solo puede ser usado por los jugadores si el jeje de mesa no esta reordenando.

## Utilidades

Se crearon varias utilidades con propositos de facilitar el desarrollo y la muestra de resultados.

- file_utility.h:

Esta libreria contiene funciones para la escritura de archivos en la carpeta ./out del proyecto.

- sleep_threaad_utility.h:

Esta libreria contiene una funcionalidad para dormir un hilo X nanosegundos. Esto se hizo con fines de facilitar el seguimiento de los hilos en la terminal.

- time_utility.h:

Esta utilidad contiene una funcion para obtener el tiempo actual en nanosegundos. Esto se hizo para poder mostrar en la terminal el tiempo transcucrrido en cada hilo desde que comenzo la ejecucion del programa.

- stopwatch_thread.h:

Este archivo contiene un hilo el cual utilizara la funcion de time_utility.h para mostrar en la terminal el tiempo transcurrido desde que comenzo la ejecucion del programa.


- app_time.py:

Este script se encarga de medir el uso del CPU, memoria y tiempo de ejecucion de un programa. Para esto, se debe ejecutar el script de la siguiente manera:

```sh 
python app_time.py <nombre del programa | ejecutable generado por gcc> <parametros del programa>
```

## Configuracion

* problema1_config.h:

Contiene constantes para configurar los parametros del problema 1, entre ellos, el numero de lectores, administradores y escritores. Algunos de estos parametros son:

```c
#define CANTIDAD_THREAD 20 // cantidad de procesos
#define SIZEDATETIME 128 //CONSTANTE PARA LAS CADENAS DE LA FECHA Y HORA DEL PROCESO
#define N_LECTORES 3 // cantidad de lectores
#define N_ESCRITORES 3 // cantidad de escritores
#define N_ADMINISTRADORES 3 // cantidad de administradores
#define PROBLEMA1_WAIT_TIME 0 //25000000 // tiempo de espera para el problema 1
```

* problema2_config.h:

Contiene constantes para configurar los parametros del problema 2, como el numero de jugadores. Algunos de estos parametros son:

```c
#define CARTA_ESPERAR 0 // carta que indica que el jugador debe esperar
#define CARTA_JUGAR 1  // carta que indica que el jugador debe jugar
#define MAX_CARTAS 10 // numero maximo de cartas en el mazo
#define NUM_JUGADORES 10 // numero de jugadores
#define PROBLEMA2_WAIT_TIME 0 // 225000000 // tiempo de espera para el problema 1
```