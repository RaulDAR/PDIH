import cv2
import sys
 
video_path = sys.argv[1] if len(sys.argv) > 1 else "video_input.mp4"
 
modelo = cv2.CascadeClassifier("haarcascade_frontalface_alt.xml")
cap = cv2.VideoCapture(video_path)
 
fps = cap.get(cv2.CAP_PROP_FPS)
w = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
h = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
out = cv2.VideoWriter("resultado_ej2.mp4", cv2.VideoWriter_fourcc(*"mp4v"), fps, (w, h))
 
frame_n = 0
while True:
    ret, frame = cap.read()
    if not ret:
        break
    gris = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    rostros = modelo.detectMultiScale(gris, scaleFactor=1.05, minNeighbors=6)
    for (x, y, fw, fh) in rostros:
        cv2.rectangle(frame, (x, y), (x+fw, y+fh), (0, 0, 255), 3)
    out.write(frame)
    frame_n += 1
    if frame_n % 30 == 0:     # cada 30 frames procesados manda un mensaje por consola para saber que no se quedó colgado el programa
        print(f"Frame {frame_n} procesado...")
 
cap.release()
out.release()
print("Guardado: resultado_ej2.mp4")
 
