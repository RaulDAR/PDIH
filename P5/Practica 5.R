#install.packages(c("tuneR", "seewave"), dependencies = TRUE) # Esta línea hay que usarla 
#la primera vez para instalar los paquetes necesarios para la práctica


library(tuneR)
library(seewave)

# 1. Creo los WAVs a partir de los MP3 que grabé

audio_aux <- readMP3("nombre.MP3")
writeWave(audio_aux, "nombre.wav")

audio_aux <- readMP3("apellido.MP3")
writeWave(audio_aux, "apellido.wav")

# 2. Leo y ploteo los dos ficheros de audio

nombre <- readWave("nombre.wav")
apellido <- readWave("apellido.wav")

par(mfrow = c(2,1))  # Igual que en matlab para poner dos gráficas a la vez
plot(nombre, main = "Nombre")
plot(apellido, main = "Apellido")
par(mfrow = c(1,1))

# 3. Info de las cabeceras de los audios
str(nombre)
str(apellido)

# 4 y 5. Unimos los dos audios, los ploteo y reproduzco
combined <- pastew(apellido, nombre, 48000, output = "Wave")
plot(combined, main = "Nombre + Apellido")
listen(combined, 48000)

# 6. Se guarda el fichero de audio combinado
writeWave(combined, "basico.wav")

#7. Filtro de la señal para eliminar frecuencias
basico <- readWave("basico.wav")
filtrado <- bwfilter(basico, 48000, channel = 1, n =   4, from = NULL, to = 10000, bandpass = TRUE, output = "Wave")
# la linea de arriba deja las frecuencias de 0 a 10k hercios, es decir, un paso bajo. Filtro de orden 4 (n = 4)
writeWave(filtrado, "filtrado.wav")
listen(filtrado, 48000)

# 8. Eco y reversa
# Generar el eco (devuelve un vector) (si lo hago sin el vector no me funciona)
eco_vector <- echo(basico, 48000, amp = c(0.8, 0.6, 0.4, 0.2, 0.1), delay = c(1, 2, 3, 4, 5), output="vector")

# Convertirlo a formato Wave para poder guardarlo y plotearlo
eco_wave <- Wave(left = eco_vector, 48000, 16)

writeWave(eco_wave, "eco.wav")
listen(eco_wave, 48000)
plot(eco_wave)
# Tuve que hacer todos estos pasos porque me estaba dando error y no sonaba nada al intentar hacer el eco


alreves <- revw(basico, output = "Wave")
writeWave(alreves, "alreves.wav")
plot(alreves)
plot(basico)
listen(alreves, 48000)

