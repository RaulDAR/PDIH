# Práctica 1 – Entrada/Salida mediante interrupciones BIOS y DOS

Este proyecto implementa una librería básica de entrada y salida utilizando interrupciones de la BIOS y del sistema operativo MS‑DOS. El objetivo de la práctica es comprender cómo funcionaban las rutinas de E/S a bajo nivel y recrear parte de la funcionalidad de la antigua biblioteca conio, pero implementando manualmente cada función mediante llamadas a `int86()`.

El código está pensado para compilarse con Borland C (bcc) dentro de DOSBox.

------------------------------------------------------------
## Cómo compilar

Desde DOSBox, situarse en la carpeta P1 y ejecutar:

bcc main.c  


Esto generará el ejecutable `main.exe`.

------------------------------------------------------------
## Funciones implementadas

El guion exige implementar 10 funciones obligatorias. Todas están incluidas en este programa:

1. gotoxy()  
2. setcursortype()  
3. setvideomode()  
4. getvideomode()  
5. textcolor()  
6. textbackground()  
7. clrscr()  
8. cputchar()  
9. getche()  
10. pixel()

Además, he añadido funciones extra para ampliar la práctica:

- drawbox(): dibuja un recuadro en modo texto usando caracteres CP437.
- escribe(): escribe una cadena completa en una posición concreta.
- demo_ascii_art(): muestra dibujos ASCII.
- demo_grafico(): dibuja líneas, un rectángulo, un círculo y una diagonal en modo gráfico.

------------------------------------------------------------
## Descripción de cada función

### gotoxy(x, y)
Coloca el cursor en la posición indicada (columna x, fila y).  
Usa la interrupción BIOS 10h, función 02h.

### setcursortype(tipo)
Cambia el aspecto del cursor: invisible, normal o grueso.  
Usa la interrupción BIOS 10h, función 01h.

### setvideomode(modo)
Establece el modo de vídeo (texto o gráfico).  
Usa la interrupción BIOS 10h, función 00h.

### getvideomode()
Devuelve el modo de vídeo actual.  
Usa la interrupción BIOS 10h, función 0Fh.

### textcolor(color)
Modifica el color de primer plano almacenado en `color_actual`.

### textbackground(color)
Modifica el color de fondo almacenado en `color_actual`.

### clrscr()
Borra toda la pantalla usando scroll vertical con AL=0.  
Usa la interrupción BIOS 10h, función 06h.

### cputchar(c)
Escribe un carácter en pantalla con el color actual y avanza el cursor.  
Usa las funciones 03h (leer posición) y 09h (escribir carácter) de la BIOS 10h.

### getche()
Lee un carácter del teclado y lo muestra en pantalla.  
Usa la interrupción BIOS 16h, función 00h.

### pixel(x, y, color)
Dibuja un pixel en modo gráfico.  
Usa la interrupción BIOS 10h, función 0Ch.

------------------------------------------------------------
## Programas de demostración incluidos

El `main()` ejecuta tres demostraciones para comprobar el funcionamiento de todas las funciones.

### demo_texto()
Prueba:
- Cambio de colores de texto y fondo
- Cambio del tipo de cursor
- Escritura de caracteres
- Lectura de teclado con getche()
- Dibujar un recuadro con drawbox()

### demo_grafico()
Activa el modo gráfico CGA 320x200 y dibuja:
- Tres líneas horizontales de colores
- Un rectángulo
- Un círculo usando el algoritmo de Bresenham
- Una diagonal

### demo_ascii_art()
Muestra:
- Un gato en ASCII art
- Un conejo en ASCII art
- Un recuadro decorativo
- Mensaje final

------------------------------------------------------------
## Ejecución

Una vez compilado:

main.exe


El programa ejecuta automáticamente las tres demostraciones.  
Cada una espera a que el usuario pulse una tecla antes de continuar.

------------------------------------------------------------
## Conclusión

Esta práctica permite comprender cómo se realizaba la entrada y salida en sistemas antiguos mediante interrupciones BIOS y DOS. Implementar manualmente funciones como gotoxy, clrscr o pixel ayuda a entender cómo funcionaban las rutinas de vídeo y teclado a bajo nivel. Las demostraciones incluidas permiten comprobar visualmente que todas las funciones se comportan correctamente.

-------------------------------------------------------------
## Imágenes del programa

<img width="631" height="371" alt="image" src="https://github.com/user-attachments/assets/50484a7a-b056-4e28-a675-5bcf6cbe7bdc" />  

<img width="618" height="398" alt="image" src="https://github.com/user-attachments/assets/5bca73b8-992e-4e51-a22f-542314308974" />

<img width="537" height="321" alt="image" src="https://github.com/user-attachments/assets/d31c9c6e-88ac-4820-aa4b-a30171d1c6e5" />



