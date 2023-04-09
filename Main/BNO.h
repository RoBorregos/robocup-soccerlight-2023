#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class BNO {
  private:
    double yaw;
    bool right = true;
    Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
    double offset = 0;
    double mag = 0;

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
      sensors_event_t magnetometerData;
      
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_QUATERNION);
      bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);

      mag = (double)magnetometerData.magnetic.y;
      
      yaw = (double)orientationData.orientation.x;
      yaw = (yaw > 180) ? -1*(360-yaw) : yaw;
      //yaw *= -1;

      if (offset > 0) {
        yaw = (yaw > 0) ? (180-yaw)*-1 : (-180-yaw)*-1;
      }
      
      right = (yaw < 0) ? true : false;


    }

    
    double getYaw(){
      return yaw;
    }

    double getMag() {
      return mag;
    }

    void setOffset(double off) {
      offset = off;
    }

    bool isRight() {
      return right;
    }

};
