#include "Motores.h"
//GND --> GND
//P4 --> RX0

 
//Librerías
#include <HTInfraredSeeker.h>
#include <openmvrpc.h>
#include "Porteria.h"
//#include "Camara.h"


//Variables
String colorPorteria = "y";
bool posesion = true;
int velocidades = 160;
String input = "a";
char lastSeen = 'i';
int last = 1;
const int sensorPin = 40;
int count = 0;
int threshold = 300;



//LEDs
int ledRojo = 2;
int ledVerde = 3;


//Motores
Motores motoresRobot(8,24,25,9,26,27,10,22,23);

Porteria porteria;


 
//SETUP------------------------------------------------------
void setup() {
  Serial3.begin(9600);
  Serial.begin(9600);

  //Motores
  motoresRobot.iniciar();

  //LED
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  
  //Seeker
  InfraredSeeker::Initialize();

}

//LOOP-------------------------------------------------------
void loop() {

  //int value = 0;
  //value = digitalRead(sensorPin);  //lectura digital de pin
  
//  if (value == LOW) {
//      posesion = true;
//
//  } else {
//    count++;
//    //posesion = false;
//  }
//  
//  
//  if (count == threshold-1){
//    posesion = false;
//  }
//
//  if (count == threshold){ 
//    count = 0;
//  }
  
   // Serial.println(posesion);

  //Lectura de la cámara
    if (Serial3.available()) {   
       // Serial3.write("y");

       input =  Serial3.readStringUntil('\n');
       porteria.actualizar(input);
    }
    
  
  //Verificar si se debe buscar la pelota o portería
  if (posesion){   
    gol(porteria);
  }
  else {
    seeker(last);
  }

}


//Funciones--------------------------------------------------

//---------------------------------------GOL
void gol (Porteria p){
  //Serial.println(p.x);

            if (p.x == -1)
              Serial.println("Nothing yet");
            else {
            if (p.x > 170){  //Derecha
              motoresRobot.movimientoLineal(45, velocidades);  
              digitalWrite(ledRojo, HIGH);
              //Serial.println(digitIn);
              Serial.println("der");
        
                    
            } else if (p.x < 60) {  //Izquierda
              motoresRobot.movimientoLineal(-45, velocidades);
              lastSeen = 'i';
              digitalWrite(ledVerde, HIGH);
              //Serial.println(digitIn);
              Serial.println("izq");


            } else {  //Centro
              motoresRobot.setAllMotorSpeed(velocidades);
              motoresRobot.adelante();
              digitalWrite(ledVerde, LOW);
              //digitalWrite(ledRojo, LOW);
              Serial.println("Adelante");
                          
            }
            }
          
}


//---------------------------------------SEEKER
void seeker(int last){

    //Definir dirección
    InfraredResult InfraredBall = InfraredSeeker::ReadAC(); 
    int dirSeeker = InfraredBall.Direction;
    int dirGrados = -150 + (dirSeeker*30);


    if (abs(dirGrados) < 90) {
     // Serial.print("Grados: ");
      Serial.println(dirGrados);
      motoresRobot.movimientoLineal(dirGrados, velocidades);
     
    }
    //Si no se encuentra en el rango
    else if (dirSeeker == 0){
      motoresRobot.movimientoLineal(110*last, velocidades);
      
    }
    //Transformar resultado del seeker a grados 
    else {
      Serial.println("Nope");
      motoresRobot.setAllMotorSpeed(velocidades);
      motoresRobot.atras();
     
    }

    if (dirGrados > 0)
      last = -1;
    else
      last = 1;

    return last;
    
  
}
