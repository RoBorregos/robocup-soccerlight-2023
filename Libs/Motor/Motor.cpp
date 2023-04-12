#include "Arduino.h"
#include "Motor.h"

Motor::Motor() {};

//Set
void Motor::set(int motorSpeed, int pin1, int pin2) {
  this-> motorSpeed = motorSpeed;
  this-> pin1 = pin1;
  this-> pin2 = pin2;
}

void Motor::iniciarMotor() {
  //pinMode(motorSpeed, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
}

void Motor::motorAdelante() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
}

void Motor::motorAtras() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
}

void Motor::apagar() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}

//Getters
int Motor::getMotorSpeed() {
  return motorSpeed;
}

int Motor::getPin1() {
  return pin1;
}

int Motor::getPin2() {
  return pin2;
}
