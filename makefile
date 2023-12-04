# Definir las variables
CC = gcc # El compilador a usar
CFLAGS = -Wall -Wextra -g -pthread -lpthread -lrt -std=c99 # Las opciones de compilación
LDFLAGS = -Llib # La ruta de las librerías externas
LDLIBS = -lm -lrt # Las librerías a enlazar
TARGET = main.bin # El nombre del ejecutable
OBJS = main.o # Los archivos objeto a generar

# Definir la regla por defecto
all: $(TARGET) 

# Definir la regla para generar el ejecutable y eliminar objetos
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET) $(LDLIBS) 

# Definir la regla para generar los archivos objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Definir la regla para limpiar los archivos generados
clean:
	rm -f $(OBJS)

cleanall:
	rm -f $(OBJS) $(TARGET)
