# Práctica 6 - Reconocimiento de Imágenes

Uso de OpenCV con clasificadores Haar Cascade para detección facial, corporal y de animales en imágenes y vídeo.

---

## Requisitos

```bash
pip3 install opencv-python-headless
```

Clasificadores XML necesarios (colocar en el mismo directorio que los scripts):

```bash
# Ya incluido en OpenCV o descargable:
wget https://raw.githubusercontent.com/opencv/opencv/master/data/haarcascades/haarcascade_frontalface_alt.xml
wget https://raw.githubusercontent.com/opencv/opencv/master/data/haarcascades/haarcascade_frontalcatface.xml
wget https://raw.githubusercontent.com/opencv/opencv/master/data/haarcascades/haarcascade_fullbody.xml
```

---

## Ejercicio 1 – Detección de caras en imágenes

**Script:** `ej1_caras_imagen.py`

Detecta rostros humanos en una fotografía usando `haarcascade_frontalface_alt.xml`. Dibuja un rectángulo rojo sobre cada cara detectada y guarda el resultado como imagen.

**Uso:**
```bash
python3 ej1_caras_imagen.py foto.jpg
```

**Salida:** `resultado_ej1.jpg`

**Parámetros clave:**
- `scaleFactor=1.05` → más preciso, algo más lento
- `minNeighbors=6` → reduce falsos positivos

---

## Ejercicio 2 – Detección de caras en vídeo

**Script:** `ej2_caras_video.py`

Aplica el mismo detector del ejercicio 1 frame a frame sobre un vídeo. Genera un nuevo vídeo con los rectángulos pintados.

**Uso:**
```bash
python3 ej2_caras_video.py video.mp4
```

**Salida:** `resultado_ej2.mp4`

---

## Ejercicio 3 – Detección de caras de gatos en vídeo

**Script:** `ej3_gatos_video.py`

Detecta caras de gatos en vídeo usando `haarcascade_frontalcatface.xml`. Dibuja un rectángulo verde con la etiqueta "Gato" sobre cada detección.

**Uso:**
```bash
python3 ej3_gatos_video.py video_gato.mp4
```

**Salida:** `resultado_ej3.mp4`

---

## Ejercicio 4 – Detección simultánea de caras y cuerpos en vídeo

**Script:** `ej4_caras_cuerpos_video.py`

Detecta simultáneamente caras (rectángulo rojo) y cuerpos completos (rectángulo azul) en cada frame del vídeo, usando dos clasificadores en paralelo.

**Uso:**
```bash
python3 ej4_caras_cuerpos_video.py video.mp4
```

**Salida:** `resultado_ej4.mp4`

---

## Conclusiones

### Ejercicios 1, 2 y 3
Los tres programas funcionaron correctamente. La detección de caras humanas (imágenes y vídeo) y de caras de gatos resultó fiable con los parámetros utilizados. Ajustar `scaleFactor` más cercano a `1.0` mejora la detección de caras pequeñas o parcialmente visibles, a costa de mayor tiempo de procesado.

### Ejercicio 4 – Problemas con la detección de cuerpos

La detección simultánea de caras y cuerpos presentó resultados deficientes en la parte de cuerpos completos:

- **Falsos positivos frecuentes:** `haarcascade_fullbody.xml` detecta como "cuerpo" zonas de la imagen que no lo son (fondos, objetos, áreas con contraste similar a una silueta humana).
- **Limitaciones del clasificador:** este cascade fue entrenado para detectar siluetas humanas completas y de frente, por lo que falla con cuerpos parciales, en movimiento, girados o con ropa de colores que generen poco contraste.
- **Sensibilidad excesiva:** con `minNeighbors` bajo genera muchas detecciones erróneas; con valores altos deja de detectar cuerpos reales.

Estas limitaciones son inherentes al algoritmo de Viola & Jones aplicado a la detección corporal: el clasificador `haarcascade_fullbody` es notoriamente menos robusto que los clasificadores faciales, que tienen décadas más de refinamiento. Alternativas más fiables para detección corporal serían HOG+SVM (`cv2.HOGDescriptor`) o modelos basados en deep learning como YOLO o MobileNet SSD.
