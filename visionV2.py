# Untitled - By: alecoeto - mié feb 15 2023

import image, math, rpc, sensor, struct, pyb

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
red_led = pyb.LED(1)


interface = rpc.rpc_uart_slave(baudrate=115200)

def detectarPorteria(data):
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    thresholds = struct.unpack("<bbbbbb", data)
    img = sensor.snapshot()
    #red_led.off()

    count = 0
    x = 0
    cx = 0

    blobs = img.find_blobs([thresholds],
                                         pixels_threshold=500,
                                         area_threshold=500,
                                         merge=True)

    if not blobs: return bytes() # No detections.
    for blob in blobs:
        img.draw_rectangle(blob.rect(), color=(255,255,0))
        img.draw_cross(blob.cx(), blob.cy(), color=(255,255,0))
        img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
        red_led.on()

        if count == 0:
            area = blob.area()
            x = (blob.x()) #*10//320)
            width = str(blob.w())
            height = str(blob.h())
            cx = (blob.cx())#*10//320)

        count += 1


        if blob.area() > area:
            x = (blob.x())#*10//320)
            cx = (blob.cx())#*10//320)

        area = max(blob.area(), area)

    return struct.pack("<HH", x, cx)


interface.register_callback(color_detection)
interface.loop()

