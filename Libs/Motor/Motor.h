#ifndef Motor_h
  #define Motor_h
  #pragma once
  
  class Motor {
    private:
      int motorSpeed;
      int pin1;
      int pin2;
  
    public:

      Motor();
      void set(int motorSpeed, int pin1, int pin2);
      void iniciarMotor();
      void motorAdelante();
      void motorAtras();
      void apagar();
      int getMotorSpeed();
      int getPin1();
      int getPin2();
          
  };

#endif
