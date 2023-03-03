#include <ICM_20948.h>

//Librerías
#include <HTInfraredSeeker.h>
//#include <openmvrpc.h>
#include "Porteria.h"
#include "Motores.h"
#include "Imu.h"
#include "PID.h"


//Variables
bool posesion = false;
int velocidades = 100;


//LEDs
int ledRojo = 2;
int ledVerde = 3;


//Seeker
int dirSeeker;
int dirGrados;
int strSeeker;
int lastSeen = 1;

//IMU
Imu imu;

//PID
PID pid;

//MOTORES
Motores motoresRobot(8,24,25,9,26,27,10,22,23);
//Motores motoresRobot(8,41,27,7,25,24,6,22,23);

//Porterias
Porteria porteriaAzul;
Porteria porteriaAmarilla;
Porteria porterias[] = {porteriaAmarilla, porteriaAzul};
int indexColor = 0;

String input = "";
char lastP = "i";
 
//SETUP------------------------------------------------------
void setup() {
  Serial3.begin(9600);
  Serial.begin(9600);

  //Motores
  motoresRobot.iniciar();
  
  //Seeker
 // InfraredSeeker::Initialize();

imu.iniciar();
  pid.setPID(0.7,0.5);


}

//LOOP-------------------------------------------------------
void loop() {

  imu.readValues();

  //Serial.println(imu.getYaw());
  int change = pid.calcularError(0,imu.getYaw(),velocidades);
  
  motoresRobot.giro(change, imu.isRight());

  Serial.println(change);
    //Seeker
        //Serial.println("Seeker: ");

//    InfraredResult InfraredBall = InfraredSeeker::ReadAC(); 
//    Serial.println("1");
//
//    dirSeeker = InfraredBall.Direction;
//    dirGrados = -150 + (dirSeeker*30);
//    strSeeker = InfraredBall.Strength;
//    Serial.println(dirGrados);

    //motoresRobot.motor1Mover();
    //motoresRobot.adelante();
//  motoresRobot.movimientoLineal(0,velocidades);
//  delay(1000);
//
// motoresRobot.movimientoLineal(180,velocidades);
//  delay(1000);
  
//  motoresRobot.movimientoLineal(90,velocidades);
//  delay(1000);
//
// motoresRobot.movimientoLineal(-90,velocidades);
//  delay(1000);
//
//  motoresRobot.movimientoLineal(45,velocidades);
//  delay(1000);
//
//    motoresRobot.movimientoLineal(-135,velocidades);
//  delay(1000);
//
//  motoresRobot.movimientoLineal(-45,velocidades);
//  delay(1000);
//
//motoresRobot.movimientoLineal(135,velocidades);
//  delay(1000);


  

//    //Cámara
//    if (Serial3.available()) {   
//        input =  Serial3.readStringUntil('\n');
//        //Serial.println(input);
//        if(input[0] == '0')
//          porteriaAmarilla.actualizar(input);
//        else
//          porteriaAzul.actualizar(input);
//     }
//  
//      
//    //posesion = hasPosesion(strSeeker, dirSeeker);
//    
//    //Verificar si se debe buscar la pelota o portería
//    if (posesion){   
//      //motoresRobot.apagarMotores();
//      gol(porteriaAmarilla, lastP);
//    }
//    else {
//      lastSeen = seeker(dirSeeker, lastSeen);
//    }

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
              //Serial.println("Nothing yet");
            }
            else {
              if (p.x > 220){  //Derecha
                  motoresRobot.movimientoLineal(45, velocidades);  
                 // digitalWrite(ledRojo, HIGH);
                  //Serial.println(digitIn);
                  Serial.println("der");
                  lastP  = 'd';
            
                        
                } else if (p.x < 100) {  //Izquierda
                  motoresRobot.movimientoLineal(-45, velocidades);
                  //lastSeen = 'i';
                  //digitalWrite(ledVerde, HIGH);
                  //Serial.println(digitIn);
                  Serial.println("izq");
                  lastP = 'i';
    
    
                } else {  //Centro
                  motoresRobot.setAllMotorSpeed(velocidades);
                  motoresRobot.adelante();
                  //digitalWrite(ledVerde, LOW);
                  //digitalWrite(ledRojo, LOW);
                  Serial.println("Adelante");
                              
                }
            }
            return lastP;
          
} 


//---------------------------------------SEEKER

bool hasPosesion(int strSeeker, int dirSeeker){
  //Serial.println(strSeeker);
  if ((dirSeeker == 5) && (strSeeker > 140)){
    //Serial.println("true");
   digitalWrite(ledRojo, LOW);
     digitalWrite(ledVerde, HIGH);

    return true;
  }
    //Serial.println("false");
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledRojo, HIGH);

  return false;
}

//-------------------------------------
int seeker(int dirSeeker, int last){
    
    int dirGrados = -150 + (dirSeeker*30);
      //Serial.println(dirSeeker);

     Serial.println(dirGrados);

    if (dirSeeker == 0){
      Serial.println("Diagonal");
      motoresRobot.movimientoLineal(last*120, velocidades);
      Serial.println(last*120);

      
    } else if ((abs(dirGrados) <= 30)) {
      Serial.println("directo");
      motoresRobot.movimientoLineal(dirGrados, velocidades);
     
    } else if ((dirGrados != 0) && (abs(dirGrados) < 90)) {
      Serial.println("casi directoo");
      dirGrados += (dirGrados > 0) ? 20 : -20;
      motoresRobot.movimientoLineal(dirGrados, velocidades);
     
    }

    //Transformar resultado del seeker a grados 
    else {
      Serial.println("Atras");
      motoresRobot.setAllMotorSpeed(velocidades);
      motoresRobot.atras();
     
    }
//
    if (dirGrados > 0)
      last = -1;
    else
      last = 1;

    return last;

  
}
