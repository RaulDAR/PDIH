# PONG en C con ncurses  

Este proyecto implementa una versión del clásico juego Pong utilizando el lenguaje C y la librería ncurses para crear una interfaz en modo texto. 
El objetivo principal es aprender a manejar ventanas, colores, entrada de teclado y refresco de pantalla dentro de un terminal Linux.

------------------------------------------------------------
## Descripción general del programa

El programa recrea una partida de Pong para dos jugadores en la misma terminal. Consta de tres partes principales:

### Pantalla de bienvenida
Muestra:
- Título del juego
- Información de la práctica
- Controles de cada jugador
- Un marco decorativo
- Un mensaje para comenzar

El juego empieza cuando el usuario pulsa cualquier tecla.

### Bucle principal del juego
Aquí se desarrolla la lógica del juego:
- Se dibujan las palas, la pelota, el marcador y el campo.
- Se leen las teclas pulsadas sin bloquear la ejecución.
- La pelota se mueve con velocidad en los ejes X e Y.
- Rebota en paredes y palas.
- Si la pelota sale por un lateral, el jugador contrario suma un punto.
- La partida termina cuando un jugador alcanza 5 puntos.

### Pantalla final
Cuando un jugador gana:
- Se muestra un mensaje indicando el ganador.
- Se enseña el marcador final.
- Se ofrece volver a jugar o salir.

------------------------------------------------------------
## Controles

Jugador 1 (izquierda)  
- W: subir  
- S: bajar  

Jugador 2 (derecha)  
- Flecha arriba: subir  
- Flecha abajo: bajar  

Otros  
- Q: salir del juego en cualquier momento  

------------------------------------------------------------
## Opciones de configuración

El programa no incluye un menú de ajustes, pero sí varias constantes que permiten modificar su comportamiento desde el código:

### Velocidad del juego

#define RETARDO 70000

Controla el tiempo entre frames (en microsegundos).  
Valores más bajos hacen el juego más rápido.  
Valores más altos lo hacen más lento.

### Puntuación necesaria para ganar

#define PUNTOS_MAX 5

Permite cambiar cuántos puntos hacen falta para ganar la partida.

### Tamaño de las palas

#define ALTO_PALA 7
Aumentarlo facilita el juego; reducirlo lo hace más difícil.

### Colores del juego
Los pares de colores se definen en la función `inicializar_colores()`:

init_pair(COLOR_TITULO,   COLOR_YELLOW, COLOR_BLACK);
init_pair(COLOR_PELOTA,   COLOR_RED,    COLOR_BLACK);
init_pair(COLOR_PALA1,    COLOR_CYAN,   COLOR_BLACK);
init_pair(COLOR_PALA2,    COLOR_GREEN,  COLOR_BLACK);
init_pair(COLOR_MARCADOR, COLOR_WHITE,  COLOR_BLACK);
init_pair(COLOR_GANADOR,  COLOR_BLACK,  COLOR_YELLOW);
init_pair(COLOR_BORDE,    COLOR_BLUE,   COLOR_BLACK);

Estos valores pueden modificarse para personalizar el aspecto del juego.

### Velocidad inicial de la pelota

juego->vel_x = (rand() % 2 == 0) ?  1.5f : -1.5f;
juego->vel_y = (rand() % 2 == 0) ?  1.0f : -1.0f;

Modificando estos valores se puede ajustar la dificultad o la rapidez del juego.

------------------------------------------------------------
## Estructura del código

El estado del juego se almacena en una estructura:

typedef struct {
int filas, columnas;
float pelota_x, pelota_y;
float vel_x, vel_y;
int pala1_y, pala2_y;
int puntos1, puntos2;
} EstadoJuego;

Esto permite pasar toda la información entre funciones sin usar variables globales.

------------------------------------------------------------
## Conclusión

Esta práctica permite familiarizarse con ncurses y con la creación de interfaces interactivas en modo texto. 
Además, programar Pong es una buena forma de practicar animación, colisiones, entrada de teclado y control del flujo del juego en tiempo real.


