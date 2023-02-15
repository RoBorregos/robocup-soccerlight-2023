# Vision - By: alecoeto - mié feb 1 2023

import sensor, image, time, math, pyb, utime
from pyb import UART

#Thresholds (LAB)
amarillo = ((52, 97, -25, 10, 33, 90))
azul = ((21, 60, -6, 30, -74, -28))

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

while(True):
    clock.tick()
    img = sensor.snapshot()
    #red_led.off()

    count = 0

    xA = 'a'
    for blob in img.find_blobs([amarillo], pixels_threshold=200, area_threshold=300, merge=True):
        img.draw_rectangle(blob.rect(), color=(255,255,0))
        img.draw_cross(blob.cx(), blob.cy(), color=(255,255,0))
        img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
        red_led.on()

        if count == 0:
            area = blob.area()
            x = str(blob.x()) #*10//320)
            width = str(blob.w())
            height = str(blob.h())
        count += 1


        if blob.area() > area:
            x = str(blob.x())#*10//320)

        area = max(blob.area(), area)
        print("x = ",blob.x())
        print("cx = ",blob.cx())

        print("y = ",blob.y())


    uart.write(x)
    uart.write('\n')
    #if (uart.any()):
        #uart.write(x)
        #colorIn = uart.read()
        #print(colorIn, '\n')

        #if colorIn == b"y":
            #print("si")

    #for blob in img.find_blobs([azul], pixels_threshold=200, area_threshold=300, merge=True):
        #img.draw_rectangle(blob.rect(), color=(0,196,255))
        #img.draw_cross(blob.cx(), blob.cy(), color=(0,196,255))
        #img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
        #red_led.on()
        #x = blob.cx()*10//320
        #uart.write(str(x))


