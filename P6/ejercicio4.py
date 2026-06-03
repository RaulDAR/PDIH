import cv2
import sys
 
video_path = sys.argv[1] if len(sys.argv) > 1 else "video_input.mp4"
 
modelo_cara = cv2.CascadeClassifier("haarcascade_frontalface_alt.xml")
modelo_cuerpo = cv2.CascadeClassifier("haarcascade_fullbody.xml")
cap = cv2.VideoCapture(video_path)
 
fps = cap.get(cv2.CAP_PROP_FPS)
w = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
h = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
out = cv2.VideoWriter("resultado_ej4.mp4", cv2.VideoWriter_fourcc(*"mp4v"), fps, (w, h))
 
frame_n = 0
while True:
    ret, frame = cap.read()
    if not ret:
        break
    gris = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
 
    caras = modelo_cara.detectMultiScale(gris, scaleFactor=1.05, minNeighbors=6)
    for (x, y, fw, fh) in caras:
        cv2.rectangle(frame, (x, y), (x+fw, y+fh), (0, 0, 255), 3)
        cv2.putText(frame, "Cara", (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
 
    cuerpos = modelo_cuerpo.detectMultiScale(gris, scaleFactor=1.02, minNeighbors=2)
    for (x, y, fw, fh) in cuerpos:
        cv2.rectangle(frame, (x, y), (x+fw, y+fh), (255, 0, 0), 3)
        cv2.putText(frame, "Cuerpo", (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 0, 0), 2)
 
    out.write(frame)
    frame_n += 1
    if frame_n % 30 == 0:
        print(f"Frame {frame_n} | Caras: {len(caras)} | Cuerpos: {len(cuerpos)}")
 
cap.release()
out.release()
print("Guardado: resultado_ej4.mp4")
