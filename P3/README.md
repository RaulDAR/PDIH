# Memoria de Prácticas: Control de Sistemas con Arduino

Este repositorio contiene una serie de 6 ejercicios progresivos realizados en **Tinkercad**. Los ejercicios cubren desde el control básico de salidas digitales hasta el uso de sensores ultrasónicos, fotorresistencias y control de potencia con transistores.

##  Índice
1. [Ejercicio 1: Semáforo Secuencial Simple](#ejercicio-1-semáforo-secuencial-simple)
2. [Ejercicio 2: Semáforo con Interrupción por Pulsador](#ejercicio-2-semáforo-con-interrupción-por-pulsador)
3. [Ejercicio 3: Secuencia Estilo "Coche Fantástico"](#ejercicio-3-secuencia-estilo-coche-fantástico)
4. [Ejercicio 4: Sensor de Proximidad con Alerta Sonora](#ejercicio-4-sensor-de-proximidad-con-alerta-sonora)
5. [Ejercicio 2: Control de Iluminación Inteligente (LDR)](#ejercicio-5-control-de-iluminación-inteligente-ldr)
6. [Ejercicio 6: Control de Motor CC con Transistor](#ejercicio-6-control-de-motor-cc-con-transistor-y-pulsador)

---

## Ejercicio 1: Semáforo Secuencial Simple
Consiste en la creación de un ciclo de luces básico (Rojo, Amarillo y Verde) simulando el funcionamiento de un semáforo con tiempos de espera fijos de 1.5 segundos.

###  Hardware
* Arduino Uno R3, 3 LEDs (R, A, V), 3 Resistencias 220Ω.

###  Conexiones
| Componente | Pin Arduino |
| :--- | :--- |
| LED Rojo | D11 |
| LED Amarillo | D12 |
| LED Verde | D13 |

<img width="910" height="281" alt="image" src="https://github.com/user-attachments/assets/569d0905-46b4-4ac5-ab45-67acd5638cc9" />


---

## Ejercicio 2: Semáforo con Interrupción por Pulsador
Evolución del diseño anterior que añade un pulsador. El sistema alterna entre una secuencia automática y un estado de "parada" (Luz Roja) activado manualmente.

###  Hardware
* Los anteriores + 1 Pulsador (configurado como `INPUT_PULLUP`).

###  Funcionamiento
* **Modo Prioridad:** Al pulsar (lectura `LOW`), el semáforo cambia a **Rojo** inmediatamente.
* **Modo Secuencial:** Mientras no se pulse, ejecuta el ciclo normal Rojo -> Amarillo -> Verde.

<img width="918" height="321" alt="image" src="https://github.com/user-attachments/assets/a1ed7d50-a7fa-4d99-a2e0-57068de4e48f" />


---

## Ejercicio 3: Secuencia Estilo "Coche Fantástico"
Implementación de una secuencia de barrido de ida y vuelta utilizando un **array** de pines y bucles `for` para optimizar el código.

###  Hardware
* 4 LEDs Rojos, 4 Resistencias 220Ω.

###  Conexiones
* Pines digitales: **D10, D11, D12, D13**.

###  Funcionamiento
El código recorre el array de leds encendiéndolos uno a uno con un `RETARDO` de 200ms, creando un efecto visual de movimiento simétrico.

<img width="897" height="343" alt="image" src="https://github.com/user-attachments/assets/0932b1a4-8709-41c7-b8a6-5ead1de93280" />


---

## Ejercicio 4: Sensor de Proximidad con Alerta Sonora
Sistema de alarma que mide distancias con un sensor HC-SR04 y emite pitidos cuya frecuencia aumenta a medida que un objeto se acerca (estilo sensor de aparcamiento).

###  Hardware
* Sensor HC-SR04, Piezo Buzzer.

###  Funcionamiento
* **Medición:** Calcula la distancia en cm mediante el tiempo de respuesta del eco.
* **Mapeo:** La función `map()` ajusta las pausas entre pitidos entre 50ms (muy cerca) y 1000ms (lejos).
* **Rango:** Activo hasta los 100 cm.

<img width="892" height="404" alt="image" src="https://github.com/user-attachments/assets/64a5c260-065e-4465-93f1-53a13818fe31" />


---

## Ejercicio 5: Control de Iluminación Inteligente (LDR)
Uso de una fotorresistencia para medir la luz ambiental y ajustar el brillo de un LED de forma inversamente proporcional mediante **PWM**.

###  Hardware
* LDR, Resistencia 10kΩ (divisor), LED Rojo.

###  Conexiones
* **Entrada:** Pin Analógico **A0**.
* **Salida:** Pin PWM **D9**.

###  Funcionamiento
A menor luz detectada en A0, el Arduino aumenta el ciclo de trabajo en el Pin 9, haciendo que el LED brille más en la oscuridad y se apague con luz intensa.


<img width="904" height="354" alt="image" src="https://github.com/user-attachments/assets/0593e41b-a4bd-4cf3-8433-c4fe7972e312" />


---

## Ejercicio 6: Control de Motor CC con Transistor y Pulsador
Control de un motor de corriente continua utilizando un transistor NPN como interruptor de potencia para manejar una fuente externa de 9V.

###  Hardware
* Motor CC, Transistor NPN, Batería 9V, Pulsador.

###  Funcionamiento
* El Arduino no alimenta al motor directamente; envía una señal de control a la **base** del transistor.
* Al presionar el pulsador, el transistor permite el paso de corriente de la batería al motor, activando el giro.

<img width="933" height="336" alt="image" src="https://github.com/user-attachments/assets/d97ab9bd-fb59-45f2-8a61-62b823181317" />


---

##  Conclusión
Esta serie de ejercicios demuestra la versatilidad de Arduino para interactuar con el mundo físico, pasando de lógicas digitales simples a sistemas complejos que integran sensores analógicos, actuadores de potencia y procesamiento de señales en tiempo real.
