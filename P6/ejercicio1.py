import cv2
import sys
 
img_path = sys.argv[1] if len(sys.argv) > 1 else "imagen_input.png"
 
modelo = cv2.CascadeClassifier("haarcascade_frontalface_alt.xml")
img = cv2.imread(img_path)
gris = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
 
rostros = modelo.detectMultiScale(gris, scaleFactor=1.05, minNeighbors=8)
for (x, y, w, h) in rostros:
    cv2.rectangle(img, (x, y), (x+w, y+h), (0, 0, 255), 3)
 
print(f"Rostros detectados: {len(rostros)}")
cv2.imwrite("resultado_ej1.jpg", img)
print("Guardado: resultado_ej1.jpg")
 
