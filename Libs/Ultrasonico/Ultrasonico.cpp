  
#include "Arduino.h"
#include "Ultrasonico.h"
#include  "SingleEMAFilterLib.h"  


SingleEMAFilter<double> filterDistancia(0.6);
Ultrasonico::Ultrasonico(int trig, int echo) {
    trigPin = trig;
    echoPin = echo;

}

void Ultrasonico::iniciar() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

double Ultrasonico::getDistancia(){
    digitalWrite(trigPin, HIGH);
    unsigned long ms3 = micros();

    while ((micros() - ms3) < 10) {
    }

    digitalWrite(trigPin, LOW);

    long t; //timepo que demora en llegar el eco
    long d; //distancia en centimetros
    t = pulseIn(echoPin, HIGH, 10000); //obtenemos el ancho del pulso
    d = t / 59;
    filterDistancia.AddValue(d);

    return filterDistancia.GetLowPass();
}


   




