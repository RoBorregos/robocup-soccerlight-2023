# Remote Control - As The Remote Device
#
# This script configures your OpenMV Cam as a co-processor that can be remotely controlled by
# another microcontroller or computer such as an Arduino, ESP8266/ESP32, RaspberryPi, and
# even another OpenMV Cam.
#
# This script is designed to pair with "popular_features_as_the_controller_device.py".

import image, network, math, rpc, sensor, struct, tf

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
#sensor.skip_frames(time = 2000)

# The RPC library above is installed on your OpenMV Cam and provides mutliple classes for
# allowing your OpenMV Cam to be controlled over CAN, I2C, SPI, UART, USB VCP, or LAN/WLAN.

# Uncomment the below line to setup your OpenMV Cam for control over UART.
#* baudrate - Serial Baudrate.
#
# NOTE: Master and slave baud rates must match. Connect master tx to slave rx and master rx to
#       slave tx. Finally, both devices must share a common ground.
#
interface = rpc.rpc_uart_slave(baudrate=115200)


# Helper methods used by the call backs below.

def draw_detections(img, dects):
    for d in dects:
        c = d.corners()
        l = len(c)
        for i in range(l): img.draw_line(c[(i+0)%l] + c[(i+1)%l], color = (0, 255, 0))
        img.draw_rectangle(d.rect(), color = (255, 0, 0))

# Remote control works via call back methods that the controller
# device calls via the rpc module on this device. Call backs
# are functions which take a bytes() object as their argument
# and return a bytes() object as their result. The rpc module
# takes care of moving the bytes() objects across the link.
# bytes() may be the micropython int max in size.

# When called returns x, y, w, and h of the largest face within view.
#
# data is unused

def apriltag_detection(data):
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QQVGA)
    tags = sensor.snapshot().find_apriltags()
    if not tags: return bytes() # No detections.
    draw_detections(sensor.get_fb(), tags)
    output_tag = max(tags, key = lambda t: t.w() * t.h())
    return struct.pack("<HHHH", output_tag.cx(), output_tag.cy(), output_tag.id(),
                       int(math.degrees(output_tag.rotation())))

# When called returns a json list of json apriltag objects for all apriltags in view.
#
# data is unused
def all_apriltag_detection(data):
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QQVGA)
    tags = sensor.snapshot().find_apriltags()
    if not tags: return bytes() # No detections.
    draw_detections(sensor.get_fb(), tags)
    return str(tags).encode()




# When called returns the x/y centroid of the largest blob
# within the OpenMV Cam's field-of-view.
#
# data is the 6-byte color tracking threshold tuple of L_MIN, L_MAX, A_MIN, A_MAX, B_MIN, B_MAX.
def color_detection(data):
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    thresholds = struct.unpack("<bbbbbb", data)
    img = sensor.snapshot()
    #red_led.off()

    count = 0

    x = 0
    cx = 0
    for blob in img.find_blobs([thresholds], pixels_threshold=200, area_threshold=300, merge=True):
        img.draw_rectangle(blob.rect(), color=(255,255,0))
        img.draw_cross(blob.cx(), blob.cy(), color=(255,255,0))
        img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
        #red_led.on()

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
        #print("x = ",blob.x())
        #print("cx = ",blob.cx())

        #print("y = ",blob.y())

    return struct.pack("<HH", x, cx)
    #uart.write(x)
    #uart.write('\n')

def color_detection2(data):
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    thresholds = struct.unpack("<bbbbbb", data)

    x = 'a'
    cx = "a"
    count = 0
    area = 0
    blobs = sensor.snapshot().find_blobs([thresholds],
                                         pixels_threshold=500,
                                         area_threshold=500,
                                         merge=True,
                                         margin=20)
    if not blobs: return bytes() # No detections.
    for blob in blobs:
        sensor.get_fb().draw_rectangle(blob.rect(), color=(255,255,0))
        sensor.get_fb().draw_cross(blob.cx(), blob.cy(), color=(255,255,0))
        #red_led.on()

        if count == 0:
            area = blob.area()
            x = (blob.x()) #*10//320)
            cx = (blob.cx())
            width = str(blob.w())
            height = str(blob.h())
        count += 1


        if blob.area() > area:
            x = (blob.x())#*10//320)
            cx = (blob.cx())

        area = max(blob.area(), area)
        #print("x = ",blob.x())
        #print("cx = ",blob.cx())

        #print("y = ",blob.y())


   # uart.write(x)
   # uart.write('\n')
    return struct.pack("<HH", x, cx)



        #sensor.get_fb().draw_rectangle(b.rect(), color = (255, 0, 0))
        #sensor.get_fb().draw_cross(b.cx(), b.cy(), color = (0, 255, 0))
    #out_blob = max(blobs, key = lambda b: b.density())
    #return struct.pack("<HH", out_blob.cx(), out_blob.cy())

# When called returns a jpeg compressed image from the OpenMV
# Cam in one RPC call.
#
# data is unused
def jpeg_snapshot(data):
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    return sensor.snapshot().compress(quality=90).bytearray()

# Register call backs.

#interface.register_callback(face_detection)
#interface.register_callback(person_detection)
#interface.register_callback(qrcode_detection)
#interface.register_callback(all_qrcode_detection)
interface.register_callback(apriltag_detection)
interface.register_callback(all_apriltag_detection)
#interface.register_callback(datamatrix_detection)
#interface.register_callback(all_datamatrix_detection)
#interface.register_callback(barcode_detection)
#interface.register_callback(all_barcode_detection)
interface.register_callback(color_detection)
interface.register_callback(jpeg_snapshot)

# Once all call backs have been registered we can start
# processing remote events. interface.loop() does not return.

interface.loop()
