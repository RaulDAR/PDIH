import cv2
import sys
 
video_path = sys.argv[1] if len(sys.argv) > 1 else "Video_gato.mp4"
 
modelo = cv2.CascadeClassifier("haarcascade_frontalcatface.xml")
cap = cv2.VideoCapture(video_path)
 
fps = cap.get(cv2.CAP_PROP_FPS)
w = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
h = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
out = cv2.VideoWriter("resultado_ej3.mp4", cv2.VideoWriter_fourcc(*"mp4v"), fps, (w, h))
 
frame_n = 0
while True:
    ret, frame = cap.read()
    if not ret:
        break
    gris = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gatos = modelo.detectMultiScale(gris, scaleFactor=1.15, minNeighbors=7)
    for (x, y, fw, fh) in gatos:
        cv2.rectangle(frame, (x, y), (x+fw, y+fh), (0, 255, 0), 3)
        cv2.putText(frame, "Gato", (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 0), 2)
    out.write(frame)
    frame_n += 1
    if frame_n % 30 == 0: # Igual que en el ejercicio anterior aviso de los frames procesados
        print(f"Frame {frame_n} procesado...")
 
cap.release()
out.release()
print("Guardado: resultado_ej3.mp4")
