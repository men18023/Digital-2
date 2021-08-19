import serial
import time
from Adafruit_IO import Client, RequestError, Feed

#ubicación de mi plataforma
ADAFRUIT_IO_KEY = "aio_HMvd17oXDtC57piJpa8NsZj7Hnsc"  #contraseña
ADAFRUIT_IO_USERNAME = "men18023"                    #usuario
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

inicial = 255

ser = serial.Serial("COM2", 9600)
ser.timeout = 3 
while True:
    with ser:
        inicial = str(inicial)
        ser.write(inicial.encode('ascii'))
        print("se envio: {}".format(inicial))

        s1_feed = aio.feeds('sensor-1')
        s1_data = aio.receive(s1_feed.key)
        print(f'analog signal: {s1_data.value}')

        valor = int(s1_data.value)
        ser.write(b's')
        var = ser.read(1)
        print(var)

#ser.write(sr1_data)
#respuesta = ser.readline()
#aio.send_data(sr1_feed.key, respuesta)

        s2_feed = aio.feeds('sensor-2')
#aio.send_data(s2_feed.key)
        s2_data = aio.receive(s2_feed.key)
        print(f'digital signal: {s2_data.value}')
        break