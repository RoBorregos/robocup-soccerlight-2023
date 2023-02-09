//#include "Motores.h"
//GND --> GND
//P4 --> RX0

//Librerías
#include <HTInfraredSeeker.h>

//Variables
String colorPorteria = "y";
bool posesion = true;
int velocidades = 160;
char input = 'a';
char lastSeen = 'i';

//LEDs
int ledRojo = 2;
int ledVerde = 3;

//Motores
int motor1Speed = 8;
int motor1P1 = 24;
int motor1P2 = 25;

int motor2Speed = 9;
int motor2P1 = 26;
int motor2P2 = 27;

int motor3Speed = 10;
int motor3P1 = 22;
int motor3P2 = 23;

 

void setup() {
   
   Serial.begin(9600);
   Serial3.begin(9600);

  //Motores
  pinMode(motor1P1, OUTPUT);
  pinMode(motor1P2, OUTPUT);
  pinMode(motor1Speed, OUTPUT);
  
  pinMode(motor2P1, OUTPUT);
  pinMode(motor2P2, OUTPUT);
  pinMode(motor2Speed, OUTPUT);
  
  pinMode(motor3P1, OUTPUT);
  pinMode(motor3P2, OUTPUT);
  pinMode(motor3Speed, OUTPUT);


  //LED
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  
  //Seeker
  InfraredSeeker::Initialize();

}
 
void loop() {
  


  //Lectura de la cámara
  Serial3.write("y");
  if (Serial3.available()) {   
        input =  Serial3.readString();
       Serial.println(input);
    }

  if (posesion){   
    lastSeen = porteria(input, lastSeen);
  }
  else {
   // seeker();
  }
  
}

//Cuando ya se tenga la pelota
char porteria (char input, char last){
  char lastSeen = last;
       int digitIn = input - 48;

        //Si se ve la portería
        if (input != 'a'){
  
            if (digitIn > 7){  //Derecha
              movimientoLineal(45, velocidades);  
              lastSeen  = 'd';
              digitalWrite(ledRojo, HIGH);
              Serial.println(digitIn);
              Serial.println("Derecha");

              
                    
            } else if (digitIn < 2) {  //Izquierda
              movimientoLineal(-45, velocidades);
              lastSeen = 'i';
              digitalWrite(ledVerde, HIGH);
              Serial.println(digitIn);
              Serial.println("Izq");


            } else {  //Centro
              setAllMotorSpeed(velocidades);
              adelante();
              digitalWrite(ledVerde, LOW);
              digitalWrite(ledRojo, LOW);
              Serial.println("Adelante");

              
            }
          
        } else {
              setAllMotorSpeed(120);

                  if (lastSeen = 'i') {
                    giroAH();
                  
                  } else {
                    giroH();
          
                 }
//            //}
        }
      
  return lastSeen;
}


//---------------------------------------SEEKER

void seeker(){

    //Definir dirección
    InfraredResult InfraredBall = InfraredSeeker::ReadAC(); 
    int dirSeeker = InfraredBall.Direction;
    
    Serial.println("Direction = ");
    Serial.println(dirSeeker);

    //Si no se encuentra en el rango
    if (dirSeeker == 0){
      Serial.println("Nope");
      setAllMotorSpeed(200);
      atras();
      
    }
    //Transformar resultado del seeker a grados 
    else {
      int dirGrados = -150 + (dirSeeker*30);
      Serial.print("Grados: ");
      Serial.println(dirGrados);
      movimientoLineal(dirGrados, 200);
     
    }
    
    delay(2000);
    resetMotors();
    delay(1000);
  
}

//---------------------------------------MOVIMIENTOS
void movimientoLineal(int degree, int velocidad) {

    //En base a los grados se definen las velocidades de cada motor
    float m1 = cos(((150-degree) * PI / 180));
    float m2 = cos(((30-degree) * PI / 180));;
    float m3 = cos(((270-degree) * PI / 180));
    int speedA = abs(int(m1*velocidad));
    int speedB = abs(int(m2*velocidad));
    int speedC = abs(int(m3*velocidad));

    //Definir la velocidad de cada motor
    analogWrite(motor1Speed, speedA);
    analogWrite(motor2Speed, speedB);
    analogWrite(motor3Speed, speedC);

    //Mover motores según la velocidad (positiva o negativa)
    if (m1 >= 0){
        motor1Adelante();
    } else {
        motor1Atras();
    }
    
    if (m2 >= 0){
        motor2Adelante();
    } else {
        motor2Atras();
    } 
    
    if (m3 >= 0){
        motor3Adelante();
    } else {
        motor3Atras();
    }
}



//----------------------------Movimientos básicos

//Definir la velocidad de todos los motores
void setAllMotorSpeed(int allSpeed){
  analogWrite(motor1Speed, allSpeed);
  analogWrite(motor2Speed, allSpeed);
  analogWrite(motor3Speed, allSpeed);
}

void adelante(){
  resetMotors();
  motor1Atras();
  motor2Adelante();
}

void atras(){
  resetMotors();
  delay(800);
  
  motor1Adelante();
  motor2Atras();
}

//Rotación horaria
void giroH(){
  motor1Adelante();
  motor2Adelante();
  motor3Adelante();

}

//Rotación antihoraria
void giroAH(){
  motor1Atras();
  motor2Atras();
  motor3Atras();

}

//Noreste
void NE(){
  resetMotors();
  delay(800);

  motor2Adelante();
  motor3Atras();
}

//Noroeste
void NO(){
  resetMotors();
  delay(800);

  motor3Adelante();
  motor1Atras();
}

//Sureste
//void SurE(){
//  resetMotors();
//  delay(800);
//
//  motor3Atras();
//  motor1Adelante();
//}

//Suroeste
void SO(){
  motor2Atras();
  motor3Adelante();
}

//-----------------------------------------CONTROL
//Apagar motores
void resetMotors(){
  digitalWrite(motor1P1, LOW);
  digitalWrite(motor1P2, LOW);

  digitalWrite(motor2P1, LOW);
  digitalWrite(motor2P2, LOW);

  digitalWrite(motor3P1, LOW);
  digitalWrite(motor3P2, LOW);
}

void motor1Adelante(){
  digitalWrite(motor1P1, HIGH);
  digitalWrite(motor1P2, LOW);
}

void motor2Adelante(){
  digitalWrite(motor2P1, HIGH);
  digitalWrite(motor2P2, LOW);
}

void motor3Adelante(){
  digitalWrite(motor3P1, HIGH);
  digitalWrite(motor3P2, LOW);
}

void motor1Atras(){
  digitalWrite(motor1P1, LOW);
  digitalWrite(motor1P2, HIGH);
}

void motor2Atras(){
  digitalWrite(motor2P1, LOW);
  digitalWrite(motor2P2, HIGH);
}

void motor3Atras(){
  digitalWrite(motor3P1, LOW);
  digitalWrite(motor3P2, HIGH);
}
