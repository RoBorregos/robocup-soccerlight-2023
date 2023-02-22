//Librerías
#include <HTInfraredSeeker.h>
#include <openmvrpc.h>
#include "Porteria.h"
#include "Motores.h"



//Variables
bool posesion = false;
int velocidades = 160;


//LEDs
int ledRojo = 2;
int ledVerde = 3;


//Seeker
int dirSeeker;
int dirGrados;
int strSeeker;


//Motores
Motores motoresRobot(8,24,25,9,26,27,10,22,23);

//Porteria
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

    //Seeker
    InfraredResult InfraredBall = InfraredSeeker::ReadAC(); 
    dirSeeker = InfraredBall.Direction;
    dirGrados = -150 + (dirSeeker*30);
    strSeeker = InfraredBall.Strength;
  
    //Cámara
    if (Serial3.available()) {   
        input =  Serial3.readStringUntil('\n');
        porteria.actualizar(input);
     }
  
      
    posesion = hasPosesion(strSeeker, dirGrados);
    
    //Verificar si se debe buscar la pelota o portería
    if (posesion){   
      gol(porteria);
    }
    else {
      seeker(dirGrados);
    }

}


//Funciones--------------------------------------------------

//---------------------------------------GOL
void gol (Porteria p){
  //Serial.println(p.x);

            if (p.x == -1)
              Serial.println("Nothing yet");
            else {
            if (p.x > 220){  //Derecha
              motoresRobot.movimientoLineal(45, velocidades);  
              digitalWrite(ledRojo, HIGH);
              //Serial.println(digitIn);
              Serial.println("der");
        
                    
            } else if (p.x < 100) {  //Izquierda
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

bool hasPosesion(int strSeeker, int dirSeeker){
  if (dirSeeker == 0 && (strSeeker > 115 || strSeeker == 0)){
    return true;
  }
  return false;
}

void seeker(int dirSeeker){
    int dirGrados = -150 + (dirSeeker*30);

    
    if (abs(dirGrados) < 90) {
     // Serial.print("Grados: ");
      //Serial.println(dirGrados);
      motoresRobot.movimientoLineal(dirGrados, velocidades);
     
    }
    //Si no se encuentra en el rango
    else if (dirSeeker == 0){
      motoresRobot.movimientoLineal(110*last, velocidades);
      
    }
    //Transformar resultado del seeker a grados 
    else {
      //Serial.println("Nope");
      motoresRobot.setAllMotorSpeed(velocidades);
      motoresRobot.atras();
     
    }

    if (dirGrados > 0)
      last = -1;
    else
      last = 1;

    return last;
    //rojo 5v
    //3.3v
    //gnd
    //scl
    //sda
  
}
