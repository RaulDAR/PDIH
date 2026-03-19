// Compilar: bc main.c desde la carpeta P1
#include <dos.h>

#define BYTE unsigned char

// Variables para la funcion donde cambia de tamaño el cursor

#define CURSOR_INVISIBLE 0
#define CURSOR_NORMAL    1
#define CURSOR_GRUESO    2

//Colores para pintar las letras y el fondo

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

//Colores para la funcion setvideomode()

#define VIDEO_TEXT_40x25_COLOR   0x01
#define VIDEO_TEXT_80x25_COLOR   0x03
#define VIDEO_TEXT_80x25_MONO    0x07
#define VIDEO_GRAPH_320x200_4    0x04
#define VIDEO_GRAPH_320x200_16   0x0D
#define VIDEO_GRAPH_640x200_16   0x0E
#define VIDEO_GRAPH_640x480_16   0x12
#define VIDEO_GRAPH_320x200_256  0x13

//Margenes de pantalla

#define ESQ_SUP_IZQ  218
#define ESQ_SUP_DER  191
#define ESQ_INF_IZQ  192
#define ESQ_INF_DER  217
#define BORDE_HORIZ  196
#define BORDE_VERT   179


BYTE color_actual = 0x07;

//Funciones que voy a crear

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
void escribe(int x, int y, char *s);



// Funcion para escribir una cadena en la posicion x, y.

void escribe(int x, int y, char *s)
{
    int j;
    gotoxy(x, y);
    for (j = 0; s[j] != '\0'; j++){
        cputchar(s[j]);
	}
}

//Usa la funcion 02 despues de asignar los valores necesarios a los registros de datos

void gotoxy(int x, int y)
{
    union REGS inregs, outregs;
    inregs.h.ah = 0x02;     // Se asigna al registro ah la función a realizar
    inregs.h.dh = (BYTE)y;	//En los registros dh y d1 se ponen las coordenadas (x, y)
    inregs.h.dl = (BYTE)x;	
    inregs.h.bh = 0x00;		//Subfuncion 0 en el registro bh
    int86(0x10, &inregs, &outregs);		//se llama a la función int86() pasandole los registros como datos.
}

//Usa la funcion 01

void setcursortype(int tipo)
{
    union REGS inregs, outregs;
    inregs.h.ah = 0x01;
    switch (tipo) {
        case CURSOR_INVISIBLE:
            inregs.h.ch = 0x20;
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

   // Funcion 00.
 
void setvideomode(BYTE modo)
{
    union REGS inregs, outregs;
    inregs.h.ah = 0x00;
    inregs.h.al = modo;
    int86(0x10, &inregs, &outregs);
}

//Funcion 0F.
   
BYTE getvideomode(void)
{
    union REGS inregs, outregs;
    inregs.h.ah = 0x0F;
    int86(0x10, &inregs, &outregs);
    return outregs.h.al;
}

//Funcion 0F.

void textcolor(int color)
{
    color_actual = (color_actual & 0xF0) | (BYTE)(color & 0x0F);
}

//Funcion 0F.

void textbackground(int color)
{
    color_actual = (color_actual & 0x0F) | (BYTE)((color & 0x0F) << 4);
}


//Funcion 06, subfuncion 0
void clrscr(void)
{
    union REGS inregs, outregs;
    inregs.h.ah = 0x06;
    inregs.h.al = 0x00;
    inregs.h.bh = color_actual;
    inregs.h.ch = 0x00;
    inregs.h.cl = 0x00;
    inregs.h.dh = 24;
    inregs.h.dl = 79;
    int86(0x10, &inregs, &outregs);
    gotoxy(0, 0);
}

//primero funcion 03 despues funcion 09.

void cputchar(char c)
{
    union REGS inregs, outregs;
    BYTE fila, col;

    inregs.h.ah = 0x03;
    inregs.h.bh = 0x00;
    int86(0x10, &inregs, &outregs);
    fila = outregs.h.dh;
    col  = outregs.h.dl;

    inregs.h.ah = 0x09;
    inregs.h.al = (BYTE)c;
    inregs.h.bl = color_actual;
    inregs.h.bh = 0x00;
    inregs.x.cx = 1;
    int86(0x10, &inregs, &outregs);

    col++;
    if (col > 79) {
        col = 0;
        fila++;
        if (fila > 24) fila = 24;
    }
    gotoxy((int)col, (int)fila);
}

// Funcion 00

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

// Funcion 0C

void pixel(int x, int y, BYTE color)
{
		union REGS inregs, outregs;
		inregs.h.ah = 0x0C;
		inregs.h.al = color;
		inregs.x.cx = (unsigned int)x;
		inregs.x.dx = (unsigned int)y;
		int86(0x10, &inregs, &outregs);
}

// Ejercicio extra usando la funcion gotoxy() de antes

void drawbox(int x1, int y1, int x2, int y2, int fg, int bg)
{
		int i;
		BYTE color_guardado = color_actual;

		textcolor(fg);
		textbackground(bg);

		gotoxy(x1, y1); 
		cputchar((char)ESQ_SUP_IZQ);
		for (i = x1+1; i < x2; i++) { 
			gotoxy(i, y1); 
			cputchar((char)BORDE_HORIZ); 
		}
		gotoxy(x2, y1); 
		cputchar((char)ESQ_SUP_DER);

		for (i = y1+1; i < y2; i++) {
				gotoxy(x1, i); 
				cputchar((char)BORDE_VERT);
				gotoxy(x2, i); 
				cputchar((char)BORDE_VERT);
		}

		gotoxy(x1, y2); 
		cputchar((char)ESQ_INF_IZQ);
		for (i = x1+1; i < x2; i++) { 
			gotoxy(i, y2); 
			cputchar((char)BORDE_HORIZ); 
		}
		gotoxy(x2, y2); 
		cputchar((char)ESQ_INF_DER);

		color_actual = color_guardado;
}

// funcino 00 para pausar la ejecucion durante la ejecucion del programa

void mi_pausa(void)
{
		union REGS inregs, outregs;
		inregs.h.ah = 0x00;
		int86(0x16, &inregs, &outregs);
}

// Main del programa, es decir, el programa principal

int main(void)
{
		demo_texto();
		demo_grafico();
		demo_ascii_art();
		return 0;
}

//Esta funcion se usa en el main para probar las funciones que cree antes relacionadas con el texto y el cursor

void demo_texto(void)
{
		int i, j, k;
		BYTE modo;
		int colores[8];		//Voy a ptrobar 8 colores distintos de letra y fondo
		int fondos[8];
		char *nombres[8];
		char *nf[8];

		//Si se quiere cambiar el color de las letras y fondos de la demo hay que cambiar el valor de las siguientes variables

		colores[0]=WHITE;  
		colores[1]=YELLOW;  
		colores[2]=LIGHTGREEN;
		colores[3]=LIGHTCYAN; 
		colores[4]=LIGHTRED; 
		colores[5]=LIGHTMAGENTA;
		colores[6]=LIGHTBLUE; 
		colores[7]=LIGHTGRAY;

		nombres[0]="WHITE"; 
		nombres[1]="YELLOW"; 
		nombres[2]="LGREEN";
		nombres[3]="LCYAN"; 
		nombres[4]="LRED";   
		nombres[5]="LMAG";
		nombres[6]="LBLUE"; 
		nombres[7]="LGRAY";

		fondos[0]=BLACK; 
		fondos[1]=BLUE;    
		fondos[2]=GREEN;
		fondos[3]=CYAN;  
		fondos[4]=RED;     
		fondos[5]=MAGENTA;
		fondos[6]=BROWN; 
		fondos[7]=LIGHTGRAY;

		nf[0]=" BLK "; 
		nf[1]=" BLU "; 
		nf[2]=" GRN ";
		nf[3]=" CYN "; 
		nf[4]=" RED ";
		nf[5]=" MAG ";
		nf[6]=" BRN "; 
		nf[7]=" GRY ";

		setvideomode(VIDEO_TEXT_80x25_COLOR);
		modo = getvideomode();

		textcolor(WHITE);
		textbackground(BLACK);

		clrscr();  //limpia la ventana de dosbox

		//Titulo 
		textcolor(YELLOW);
		textbackground(BLUE);
		escribe(15, 1, "=== DEMO LIBRERIA MI_IO ===");

		//Modo de video actual
		textcolor(LIGHTCYAN);
		textbackground(BLACK);
		escribe(2, 3, "Modo de video actual: 0x");
		cputchar("0123456789ABCDEF"[(modo >> 4) & 0xF]);
		cputchar("0123456789ABCDEF"[modo & 0xF]);

		//Color de texto
		textbackground(BLACK);
		for (j = 0; j < 8; j++) {
				gotoxy(2 + j*10, 5);
				textcolor(colores[j]);
				for (k = 0; nombres[j][k] != '\0'; k++)
						cputchar(nombres[j][k]);
		}

		//Color del fondo
		for (j = 0; j < 8; j++) {
				gotoxy(2 + j*10, 7);
				textcolor(WHITE);
				textbackground(fondos[j]);
				for (k = 0; nf[j][k] != '\0'; k++)
						cputchar(nf[j][k]);
		}
		textbackground(BLACK);

		//Pruebas del cursor
		textcolor(LIGHTGREEN);
		escribe(2, 10, "Prueba setcursortype()");

		textcolor(WHITE);
		escribe(2, 12, "Cursor INVISIBLE (pulsa tecla)...");
		setcursortype(CURSOR_INVISIBLE);
		mi_pausa();

		escribe(2, 13, "Cursor GRUESO (pulsa tecla)...");
		setcursortype(CURSOR_GRUESO);
		mi_pausa();

		escribe(2, 14, "Cursor NORMAL (pulsa tecla)...");
		setcursortype(CURSOR_NORMAL);
		mi_pausa();

		//prueba de la funcion getche()
		textcolor(LIGHTRED);
		escribe(2, 16, "Prueba getche()");
		textcolor(WHITE);
		escribe(2, 18, "Escribe 5 teclas: ");
		for (i = 0; i < 5; i++) getche();

		/* Demo drawbox */
		textcolor(LIGHTMAGENTA);
		escribe(2, 20, "Prueba drawbox()");
		drawbox(10, 21, 70, 23, YELLOW, BLUE);
		textcolor(YELLOW);
		textbackground(BLUE);
		escribe(12, 22, "Recuadro dibujado con drawbox()");
		textbackground(BLACK);

		textcolor(WHITE);
		escribe(2, 24, "Pulsa una tecla para la prueba GRAFICA...");
		mi_pausa();
}

//Al igual que la funcion demo_texto() esta función se usa en el main para probar las funciones de dibujos que hay antes en el programa

void demo_grafico(void)
{
		int x, y, i;
		int cx, cy, r, f, ddF_x, ddF_y, bx, by;

		setvideomode(VIDEO_GRAPH_320x200_4);

		//Lineas horizontales de distintos colores enla parte superior de la pantalla (recorre toda la x pero están fijas en y=10, 20 y 30 respectivamente)
		for (x = 0; x < 320; x++) {
				pixel(x, 10, 1);
				pixel(x, 20, 2);
				pixel(x, 30, 3);
		}

		//Dibujo del rectangulo
		for (x = 50; x <= 150; x++) { 
			pixel(x, 50, 1); pixel(x, 100, 2); 
		}
		for (y = 50; y <= 100; y++) { 
			pixel(50, y, 1); pixel(150, y, 2); 
		}

		//Dibujo del circulo
		cx = 240; 
		cy = 80; 
		r = 40;
		f = 1 - r; 
		ddF_x = 1; 
		ddF_y = -2 * r; 
		bx = 0; 
		by = r;
		pixel(cx, cy+r, 3); 
		pixel(cx, cy-r, 3);
		pixel(cx+r, cy, 3); 
		pixel(cx-r, cy, 3);
		while (bx < by) {
				if (f >= 0) { 
					by--; ddF_y += 2; f += ddF_y; 
				}
				bx++; 
				ddF_x += 2; 
				f += ddF_x;
				pixel(cx+bx, cy+by, 3); 
				pixel(cx-bx, cy+by, 3);
				pixel(cx+bx, cy-by, 3); 
				pixel(cx-bx, cy-by, 3);
				pixel(cx+by, cy+bx, 3); 
				pixel(cx-by, cy+bx, 3);
				pixel(cx+by, cy-bx, 3); 
				pixel(cx-by, cy-bx, 3);
		}

		//Dibujo de la diagonal
		for (i = 0; i < 100; i++) {
			pixel(i, 150 + i/3, 1);
		}

		mi_pausa();
		setvideomode(VIDEO_TEXT_80x25_COLOR);
}

//Funcion a mayores para hacer pixel arts 

void demo_ascii_art(void)
{
		int fila, k;
		char *gato[6];
		char *conejo[4];
		
		gato[0] = "    /\\_/\\  ";
		gato[1] = "   ( o.o ) ";
		gato[2] = "    > ^ <  ";
		gato[3] = "   /|   |\\ ";
		gato[4] = "  (_|   |_)";
		gato[5] = 0;

		conejo[0] = "  (\\(\\    ";
		conejo[1] = "  ( -.-)  ";
		conejo[2] = "  o_(\")(\")";
		conejo[3] = 0;

		textcolor(WHITE);
		textbackground(BLACK);
		clrscr();

		textcolor(YELLOW);
		escribe(20, 1, "ASCII ART");

		textcolor(LIGHTCYAN);
		escribe(5, 4, "Gato");
		for (fila = 0; gato[fila] != 0; fila++) {
				gotoxy(5, 6 + fila);
				textcolor(WHITE);
				for (k = 0; gato[fila][k] != '\0'; k++){
						cputchar(gato[fila][k]);
				}
		}

		textcolor(LIGHTCYAN);
		escribe(25, 4, "Conejo");
		for (fila = 0; conejo[fila] != 0; fila++) {
				gotoxy(25, 6 + fila);
				textcolor(LIGHTGREEN);
				for (k = 0; conejo[fila][k] != '\0'; k++){
						cputchar(conejo[fila][k]);
				}
		}

		drawbox(3, 3, 40, 13, YELLOW, BLACK);

		textcolor(LIGHTRED);
		textbackground(BLACK);
		escribe(5, 15, "Final del la demo ASCII ART. Pulsa una tecla para salir.");

		mi_pausa();
		setcursortype(CURSOR_NORMAL);
		textcolor(WHITE);
		textbackground(BLACK);
		clrscr();
}
