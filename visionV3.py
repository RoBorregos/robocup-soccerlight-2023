# Multi Color Code Tracking Example
#
# This example shows off multi color code tracking using the OpenMV Cam.
#
# A color code is a blob composed of two or more colors. The example below will
# only track colored objects which have two or more the colors below in them.

import sensor, image, time, math, pyb, utime
from pyb import UART
# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green things. You may wish to tune them...
thresholds = [(11, 47, 18, 80, -89, -30), # Rojo -> index is 0 so code == (1 << 0), # generic_green_thresholds -> index is 1 so code == (1 << 1)
              (56, 98, 26, 127, -42, 3)] # rosa generic_blue_thresholds -> index is 2 so code == (1 << 2)
# Codes are or'ed together when "merge=True" for "find_blobs".

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

clock = time.clock()
uart = UART(3, 9600, timeout_char=0)     #Comunicación serial por medio de UART


# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. "merge=True" must be set to merge overlapping color blobs for color codes.






def detectar_robot(thresholds):
    count = 0
    area = 0
    x = -1
    y = -1
    h = -1
    w = -1

    for blob in img.find_blobs(thresholds, pixels_threshold=100, area_threshold=100, merge=True):
        if blob.code() == 3: # rojo-rosa
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy())
            img.draw_string(blob.x() + 2, blob.y() + 2, "Rojo/rosa")

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

        uart.write(f"2,{x},{y},{w},{h}\n")
        print(f"2,{x},{y},{w},{h}\n")


while(True):
    clock.tick()
    img = sensor.snapshot()
    detectar_robot(thresholds)

    #for blob in img.find_blobs(thresholds, pixels_threshold=100, area_threshold=100, merge=True):
        #if blob.code() == 3: # rojo-rosa
            #img.draw_rectangle(blob.rect())
            #img.draw_cross(blob.cx(), blob.cy())
            #img.draw_string(blob.x() + 2, blob.y() + 2, "Rojo/rosa")
        #if blob.code() == 8: # azul
            #img.draw_rectangle(blob.rect())
            #img.draw_cross(blob.cx(), blob.cy())
            #img.draw_string(blob.x() + 2, blob.y() + 2, "Amarillo")
        #if blob.code() == 4: # g/b code
            #img.draw_rectangle(blob.rect())
            #img.draw_cross(blob.cx(), blob.cy())
            #img.draw_string(blob.x() + 2, blob.y() + 2, "Azul")
        ##if blob.code() == 7: # r/g/b code
            ##img.draw_rectangle(blob.rect())
            ##img.draw_cross(blob.cx(), blob.cy())
            ##img.draw_string(blob.x() + 2, blob.y() + 2, "r/g/b")
    #print(clock.fps())
