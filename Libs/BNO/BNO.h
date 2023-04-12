#ifndef BNO_h
  #define BNO_h
  
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

      BNO();
      void iniciar();
      void readValues();
      double getYaw();
      double getMag();
      void setOffset(double off);
      bool isRight();
      
  };

#endif
