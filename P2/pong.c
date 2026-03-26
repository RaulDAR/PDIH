// Compilar: gcc pong.c -o pong -lncurses
// Ejecutar: ./pong
//
// Controles:
//   Jugador 1 (izquierda): W sube, S baja
//   Jugador 2 (derecha): flecha arriba sube, flecha abajo baja
//   Q: salir

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Constantes del juego
#define RETARDO      70000  // microsegundos entre cada frame
#define PUNTOS_MAX   5      // puntos necesarios para ganar
#define ALTO_PALA    7      // numero de filas que ocupa cada pala

// Indices de los pares de colores que vamos a usar
#define COLOR_TITULO   1  // amarillo sobre negro
#define COLOR_PELOTA   2  // rojo sobre negro
#define COLOR_PALA1    3  // cian sobre negro
#define COLOR_PALA2    4  // verde sobre negro
#define COLOR_MARCADOR 5  // blanco sobre negro
#define COLOR_GANADOR  6  // negro sobre amarillo
#define COLOR_BORDE    7  // azul sobre negro

// Estructura con todo el estado del juego
typedef struct {
    int filas, columnas; // tamanio de la pantalla

    // posicion y velocidad de la pelota
    float pelota_x, pelota_y;
    float vel_x, vel_y;

    // posicion vertical del extremo superior de cada pala
    int pala1_y, pala2_y;

    // puntuacion de cada jugador
    int puntos1, puntos2;
} EstadoJuego;

// Declaracion de funciones
void inicializar_colores();
void pantalla_bienvenida(int filas, int columnas);
void pantalla_fin(int filas, int columnas, int p1, int p2);
int  bucle_juego();
void dibujar_campo(EstadoJuego *juego);
void dibujar_palas(EstadoJuego *juego);
void dibujar_pelota(EstadoJuego *juego);
void dibujar_marcador(EstadoJuego *juego);
void reiniciar_pelota(EstadoJuego *juego);
void dibujar_marco(int filas, int columnas);


int main() {
    initscr(); // inicializar ncurses

    // comprobar que el terminal tiene colores
    if (has_colors() == FALSE) {
        endwin();
        printf("Este terminal no soporta colores, no se puede ejecutar el juego.\n");
        return 1;
    }

    inicializar_colores();
    cbreak();             // leer teclas sin esperar enter
    noecho();             // no mostrar las teclas pulsadas
    keypad(stdscr, TRUE); // activar teclas especiales (flechas, etc)
    curs_set(0);          // ocultar el cursor

    int filas, columnas;
    getmaxyx(stdscr, filas, columnas); // obtener tamanio de la pantalla

    // bucle principal: bienvenida -> partida -> resumen -> repetir
    while (1) {
        pantalla_bienvenida(filas, columnas);

        int ganador = bucle_juego(); // devuelve 1 o 2 segun quien gane

        // actualizar dimensiones por si cambio la ventana durante la partida
        getmaxyx(stdscr, filas, columnas);

        // mostrar la pantalla de fin con los puntos del ganador
        pantalla_fin(filas, columnas, (ganador == 1) ? PUNTOS_MAX : 0, (ganador == 2) ? PUNTOS_MAX : 0);

        // esperar tecla: Q para salir, cualquier otra para repetir
        int tecla = getch();
        if (tecla == 'q' || tecla == 'Q') break;
    }

    endwin(); // restaurar el terminal
    return 0;
}


// Inicializa los pares de colores que usaremos en todo el juego
void inicializar_colores() {
    start_color();
    init_pair(COLOR_TITULO,   COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_PELOTA,   COLOR_RED,    COLOR_BLACK);
    init_pair(COLOR_PALA1,    COLOR_CYAN,   COLOR_BLACK);
    init_pair(COLOR_PALA2,    COLOR_GREEN,  COLOR_BLACK);
    init_pair(COLOR_MARCADOR, COLOR_WHITE,  COLOR_BLACK);
    init_pair(COLOR_GANADOR,  COLOR_BLACK,  COLOR_YELLOW);
    init_pair(COLOR_BORDE,    COLOR_BLUE,   COLOR_BLACK);
}


// Dibuja un marco rectangular en toda la pantalla
void dibujar_marco(int filas, int columnas) {
    attron(COLOR_PAIR(COLOR_BORDE));

    // lineas horizontales arriba y abajo
    for (int c = 0; c < columnas; c++) {
        mvaddch(0, c, ACS_HLINE);
        mvaddch(filas - 1, c, ACS_HLINE);
    }
    // lineas verticales izquierda y derecha
    for (int f = 1; f < filas - 1; f++) {
        mvaddch(f, 0, ACS_VLINE);
        mvaddch(f, columnas - 1, ACS_VLINE);
    }
    // esquinas del marco
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(0, columnas - 1, ACS_URCORNER);
    mvaddch(filas - 1, 0, ACS_LLCORNER);
    mvaddch(filas - 1, columnas - 1, ACS_LRCORNER);

    attroff(COLOR_PAIR(COLOR_BORDE));
}


// Muestra la pantalla de bienvenida con titulo, autores y controles
void pantalla_bienvenida(int filas, int columnas) {
    clear();
    dibujar_marco(filas, columnas);

    // titulo del juego centrado
    attron(COLOR_PAIR(COLOR_TITULO) | A_BOLD);
    const char *titulo = "*** PONG ***";
    mvprintw(filas / 2 - 6, (columnas - strlen(titulo)) / 2, "%s", titulo);
    attroff(COLOR_PAIR(COLOR_TITULO) | A_BOLD);

    // nombre de la practica
    attron(COLOR_PAIR(COLOR_MARCADOR));
    const char *subtitulo = "Practica 2 - PDIH - UGR";
    mvprintw(filas / 2 - 4, (columnas - strlen(subtitulo)) / 2, "%s", subtitulo);
    attroff(COLOR_PAIR(COLOR_MARCADOR));

    // controles del jugador 1
    attron(COLOR_PAIR(COLOR_PALA1));
    mvprintw(filas / 2 - 1, (columnas / 2) - 24, "Jugador 1 (izquierda):");
    mvprintw(filas / 2, (columnas / 2) - 24, "  W -> subir");
    mvprintw(filas / 2 + 1, (columnas / 2) - 24, "  S -> bajar");
    attroff(COLOR_PAIR(COLOR_PALA1));

    // controles del jugador 2
    attron(COLOR_PAIR(COLOR_PALA2));
    mvprintw(filas / 2 - 1, (columnas / 2) + 4, "Jugador 2 (derecha):");
    mvprintw(filas / 2, (columnas / 2) + 4, "  Flecha arriba -> subir");
    mvprintw(filas / 2 + 1, (columnas / 2) + 4, "  Flecha abajo  -> bajar");
    attroff(COLOR_PAIR(COLOR_PALA2));

    // objetivo y mensaje para empezar
    attron(COLOR_PAIR(COLOR_MARCADOR));
    const char *objetivo = "Primero en llegar a 5 puntos gana.";
    mvprintw(filas / 2 + 3, (columnas - strlen(objetivo)) / 2, "%s", objetivo);
    const char *empezar = "[ Pulsa cualquier tecla para jugar ]";
    mvprintw(filas / 2 + 5, (columnas - strlen(empezar)) / 2, "%s", empezar);
    attroff(COLOR_PAIR(COLOR_MARCADOR));

    refresh();
    getch(); // esperar a que el jugador pulse una tecla
}


// Muestra la pantalla de fin de partida con el ganador y el marcador
void pantalla_fin(int filas, int columnas, int p1, int p2) {
    clear();
    dibujar_marco(filas, columnas);

    // mensaje del ganador
    attron(COLOR_PAIR(COLOR_GANADOR) | A_BOLD);
    const char *msg_ganador = (p1 > p2) ? "!!! GANA EL JUGADOR 1 !!!" : "!!! GANA EL JUGADOR 2 !!!";
    mvprintw(filas / 2 - 4, (columnas - strlen(msg_ganador)) / 2, "%s", msg_ganador);
    attroff(COLOR_PAIR(COLOR_GANADOR) | A_BOLD);

    // marcador final
    attron(COLOR_PAIR(COLOR_MARCADOR) | A_BOLD);
    mvprintw(filas / 2 - 1, (columnas / 2) - 10, "Marcador final:");
    mvprintw(filas / 2,     (columnas / 2) - 10, " Jugador 1: %d Jugador 2: %d", p1, p2);
    attroff(COLOR_PAIR(COLOR_MARCADOR) | A_BOLD);

    // opciones para continuar
    attron(COLOR_PAIR(COLOR_TITULO));
    const char *opciones = "[ Cualquier tecla para volver a jugar  |  Q para salir ]";
    mvprintw(filas / 2 + 3, (columnas - strlen(opciones)) / 2, "%s", opciones);
    attroff(COLOR_PAIR(COLOR_TITULO));

    refresh();
}


// Logica principal del juego. Devuelve 1 si gana J1, 2 si gana J2
int bucle_juego() {
    EstadoJuego juego;
    getmaxyx(stdscr, juego.filas, juego.columnas);

    // colocar palas en el centro verticalmente
    juego.pala1_y = juego.filas / 2 - ALTO_PALA / 2;
    juego.pala2_y = juego.filas / 2 - ALTO_PALA / 2;

    // empezar con puntuacion 0
    juego.puntos1 = 0;
    juego.puntos2 = 0;

    reiniciar_pelota(&juego);

    nodelay(stdscr, TRUE); // getch() no bloqueante para el bucle del juego

    while (1) {
        // leer entrada del teclado
        int tecla = getch();

        if (tecla == 'q' || tecla == 'Q') {
            nodelay(stdscr, FALSE);
            return 0; // salir
        }

        // mover pala del jugador 1 con W y S
        if ((tecla == 'w' || tecla == 'W') && juego.pala1_y > 1)
            juego.pala1_y--;
        if ((tecla == 's' || tecla == 'S') && juego.pala1_y + ALTO_PALA < juego.filas - 1)
            juego.pala1_y++;

        // mover pala del jugador 2 con las flechas
        if (tecla == KEY_UP   && juego.pala2_y > 1)
            juego.pala2_y--;
        if (tecla == KEY_DOWN && juego.pala2_y + ALTO_PALA < juego.filas - 1)
            juego.pala2_y++;

        // actualizar posicion de la pelota
        juego.pelota_x += juego.vel_x;
        juego.pelota_y += juego.vel_y;

        // rebote en la pared de arriba
        if ((int)juego.pelota_y <= 1) {
            juego.pelota_y = 1;
            juego.vel_y = -juego.vel_y;
        }

        // rebote en la pared de abajo
        if ((int)juego.pelota_y >= juego.filas - 2) {
            juego.pelota_y = juego.filas - 2;
            juego.vel_y = -juego.vel_y;
        }

        // comprobar colision con la pala del jugador 1
        if ((int)juego.pelota_x <= 2 && (int)juego.pelota_y >= juego.pala1_y && (int)juego.pelota_y <  juego.pala1_y + ALTO_PALA) {

            juego.vel_x = -juego.vel_x; // rebotar hacia la derecha
            juego.pelota_x = 2;

            // cambiar angulo segun donde golpee en la pala
            int zona = (int)juego.pelota_y - juego.pala1_y;
            juego.vel_y = (zona < ALTO_PALA / 2) ? -1.0f : 1.0f;
        }

        // comprobar colision con la pala del jugador 2
        if ((int)juego.pelota_x >= juego.columnas - 3 && (int)juego.pelota_y >= juego.pala2_y && (int)juego.pelota_y <  juego.pala2_y + ALTO_PALA) {

            juego.vel_x = -juego.vel_x; // rebotar hacia la izquierda
            juego.pelota_x = juego.columnas - 3;

            // cambiar angulo segun donde golpee en la pala
            int zona = (int)juego.pelota_y - juego.pala2_y;
            juego.vel_y = (zona < ALTO_PALA / 2) ? -1.0f : 1.0f;
        }

        // la pelota salio por la izquierda: punto para jugador 2
        if ((int)juego.pelota_x < 0) {
            juego.puntos2++;
            if (juego.puntos2 >= PUNTOS_MAX) {
                nodelay(stdscr, FALSE);
                return 2;
            }
            reiniciar_pelota(&juego);
            usleep(500000); // pausa de medio segundo antes de continuar
        }

        // la pelota salio por la derecha: punto para jugador 1
        if ((int)juego.pelota_x >= juego.columnas) {
            juego.puntos1++;
            if (juego.puntos1 >= PUNTOS_MAX) {
                nodelay(stdscr, FALSE);
                return 1;
            }
            reiniciar_pelota(&juego);
            usleep(500000);
        }

        // dibujar todo en cada frame
        clear();
        dibujar_campo(&juego);
        dibujar_marcador(&juego);
        dibujar_palas(&juego);
        dibujar_pelota(&juego);
        refresh();

        usleep(RETARDO); // esperar antes del siguiente frame
    }
}


// Dibuja los bordes del campo y la linea central punteada
void dibujar_campo(EstadoJuego *juego) {
    attron(COLOR_PAIR(COLOR_BORDE));

    // borde superior e inferior
    for (int c = 0; c < juego->columnas; c++) {
        mvaddch(0,                c, ACS_HLINE);
        mvaddch(juego->filas - 1, c, ACS_HLINE);
    }

    // linea central discontinua para separar los campos
    for (int f = 1; f < juego->filas - 1; f += 2)
        mvaddch(f, juego->columnas / 2, ACS_VLINE);

    attroff(COLOR_PAIR(COLOR_BORDE));
}


// Dibuja las dos palas en su posicion actual
void dibujar_palas(EstadoJuego *juego) {
    // pala del jugador 1 en color cian
    attron(COLOR_PAIR(COLOR_PALA1) | A_BOLD);
    for (int i = 0; i < ALTO_PALA; i++)
        mvaddch(juego->pala1_y + i, 1, ACS_BLOCK);
    attroff(COLOR_PAIR(COLOR_PALA1) | A_BOLD);

    // pala del jugador 2 en color verde
    attron(COLOR_PAIR(COLOR_PALA2) | A_BOLD);
    for (int i = 0; i < ALTO_PALA; i++)
        mvaddch(juego->pala2_y + i, juego->columnas - 2, ACS_BLOCK);
    attroff(COLOR_PAIR(COLOR_PALA2) | A_BOLD);
}


// Dibuja la pelota en su posicion actual
void dibujar_pelota(EstadoJuego *juego) {
    attron(COLOR_PAIR(COLOR_PELOTA) | A_BOLD);
    mvaddch((int)juego->pelota_y, (int)juego->pelota_x, 'O');
    attroff(COLOR_PAIR(COLOR_PELOTA) | A_BOLD);
}


// Dibuja el marcador en la parte superior de la pantalla
void dibujar_marcador(EstadoJuego *juego) {
    attron(COLOR_PAIR(COLOR_MARCADOR) | A_BOLD);
    mvprintw(0, juego->columnas / 2 - 10, " J1: %d   J2: %d ", juego->puntos1, juego->puntos2);
    attroff(COLOR_PAIR(COLOR_MARCADOR) | A_BOLD);
}


// Coloca la pelota en el centro de la pantalla con una direccion aleatoria
void reiniciar_pelota(EstadoJuego *juego) {
    juego->pelota_x = juego->columnas / 2.0f;
    juego->pelota_y = juego->filas / 2.0f;

    // elegir direccion aleatoria para que no sea siempre igual
    juego->vel_x = (rand() % 2 == 0) ?  1.5f : -1.5f;
    juego->vel_y = (rand() % 2 == 0) ?  1.0f : -1.0f;
}
