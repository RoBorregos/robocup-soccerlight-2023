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
int lastSeen;

//Motores
Motores motoresRobot(8,24,25,9,26,27,10,22,23);

//Porteria
Porteria porteria;
String input = "";
char lastP = "i";
 
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
  lastSeen = 1;

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
    
//    Verificar si se debe buscar la pelota o portería
    if (posesion){   
      gol(porteria, lastP);
    }
    else {
      lastSeen = seeker(dirSeeker, lastSeen);
    }

}


//Funciones--------------------------------------------------

//---------------------------------------GOL
char gol (Porteria p, char lastP){
  //Serial.println(p.x);

            if (p.x == -1){
                  motoresRobot.setAllMotorSpeed(velocidades-50);
                  if(lastP == 'i')
                    motoresRobot.giroAH();
                  else
                    motoresRobot.giroH();
              Serial.println("Nothing yet");
            }
            else {
              if (p.x > 220){  //Derecha
                  motoresRobot.movimientoLineal(45, velocidades);  
                  digitalWrite(ledRojo, HIGH);
                  //Serial.println(digitIn);
                  Serial.println("der");
                  lastP  = 'd';
            
                        
                } else if (p.x < 100) {  //Izquierda
                  motoresRobot.movimientoLineal(-45, velocidades);
                  //lastSeen = 'i';
                  digitalWrite(ledVerde, HIGH);
                  //Serial.println(digitIn);
                  Serial.println("izq");
                  lastP = 'i';
    
    
                } else {  //Centro
                  motoresRobot.setAllMotorSpeed(velocidades);
                  motoresRobot.adelante();
                  digitalWrite(ledVerde, LOW);
                  //digitalWrite(ledRojo, LOW);
                  Serial.println("Adelante");
                              
                }
            }
            return lastP;
          
} 


//---------------------------------------SEEKER

bool hasPosesion(int strSeeker, int dirSeeker){
  //Serial.println(strSeeker);
  if ((dirSeeker < 20 && dirSeeker > -20) && (strSeeker > 110 || strSeeker == 0)){
   // Serial.println("true");
   digitalWrite(ledRojo, LOW);
     digitalWrite(ledVerde, HIGH);

    return true;
  }
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledRojo, HIGH);

  return false;
}

//-------------------------------------
int seeker(int dirSeeker, int last){
    
    int dirGrados = -150 + (dirSeeker*30);
      //Serial.println(dirSeeker);

     Serial.println(dirGrados);

    if (abs(dirGrados) < 80) {
     // Serial.print("Grados: ");
      motoresRobot.movimientoLineal(dirGrados, velocidades);
     
    }
    //Si no se encuentra en el rango
    else if (dirSeeker == 0){
      motoresRobot.movimientoLineal(last*110, velocidades);
      
    }
    //Transformar resultado del seeker a grados 
    else {
      Serial.println("Nope");
      motoresRobot.setAllMotorSpeed(velocidades);
      motoresRobot.atras();
     
    }
//
    if (dirGrados > 0)
      last = -1;
    else
      last = 1;

    return last;
    //return last;
    //rojo 5v
    //3.3v
    //gnd
    //scl
    //sda
  
}
