#  Decodificador de Código Morse con LCD

Este proyecto implementa un sistema de traducción de código Morse a caracteres alfanuméricos en tiempo real utilizando un Arduino Uno. El sistema permite al usuario introducir señales mediante un pulsador y visualizar la traducción directamente en una pantalla LCD.

##  Características
* **Interpretación Inteligente:** Diferencia entre puntos y rayas según la duración de la pulsación.
* **Feedback Dual:** El sistema emite un pitido (buzzer) y una señal lumínica (LED) sincronizados con la pulsación.
* **Pantalla Dinámica:** Muestra la secuencia de símbolos en curso y el carácter final obtenido.
* **Diccionario Completo:** Incluye soporte para el abecedario (A-Z) y números (0-9).

##  Hardware Utilizado
* **Arduino Uno R3**
* **Pantalla LCD 16x2**
* **Pulsador (Push button)**
* **Piezo Buzzer**
* **LED Rojo**
* **Potenciómetro 10kΩ** (Contraste del LCD)
* **Resistencias:** 220Ω (LED y LCD)
* **Protoboard y cables de conexión**



##  Conexiones (Pinout)

| Componente | Pin Arduino | Función |
| :--- | :--- | :--- |
| **LCD RS** | **D12** | Register Select |
| **LCD Enable** | **D11** | Enable signal |
| **LCD D4, D5, D6, D7** | **D5, D4, D3, D6** | Bus de datos |
| **Pulsador** | **D2** | Entrada (INPUT_PULLUP) |
| **LED** | **D8** | Salida Digital |
| **Buzzer** | **D9** | Salida PWM (Tono 800Hz) |

<img width="901" height="483" alt="image" src="https://github.com/user-attachments/assets/9a9d61e3-9cdd-408f-b3fa-4b9a08f94fee" />


##  Funcionamiento y Tiempos

El código gestiona la lógica Morse mediante el cálculo de duraciones con la función `millis()`, evitando el bloqueo del programa:

1.  **Entrada de Señal:**
    * **Punto (`.`):** Pulsación inferior a 200 ms.
    * **Raya (`-`):** Pulsación superior a 200 ms.
2.  **Procesamiento:** * Al soltar el botón, se acumula el símbolo en una cadena de texto (*String*).
    * Si el sistema detecta un silencio (botón en alto) superior a **1 segundo**, se dispara la función de decodificación.
3.  **Decodificación:** * Se utiliza un `struct` para comparar la secuencia obtenida con una tabla de búsqueda.
    * Si la secuencia es válida, se imprime la letra en la fila inferior del LCD durante 2.5 segundos.
    * Si no existe coincidencia, se muestra un símbolo de interrogación (`?`).

##  Configuración del Software
El código utiliza la librería estándar `<LiquidCrystal.h>`, por lo que no es necesario instalar dependencias externas. Asegúrate de ajustar el potenciómetro de contraste si no visualizas el texto al iniciar el circuito.
