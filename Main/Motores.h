#pragma once
#include "Motor.h"

class Motores {
  public:
    Motor motor1;
    Motor motor2;
    Motor motor3;
    

    Motores (int m1Speed, int m1P1, int m1P2, int m2Speed, int m2P1, int m2P2, int m3Speed, int m3P1, int m3P2){
      motor1.set(m1Speed, m1P1, m1P2);
      motor2.set(m2Speed, m2P1, m2P2);
      motor3.set(m3Speed, m3P1, m3P2);

    }

    void iniciar(){
      motor1.iniciarMotor();
      motor2.iniciarMotor();
      motor3.iniciarMotor();
    }

    void movimientoLineal(int degree, int velocidad) {
    
        //En base a los grados se definen las velocidades de cada motor
        float m1 = cos(((150-degree) * PI / 180));
        float m2 = cos(((30-degree) * PI / 180));;
        float m3 = cos(((270-degree) * PI / 180));
        int speedA = abs(int(m1*velocidad));
        int speedB = abs(int(m2*velocidad));
        int speedC = abs(int(m3*velocidad));
    
        //Definir la velocidad de cada motor
        analogWrite(motor1.motorSpeed, speedA);
        analogWrite(motor2.motorSpeed, speedB);
        analogWrite(motor3.motorSpeed, speedC);
    
        //Mover motores según la velocidad (positiva o negativa)
        if (m1 >= 0){
            motor1.motorAdelante();
        } else {
            motor1.motorAtras();
        }
        
        if (m2 >= 0){
            motor2.motorAdelante();
        } else {
            motor2.motorAtras();
        } 
        
        if (m3 >= 0){
            motor3.motorAdelante();
        } else {
            motor3.motorAtras();
        }
    }

    void setAllMotorSpeed(int allSpeed){
      analogWrite(motor1.motorSpeed, allSpeed);
      analogWrite(motor2.motorSpeed, allSpeed);
      analogWrite(motor3.motorSpeed, allSpeed);
    }

    void apagarMotores(){
      motor1.apagar();
      motor2.apagar();
      motor3.apagar();
    }
    
    void adelante(){
      apagarMotores();
      motor1.motorAtras();
      motor2.motorAdelante();
    }
    
    void atras(){
      motor1.motorAdelante();
      motor2.motorAtras();
    }

    void giroH(){
      motor1.motorAdelante();
      motor2.motorAdelante();
      motor3.motorAdelante();
    
    }
    
    //Rotación antihoraria
    void giroAH(){
      motor1.motorAtras();
      motor2.motorAtras();
      motor3.motorAtras();
    
    }

    
};
