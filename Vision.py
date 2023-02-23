# Vision - By: alecoeto - mié feb 1 2023

import sensor, image, time, math, pyb, utime
from pyb import UART

#Thresholds (LAB)
amarillo = (52, 97, -25, 10, 33, 90) #Amarillo
azul = (21, 60, -6, 30, -74, -28)        #Azul


blue_led = pyb.LED(3)
red_led = pyb.LED(1)


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

clock = time.clock()
uart = UART(3, 9600, timeout_char=0)   #Comunicación serial por medio de UART

def detectar_porteria(color, tag):
    count = 0
    area = 0

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

            if tag == 'a':
                print(f"a,{x},{y},{w},{h} \n")
                uart.write(f"0,{x},{y},{w},{h}\n")


            else:
                print(f"b,{x},{y},{w},{h} \n")
                uart.write(f"1,{x},{y},{w},{h}\n")


    if count == 0:
        uart.write("-1,-1,-1,-1,-1\n")
        print("-1")

while(True):
    clock.tick()
    img = sensor.snapshot()
    #red_led.off()

    detectar_porteria(amarillo, 'a')
    detectar_porteria(azul, 'b')







