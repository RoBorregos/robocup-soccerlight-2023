#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class BNO {
  private:
    double yaw;
    bool right = true;
    Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

  public:
    BNO() {
    }

    void iniciar() {
      if (!bno.begin()) {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
      }
    }

    void readValues() {
      sensors_event_t orientationData;
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
      yaw = (double)orientationData.orientation.x;
      yaw = (yaw > 180) ? -1*(360-yaw) : yaw;
      yaw *= -1;
      right = (yaw > 0) ? true : false;

     // right = (yaw < 180) ? true : false;
    }
    
    double getYaw(){
      return yaw;
    }

    bool isRight() {
      return right;
    }

};
