#include "Arduino.h"
#include "Dribbler.h"
  #include <Servo.h>


Dribbler::Dribbler (int pin) {
    this-> pin = pin;
}

void Dribbler::iniciar() {
    esc.attach(pin);
    delay(2000); 
    esc.writeMicroseconds(800); // set the motor speed to minimum
    delay(1000); // wait for 3 seconds
};

void Dribbler::prender() {
    esc.writeMicroseconds(1000);

    /*for (int i = 800; i <= 1500; i++) { // increase the motor speed from min to max
        esc.writeMicroseconds(i);
        delay(10); // wait for 10ms before changing the speed
        Serial.print("PWM: " );  Serial.println(i);
    }
    delay(5000); // wait for 5 seconds
    for (int i = 2000; i >= 500; i--) { // decrease the motor speed from max to min
        esc.writeMicroseconds(i);
        delay(10); // wait for 10ms before changing the speed
    }
    delay(2000); // wait for 5 seconds
    */
};

void Dribbler::apagar() {
    esc.writeMicroseconds(800); // set the motor speed to minimum
    //delay(2000); // wait for 3 seconds
};