# Vision - By: alecoeto - mié feb 1 2023

import sensor, image, time, math, pyb, utime
from pyb import UART

#Thresholds (LAB)
#amarillo = ((51, 81, -34, 4, 15, 84))     #Amarillo
amarillo = (57, 69, 24, 67, 43, 94)
#azul = (14, 53, 1, 40, -74, -15)       #Azul
azul = (14, 45, 22, 56, -63, -15)

blue_led = pyb.LED(3)
red_led = pyb.LED(1)


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

clock = time.clock()
uart = UART(3, 9600, timeout_char=0)     #Comunicación serial por medio de UART

#Funcion para enviar cordenadas de blobs detectadas
def detectar_porteria(color, tag):
    count = 0
    area = 0
    x = -1
    y = -1
    h = -1
    w = -1




    for blob in img.find_blobs([color], pixels_threshold=200, area_threshold=300, merge=True):
            img.draw_rectangle(blob.rect(), color=(255,255,0))
            img.draw_cross(blob.cx(), blob.cy(), color=(255,255,0))
            img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
            red_led.on()

            if count == 0:
                x = str(blob.cx())
                y = str(blob.cy())
                w = str(blob.w())
                h = str(blob.h())
            count += 1

            if blob.area() > area:
                x = str(blob.cx())
                y = str(blob.cy())
                w = str(blob.w())
                h = str(blob.h())

            area = max(blob.area(), area)

    uart.write(f"{tag},{x},{y},{w},{h}\n")
    print(f"{tag},{x},{y},{w},{h}\n")

#Loop
while(True):
    clock.tick()
    img = sensor.snapshot()
    detectar_porteria(amarillo, 'a')
    detectar_porteria(azul, 'b')
    #red_led.off()

    #if (uart.any() > 0):
        #txt = uart.readline()
        #if (txt == b'0'):
            #detectar_porteria(amarillo, 'a')
        #elif (txt == b'1'):
            #detectar_porteria(azul, 'b')













