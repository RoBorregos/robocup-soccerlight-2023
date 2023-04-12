#include <Servo.h>

Servo ESC; 

void setup(){
ESC.attach(9); //Poner en PWM obligatoriamente
ESC.writeMicroseconds(1000); //velocidad minima es 1000
delay(5000); //esto se puede quitar
}

void loop(){
  ESC.writeMicroseconds(1200); //velocidad minima = 1000 maxima = 2000, en 0 se apaga (en teoría) 
  delay(20); //esto esta de adorno, pero se puede reemplazar por un milis, creo
}
