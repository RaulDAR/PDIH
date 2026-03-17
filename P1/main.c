/*

Para compilar se usa el comando: bcc main.c

*/

#include <dos.h>

////////////////////////////////////////////////////////////////////////
//TIPOS Y CONSTANTES
////////////////////////////////////////////////////////////////////////
#define BYTE unsigned char

// Tipos de cursor
#define CURSOR_INVISIBLE 0
#define CURSOR_NORMAL    1
#define CURSOR_GRUESO    2

// Colores
#define BLACK        0
#define BLUE         1
#define GREEN        2
#define CYAN         3
#define RED          4
#define MAGENTA      5
#define BROWN        6
#define LIGHTGRAY    7
#define DARKGRAY     8
#define LIGHTBLUE    9
#define LIGHTGREEN   10
#define LIGHTCYAN    11
#define LIGHTRED     12
#define LIGHTMAGENTA 13
#define YELLOW       14
#define WHITE        15
#define BLINK        128

// Modos de video 
#define VIDEO_TEXT_40x25_COLOR   0x01
#define VIDEO_TEXT_80x25_COLOR   0x03
#define VIDEO_TEXT_80x25_MONO    0x07
#define VIDEO_GRAPH_320x200_4    0x04
#define VIDEO_GRAPH_320x200_16   0x0D
#define VIDEO_GRAPH_640x200_16   0x0E
#define VIDEO_GRAPH_640x480_16   0x12
#define VIDEO_GRAPH_320x200_256  0x13

// Caracteres de borde IBM CP437 para drawbox 
#define ESQ_SUP_IZQ  218   /* ┌ */
#define ESQ_SUP_DER  191   /* ┐ */
#define ESQ_INF_IZQ  192   /* └ */
#define ESQ_INF_DER  217   /* ┘ */
#define BORDE_HORIZ  196   /* ─ */
#define BORDE_VERT   179   /* │ */

//////////////////////////////////////////////////////////////////
   //VARIABLE GLOBAL: color actual
   //bits 7-4 = fondo, bits 3-0 = primer plano
   //Valor inicial: WHITE (7) sobre BLACK (0)
//////////////////////////////////////////////////////////////////
BYTE color_actual = 0x07;

//////////////////////////////////////////////////////////////////
//PROTOTIPOS
//////////////////////////////////////////////////////////////////
void gotoxy(int x, int y);
void setcursortype(int tipo);
void setvideomode(BYTE modo);
BYTE getvideomode(void);
void textcolor(int color);
void textbackground(int color);
void clrscr(void);
void cputchar(char c);
int  getche(void);
void pixel(int x, int y, BYTE color);
void drawbox(int x1, int y1, int x2, int y2, int fg, int bg);
void mi_pausa(void);

void demo_texto(void);
void demo_grafico(void);
void demo_ascii_art(void);


//////////////////////////////////////////////////////////////////
//gotoxy()
//Coloca el cursor en la columna x, fila y (base 0).
//INT 10h, funcion 02h.
//////////////////////////////////////////////////////////////////
void gotoxy(int x, int y)
{
    union REGS inregs, outregs;
    inregs.h.ah = 0x02;
    inregs.h.dh = (BYTE)y;
    inregs.h.dl = (BYTE)x;
    inregs.h.bh = 0x00;
    int86(0x10, &inregs, &outregs);
}


//////////////////////////////////////////////////////////////////
//setcursortype()
//Fija el aspecto del cursor: INVISIBLE, NORMAL o GRUESO.
//INT 10h, funcion 01h.
//////////////////////////////////////////////////////////////////
void setcursortype(int tipo)
{
    union REGS inregs, outregs;
    inregs.h.ah = 0x01;
    switch (tipo) {
        case CURSOR_INVISIBLE:
            inregs.h.ch = 0x20;  // bit 5 = ocultar cursor
            inregs.h.cl = 0x00;
            break;
        case CURSOR_GRUESO:
            inregs.h.ch = 0x00;
            inregs.h.cl = 0x07;
            break;
        case CURSOR_NORMAL:
        default:
            inregs.h.ch = 0x06;
            inregs.h.cl = 0x07;
            break;
    }
    int86(0x10, &inregs, &outregs);
}


///////////////////////////////////////////////////////////////////
//setvideomode()
//Establece el modo de video indicado.
//INT 10h, funcion 00h.
//////////////////////////////////////////////////////////////////
void setvideomode(BYTE modo)
{
    union REGS inregs, outregs;
    inregs.h.ah = 0x00;
    inregs.h.al = modo;
    int86(0x10, &inregs, &outregs);
}


//////////////////////////////////////////////////////////////////
//getvideomode()
//Devuelve el modo de video actual.
//INT 10h, funcion 0Fh.
//////////////////////////////////////////////////////////////////
BYTE getvideomode(void)
{
    union REGS inregs, outregs;
    inregs.h.ah = 0x0F;
    int86(0x10, &inregs, &outregs);
    return outregs.h.al;
}


//////////////////////////////////////////////////////////////////
//textcolor()
//Cambia el color de primer plano (bits 3-0 de color_actual).
//////////////////////////////////////////////////////////////////
void textcolor(int color)
{
    color_actual = (color_actual & 0xF0) | (BYTE)(color & 0x0F);
}

//////////////////////////////////////////////////////////////////
//textbackground()
//Cambia el color de fondo (bits 7-4 de color_actual).
//////////////////////////////////////////////////////////////////

void textbackground(int color)
{
    color_actual = (color_actual & 0x0F) | (BYTE)((color & 0x0F) << 4);
}


//////////////////////////////////////////////////////////////////
//clrscr()
//Borra toda la pantalla y coloca el cursor en (0,0).
//INT 10h, funcion 06h con AL=0.
//////////////////////////////////////////////////////////////////

void clrscr(void)
{
    union REGS inregs, outregs;
    inregs.h.ah = 0x06;
    inregs.h.al = 0x00;        // AL=0: borrar zona entera 
    inregs.h.bh = color_actual;
    inregs.h.ch = 0x00;
    inregs.h.cl = 0x00;
    inregs.h.dh = 24;
    inregs.h.dl = 79;
    int86(0x10, &inregs, &outregs);
    gotoxy(0, 0);
}


//////////////////////////////////////////////////////////////////////
//cputchar()
//Escribe el caracter c en la posicion actual del cursor
//con el color de color_actual, y avanza el cursor.
//INT 10h, func 03h (leer pos) + func 09h (escribir char).
///////////////////////////////////////////////////////////////////////
void cputchar(char c)
{
    union REGS inregs, outregs;
    BYTE fila, col;

    // Leer posicion actual 
    inregs.h.ah = 0x03;
    inregs.h.bh = 0x00;
    int86(0x10, &inregs, &outregs);
    fila = outregs.h.dh;
    col  = outregs.h.dl;

    //Escribir caracter con color 
    inregs.h.ah = 0x09;
    inregs.h.al = (BYTE)c;
    inregs.h.bl = color_actual;
    inregs.h.bh = 0x00;
    inregs.x.cx = 1;
    int86(0x10, &inregs, &outregs);

    //Avanzar cursor 
    col++;
    if (col > 79) {
        col = 0;
        fila++;
        if (fila > 24) fila = 24;
    }
    gotoxy((int)col, (int)fila);
}


//////////////////////////////////////////////////////////////////
//getche()
//Lee un caracter del teclado y lo muestra en pantalla.
//INT 16h, funcion 00h.
//////////////////////////////////////////////////////////////////
int getche(void)
{
    union REGS inregs, outregs;
    int caracter;
    inregs.h.ah = 0x00;
    int86(0x16, &inregs, &outregs);
    caracter = (int)outregs.h.al;
    cputchar((char)caracter);
    return caracter;
}


//////////////////////////////////////////////////////////////////
//Dibuja un pixel en (x,y) con el color dado.
//INT 10h, funcion 0Ch.
//////////////////////////////////////////////////////////////////
void pixel(int x, int y, BYTE color)
{
    union REGS inregs, outregs;
    inregs.h.ah = 0x0C;
    inregs.h.al = color;
    inregs.x.cx = (unsigned int)x;
    inregs.x.dx = (unsigned int)y;
    int86(0x10, &inregs, &outregs);
}


//////////////////////////////////////////////////////////////////
//drawbox()  [EXTRA]
//Dibuja un recuadro en modo texto con caracteres CP437.
//x1,y1 = esquina superior izquierda  (columna, fila)
//x2,y2 = esquina inferior derecha    (columna, fila)
//////////////////////////////////////////////////////////////////
void drawbox(int x1, int y1, int x2, int y2, int fg, int bg)
{
    int i;
    BYTE color_guardado = color_actual;

    textcolor(fg);
    textbackground(bg);

    gotoxy(x1, y1); cputchar((char)ESQ_SUP_IZQ);
    for (i = x1 + 1; i < x2; i++) { gotoxy(i, y1); cputchar((char)BORDE_HORIZ); }
    gotoxy(x2, y1); cputchar((char)ESQ_SUP_DER);

    for (i = y1 + 1; i < y2; i++) {
        gotoxy(x1, i); cputchar((char)BORDE_VERT);
        gotoxy(x2, i); cputchar((char)BORDE_VERT);
    }

    gotoxy(x1, y2); cputchar((char)ESQ_INF_IZQ);
    for (i = x1 + 1; i < x2; i++) { gotoxy(i, y2); cputchar((char)BORDE_HORIZ); }
    gotoxy(x2, y2); cputchar((char)ESQ_INF_DER);

    color_actual = color_guardado;
}


//////////////////////////////////////////////////////////////////
//mi_pausa()
//Espera a que el usuario pulse una tecla.
//INT 16h, funcion 00h.
//////////////////////////////////////////////////////////////////
void mi_pausa(void)
{
    union REGS inregs, outregs;
    inregs.h.ah = 0x00;
    int86(0x16, &inregs, &outregs);
}


//////////////////////////////////////////////////////////////////
//MAIN
//////////////////////////////////////////////////////////////////
int main(void)
{
    demo_texto();
    demo_grafico();
    demo_ascii_art();
    return 0;
}


//////////////////////////////////////////////////////////////////
//demo_texto()
//Prueba de las funciones que creé antes
//////////////////////////////////////////////////////////////////
void demo_texto(void)
{
    int i;
    BYTE modo;

    setvideomode(VIDEO_TEXT_80x25_COLOR);
    modo = getvideomode();

    textcolor(WHITE);
    textbackground(BLACK);
    clrscr();

    // Titulo 
    textcolor(YELLOW);
    textbackground(BLUE);
    gotoxy(15, 1);
    { const char *s = "=== DEMO LIBRERIA MI_IO ==="; int j; for(j=0;s[j];j++) cputchar(s[j]); }

    // Modo de video actual 
    textcolor(LIGHTCYAN);
    textbackground(BLACK);
    gotoxy(2, 3);
    { const char *s = "Modo de video actual: 0x"; int j; for(j=0;s[j];j++) cputchar(s[j]); }
    cputchar("0123456789ABCDEF"[(modo >> 4) & 0xF]);
    cputchar("0123456789ABCDEF"[modo & 0xF]);

    // Demo colores de texto 
    {
        int colores[] = {WHITE,YELLOW,LIGHTGREEN,LIGHTCYAN,LIGHTRED,LIGHTMAGENTA,LIGHTBLUE,LIGHTGRAY};
        const char *nombres[] = {"WHITE","YELLOW","LGREEN","LCYAN","LRED","LMAG","LBLUE","LGRAY"};
        int j, k;
        textbackground(BLACK);
        for (j = 0; j < 8; j++) {
            gotoxy(2 + j*10, 5);
            textcolor(colores[j]);
            for (k = 0; nombres[j][k]; k++) cputchar(nombres[j][k]);
        }
    }

    //Demo colores de fondo 
    {
        int fondos[] = {BLACK,BLUE,GREEN,CYAN,RED,MAGENTA,BROWN,LIGHTGRAY};
        const char *nf[] = {" BLK "," BLU "," GRN "," CYN "," RED "," MAG "," BRN "," GRY "};
        int j, k;
        for (j = 0; j < 8; j++) {
            gotoxy(2 + j*10, 7);
            textcolor(WHITE);
            textbackground(fondos[j]);
            for (k = 0; nf[j][k]; k++) cputchar(nf[j][k]);
        }
        textbackground(BLACK);
    }

    // Demo cursor 
    textcolor(LIGHTGREEN);
    gotoxy(2, 10);
    { const char *s = "--- Demo setcursortype() ---"; int j; for(j=0;s[j];j++) cputchar(s[j]); }

    textcolor(WHITE);
    gotoxy(2, 12);
    { const char *s = "Cursor INVISIBLE (pulsa tecla)..."; int j; for(j=0;s[j];j++) cputchar(s[j]); }
    setcursortype(CURSOR_INVISIBLE);
    mi_pausa();

    gotoxy(2, 13);
    { const char *s = "Cursor GRUESO    (pulsa tecla)..."; int j; for(j=0;s[j];j++) cputchar(s[j]); }
    setcursortype(CURSOR_GRUESO);
    mi_pausa();

    gotoxy(2, 14);
    { const char *s = "Cursor NORMAL    (pulsa tecla)..."; int j; for(j=0;s[j];j++) cputchar(s[j]); }
    setcursortype(CURSOR_NORMAL);
    mi_pausa();

    // Demo getche 
    textcolor(LIGHTRED);
    gotoxy(2, 16);
    { const char *s = "--- Demo getche() ---"; int j; for(j=0;s[j];j++) cputchar(s[j]); }
    textcolor(WHITE);
    gotoxy(2, 18);
    { const char *s = "Escribe 5 teclas: "; int j; for(j=0;s[j];j++) cputchar(s[j]); }
    for (i = 0; i < 5; i++) getche();

    // Demo drawbox 
    textcolor(LIGHTMAGENTA);
    gotoxy(2, 20);
    { const char *s = "--- Demo drawbox() [extra] ---"; int j; for(j=0;s[j];j++) cputchar(s[j]); }
    drawbox(10, 21, 70, 23, YELLOW, BLUE);
    gotoxy(12, 22);
    textcolor(YELLOW);
    textbackground(BLUE);
    { const char *s = "Recuadro dibujado con drawbox()!"; int j; for(j=0;s[j];j++) cputchar(s[j]); }
    textbackground(BLACK);

    textcolor(WHITE);
    gotoxy(2, 24);
    { const char *s = "Pulsa una tecla para la demo GRAFICA..."; int j; for(j=0;s[j];j++) cputchar(s[j]); }
    mi_pausa();
}


//////////////////////////////////////////////////////////////////
//demo_grafico()
//Modo CGA 320x200 4 colores. Dibuja lineas, rectangulo,
//circulo (Bresenham) y diagonal usando pixel().
//////////////////////////////////////////////////////////////////
void demo_grafico(void)
{
    int x, y, i;

    setvideomode(VIDEO_GRAPH_320x200_4);

    // Lineas horizontales de color */
    for (x = 0; x < 320; x++) {
        pixel(x, 10, 1);
        pixel(x, 20, 2);
        pixel(x, 30, 3);
    }

    // Rectangulo 
    for (x = 50; x <= 150; x++) { pixel(x, 50, 1); pixel(x, 100, 2); }
    for (y = 50; y <= 100; y++) { pixel(50, y, 1); pixel(150, y, 2); }

    // Circulo 
    {
        int cx = 240, cy = 80, r = 40;
        int f = 1 - r, ddF_x = 1, ddF_y = -2 * r, bx = 0, by = r;
        pixel(cx, cy+r, 3); pixel(cx, cy-r, 3);
        pixel(cx+r, cy, 3); pixel(cx-r, cy, 3);
        while (bx < by) {
            if (f >= 0) { by--; ddF_y += 2; f += ddF_y; }
            bx++; ddF_x += 2; f += ddF_x;
            pixel(cx+bx, cy+by, 3); pixel(cx-bx, cy+by, 3);
            pixel(cx+bx, cy-by, 3); pixel(cx-bx, cy-by, 3);
            pixel(cx+by, cy+bx, 3); pixel(cx-by, cy+bx, 3);
            pixel(cx+by, cy-bx, 3); pixel(cx-by, cy-bx, 3);
        }
    }

    //Diagonal 
    for (i = 0; i < 100; i++) pixel(i, 150 + i/3, 1);

    mi_pausa();
    setvideomode(VIDEO_TEXT_80x25_COLOR);
}


//////////////////////////////////////////////////////////////////
//demo_ascii_art() 
//Dibuja un gato y un conejo en ASCII art con colores.
//////////////////////////////////////////////////////////////////
void demo_ascii_art(void)
{
    const char *gato[] = {
        "    /\\_/\\  ",
        "   ( o.o ) ",
        "    > ^ <  ",
        "   /|   |\\ ",
        "  (_|   |_)",
        (const char *)0
    };
    const char *conejo[] = {
        "  (\\(\\    ",
        "  ( -.-)  ",
        "  o_(\")(\")",
        (const char *)0
    };
    int fila, k;

    textcolor(WHITE);
    textbackground(BLACK);
    clrscr();

    textcolor(YELLOW);
    gotoxy(20, 1);
    { const char *s = "=== ASCII ART [extra] ==="; int i; for(i=0;s[i];i++) cputchar(s[i]); }

    textcolor(LIGHTCYAN);
    gotoxy(5, 4);
    { const char *s = "-- Gato --"; int i; for(i=0;s[i];i++) cputchar(s[i]); }
    for (fila = 0; gato[fila] != (const char *)0; fila++) {
        gotoxy(5, 6 + fila);
        textcolor(WHITE);
        for (k = 0; gato[fila][k]; k++) cputchar(gato[fila][k]);
    }

    textcolor(LIGHTCYAN);
    gotoxy(25, 4);
    { const char *s = "-- Conejo --"; int i; for(i=0;s[i];i++) cputchar(s[i]); }
    for (fila = 0; conejo[fila] != (const char *)0; fila++) {
        gotoxy(25, 6 + fila);
        textcolor(LIGHTGREEN);
        for (k = 0; conejo[fila][k]; k++) cputchar(conejo[fila][k]);
    }

    drawbox(3, 3, 40, 13, YELLOW, BLACK);

    textcolor(LIGHTRED);
    textbackground(BLACK);
    gotoxy(5, 15);
    { const char *s = "Fin practica. Pulsa una tecla para salir."; int i; for(i=0;s[i];i++) cputchar(s[i]); }

    mi_pausa();
    setcursortype(CURSOR_NORMAL);
    textcolor(WHITE);
    textbackground(BLACK);
    clrscr();
}
