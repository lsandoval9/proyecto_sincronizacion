#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h> 
#define CANTIDAD_THREAD 40 // cantidad de procesos
#define SIZEDATETIME 128 //CONSTANTE PARA LAS CADENAS DE LA FECHA Y HORA DEL PROCESO


/*------ ESTRUCUTRA DE DATOS ------*/
struct nodo 
{
  int codigo_operacion;
  char registro_operacion[SIZEDATETIME]; 
  char registro_operacion_admin[SIZEDATETIME]; 
  struct nodo *siguiente;
};

struct cola 
{
  struct nodo *inicio;
  struct nodo *fin;
};

struct cola *crearCola()
{
  struct cola *cola = malloc(sizeof(struct cola));
  cola->inicio = NULL;
  cola->fin = NULL;
  return cola;
}

void encolar(struct cola *cola, int valor) 
{
  struct nodo *nuevo_nodo = malloc(sizeof(struct nodo));
  nuevo_nodo-> codigo_operacion = valor;
  strcpy(nuevo_nodo->registro_operacion, get_time()); 
  nuevo_nodo->siguiente = NULL;

	  if (cola->inicio == NULL) 
	  {
	    cola->inicio = nuevo_nodo;
	    cola->fin = nuevo_nodo;
	  }
	  else 
	  {
	    cola->fin->siguiente = nuevo_nodo;
	    cola->fin = nuevo_nodo;
	  }
}

void encolar_proceso(struct cola *c, int valor) 
{
	if(c->inicio !=NULL && c->fin != NULL) //si existen operaciones se determina que hacer respecto a la operación
	{
		if(c->fin->codigo_operacion <= 0) // ultima operación en cola es lectura/escritura solapada u operación de administración individual
		{	
			encolar(c, valor); 
		}
		else // si la última operacion en cola es lectura o escritura 
		{
			if(valor = 0) // si la nueva opración es administración, se solapa 
			{
				c->fin->codigo_operacion *= -1;
				strcpy(c->fin->registro_operacion_admin, get_time()); 
			}
			else // sino, significa que el nuevo valor a ingresar es lectura o escritura (no "solapable")
			{
				encolar(c, valor); 
			}
		}
	}
	else //en caso de no existir ninguna operación
	{
		encolar(c, valor); 
	}
}


int desencolar(struct cola *cola) 
{
  if (cola->inicio == NULL) {
    return -1;
  }

  int valor = cola->inicio->codigo_operacion;
  struct nodo *nodoEliminado = cola->inicio;
  cola->inicio = cola->inicio->siguiente;
  free(nodoEliminado);

  if (cola->inicio == NULL) {
    cola->fin = NULL;
  }

  return valor;
}

int verPrimero(struct cola *cola) 
{
  if (cola->inicio == NULL) {
    return -1;
  }

  return cola->inicio->codigo_operacion;
}

int verUltimo(struct cola *c)
{
	if(c->fin == NULL)
	{
		return -10; 
	}
	
	return c->fin->codigo_operacion; 
}

int estaVacia(struct cola *cola) 
{
  return cola->inicio == NULL;
}
