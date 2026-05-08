# PRACTICA 5: EXPERIMENTACION CON EL SISTEMA DE SALIDA DE SONIDO

## 1. DESCRIPCION DEL PROYECTO
Práctica 5. Uso del lenguaje de programación R para trabajar con señales de audio.

## 2. OBJETIVOS
- Identificacion y representacion grafica de la forma de onda de señales sonoras.
- Analisis de la estructura y metadatos de los ficheros de sonido en formato WAV (inspeccion de cabeceras).
- Operacion con parametros de audio mediante tecnicas de union, filtrado y aplicacion de efectos digitales.

## 3. REQUISITOS DEL SISTEMA
Para la correcta ejecucion del codigo fuente adjunto, se requiere el entorno RStudio y la instalacion de las siguientes librerias:

- **tuneR**: Empleada para la lectura, escritura y gestion de archivos WAV y MP3.
- **seewave**: Utilizada para el procesamiento de señales, filtrado y aplicacion de efectos de sonido.

Comando para la instalacion de dependencias:
install.packages(c("tuneR", "seewave"), dependencies = TRUE)

## 4. DESCRIPCION DE LAS TAREAS REALIZADAS

### Procesamiento inicial y analisis
Se han capturado dos señales de audio originales en formato MP3 (correspondientes a nombre y apellido) y se han transformado a formato WAV. Posteriormente, se ha procedido a:
1. Representacion grafica de las formas de onda individuales.
2. Inspeccion de los metadatos de los archivos mediante la funcion str(), analizando la frecuencia de muestreo y la profundidad de bits.

### Edicion y modificacion de señales
1. **Union de audios**: Se han concatenado las señales originales para crear un unico archivo denominado basico.wav.
2. **Filtrado de frecuencia**: Se ha implementado un filtro paso-bajo para eliminar las frecuencias superiores a 10.000 Hz.
3. **Efecto de eco**: Se ha diseñado un efecto de eco multietapa con retardos de hasta 5 segundos y una atenuacion progresiva de la amplitud.
4. **Inversion de señal**: Se ha generado una version en sentido inverso (reversa) de la señal combinada.

## 5. RESULTADOS GRAFICOS

### Formas de onda originales

<img width="1153" height="895" alt="nombre y apellido" src="https://github.com/user-attachments/assets/4e58e591-869a-4f9f-b615-6bed4c87aa72" />

Descripcion: Representacion de las señales nombre.wav y apellido.wav de forma independiente.

### Metadatos de las señales

<img width="579" height="319" alt="image" src="https://github.com/user-attachments/assets/4ea30292-0c87-49a1-b1e2-09eff322aebf" />
Descripción: metadatos de las señales nombre.wav y apellido.wav

### Señal combinada

<img width="1075" height="784" alt="image" src="https://github.com/user-attachments/assets/fbc9d669-c3e0-49e0-be89-f9e70ce66ced" />

Descripcion: Resultado de la concatenacion de las señales originales en basico.wav.

### Analisis de efectos

<img width="1328" height="787" alt="image" src="https://github.com/user-attachments/assets/8294b602-065e-4dbb-bac9-36ac6c5a51c6" />

Descripcion: Representacion de la señal tras la aplicacion del efecto de eco.


<img width="1425" height="775" alt="image" src="https://github.com/user-attachments/assets/edfb9f21-d35b-4743-a3fd-159a09740dbf" />

Descripcion: Comparativa de la señal en modo inverso frente a la señal original.
