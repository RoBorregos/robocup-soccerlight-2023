#ifndef Motores_h
  #define Motores_h
    
  #pragma once
  #include "Motor.h"
  #include "Arduino.h"
  
  class Motores {
    public:
      Motor motor1;
      Motor motor2;
      Motor motor3;
  
      Motores (int m1Speed, int m1P1, int m1P2, int m2Speed, int m2P1, int m2P2, int m3Speed, int m3P1, int m3P2);
      void iniciar();
      void movimientoLineal(int degree, int velocidad);
      void movimientoLinealCorregido(int degree, int velocidad, int error, bool isRight);
      void setAllMotorSpeed(int allSpeed);
      void apagarMotores();
      void adelante();
      void atras();
      void giroH();
      void giro(int velocidad, bool right);
      void giroAH();
      void pruebaMovimiento(int velocidades);
      void mover1();
      void mover2();
      void mover3();
  
  };

#endif
