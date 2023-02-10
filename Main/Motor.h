#pragma once

class Motor {
  public:
    int motorSpeed;
    int pin1;
    int pin2;

     Motor(){
     }
     
     void set(int motorSpeed, int pin1, int pin2){
        this-> motorSpeed = motorSpeed;
        this-> pin1 = pin1;
        this-> pin2 = pin2;
     }

     void iniciarMotor(){
        pinMode(motorSpeed, OUTPUT);
        pinMode(pin1, OUTPUT);
        pinMode(pin2, OUTPUT);
     }

     void motorAdelante(){
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
     }

     void motorAtras(){
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
     }

     void apagar(){
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
     }
};
