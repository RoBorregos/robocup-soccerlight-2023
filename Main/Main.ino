#include "Motores.h"
//GND --> GND
//P4 --> RX0


//Librerías
#include <HTInfraredSeeker.h>


//Variables
String colorPorteria = "y";
bool posesion = false;
int velocidades = 160;
char input = 'a';
char lastSeen = 'i';


//LEDs
int ledRojo = 2;
int ledVerde = 3;


//Motores
Motores motoresRobot(8,24,25,9,26,27,10,22,23);

 
//SETUP------------------------------------------------------
void setup() {
   
  Serial.begin(9600);
  Serial3.begin(9600);

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
  
  //Lectura de la cámara
    //Serial3.write("y");
    if (Serial3.available()) {   
       input =  Serial3.read();
       Serial.println(input);
    }

  //Verificar si se debe buscar la pelota o portería
  if (posesion){   
    lastSeen = porteria(input, lastSeen);
  }
  else {
    seeker();
  }
  
}


//Funciones--------------------------------------------------
//Cuando ya se tenga la pelota
char porteria (char input, char last){
      char lastSeen = last;
      int digitIn = input - 48;

      //Si se ve la portería (recibe un valor 0-9)
      if (input != 'a'){
  
            if (digitIn > 7){  //Derecha
              motoresRobot.movimientoLineal(45, velocidades);  
              lastSeen  = 'd';
              digitalWrite(ledRojo, HIGH);
              Serial.println(digitIn);
              Serial.println("Derecha");
        
                    
            } else if (digitIn < 2) {  //Izquierda
              motoresRobot.movimientoLineal(-45, velocidades);
              lastSeen = 'i';
              digitalWrite(ledVerde, HIGH);
              Serial.println(digitIn);
              Serial.println("Izq");


            } else {  //Centro
              motoresRobot.setAllMotorSpeed(velocidades);
              motoresRobot.adelante();
              digitalWrite(ledVerde, LOW);
              digitalWrite(ledRojo, LOW);
              Serial.println("Adelante");
                          
            }
          
      } else {
              motoresRobot.setAllMotorSpeed(120);

              if (lastSeen = 'i') {
                    motoresRobot.giroAH();
                  
               } else {
                    motoresRobot.giroH();
          
                 }
     }

  //Retorna el último lugar en donde vió la portería (izquierda o derecha)
  return lastSeen;
}


//---------------------------------------SEEKER
void seeker(){

    //Definir dirección
    InfraredResult InfraredBall = InfraredSeeker::ReadAC(); 
    int dirSeeker = InfraredBall.Direction;
    
    //Si no se encuentra en el rango
    if (dirSeeker == 0){
      Serial.println("Nope");
      motoresRobot.setAllMotorSpeed(200);
      motoresRobot.atras();
      
    }
    //Transformar resultado del seeker a grados 
    else {
      int dirGrados = -150 + (dirSeeker*30);
     // Serial.print("Grados: ");
      Serial.println(dirGrados);
      motoresRobot.movimientoLineal(dirGrados, 200);
     
    }
    
  
}
