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
      
//      motor1Speed = m1Speed;
//      motor1P1 = m1P1;
//      motor1P2 = m1P2;
//
//      motor2Speed = m2Speed;
//      motor2P1 = m2P1;
//      motor2P2 = m2P2;
//
//      motor3Speed = m3Speed;
//      motor3P1 = m3P1;
//      motor3P2 = m3P2;

    }

    void iniciar(){
      motor1.iniciarMotor();
      motor2.iniciarMotor();
      motor3.iniciarMotor();
//      pinMode(motor1P1, OUTPUT);
//      pinMode(motor1P2, OUTPUT);
//      pinMode(motor1Speed, OUTPUT);
//      
//      pinMode(motor2P1, OUTPUT);
//      pinMode(motor2P2, OUTPUT);
//      pinMode(motor2Speed, OUTPUT);
//      
//      pinMode(motor3P1, OUTPUT);
//      pinMode(motor3P2, OUTPUT);
//      pinMode(motor3Speed, OUTPUT);
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

//    void resetMotors(){
//      digitalWrite(motor1P1, LOW);
//      digitalWrite(motor1P2, LOW);
//    
//      digitalWrite(motor2P1, LOW);
//      digitalWrite(motor2P2, LOW);
//    
//      digitalWrite(motor3P1, LOW);
//      digitalWrite(motor3P2, LOW);
//    }
//    
//    void motor1Adelante(){
//      digitalWrite(motor1P1, HIGH);
//      digitalWrite(motor1P2, LOW);
//    }
//    
//    void motor2Adelante(){
//      digitalWrite(motor2P1, HIGH);
//      digitalWrite(motor2P2, LOW);
//    }
//    
//    void motor3Adelante(){
//      digitalWrite(motor3P1, HIGH);
//      digitalWrite(motor3P2, LOW);
//    }
//    
//    void motor1Atras(){
//      digitalWrite(motor1P1, LOW);
//      digitalWrite(motor1P2, HIGH);
//    }
//    
//    void motor2Atras(){
//      digitalWrite(motor2P1, LOW);
//      digitalWrite(motor2P2, HIGH);
//    }
//    
//    void motor3Atras(){
//      digitalWrite(motor3P1, LOW);
//      digitalWrite(motor3P2, HIGH);
//    }

    
};
