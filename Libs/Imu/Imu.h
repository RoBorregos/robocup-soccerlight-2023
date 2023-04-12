#ifndef Imu_h
  #define Imu_h

  #include "ICM_20948.h" // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU
  
  ICM_20948_I2C myICM; // Otherwise create an ICM_20948_I2C object
  #define WIRE_PORT Wire // Your desired Wire port.      
  #define AD0_VAL 1
  
  class Imu {
    private:
      double yaw;
      double pitch;
      double roll;
      bool right = true;
      double offset = 0;
  
    public:
      Imu();
      void iniciar();
      void readValues();
      double getYaw();
      bool isRight();
      double getRoll();
      double getPitch();
      void setOffset(double off);
      
  
  };

#endif
