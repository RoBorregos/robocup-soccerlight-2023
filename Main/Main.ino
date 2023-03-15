//Librerías
#include <ICM_20948.h>
#include <HTInfraredSeeker.h>
#include "Porteria.h"
#include "Motores.h"
#include "Imu.h"
#include "PID.h"
#include "AroIR.h"


//Variables
bool posesion = true;
int velocidades = 160;
String input = "";
char lastP = "i";

//LEDs
int ledRojo = 2;
int ledVerde = 3;

//Seeker
int dirSeeker;
int dirGrados;
int strSeeker;
int lastSeen = 1;

//Objetos
Imu imu;
AroIR aroIR;
PID pid;

//MOTORES
//Motores motoresRobot(8, 24, 25, 9, 26, 27, 10, 22, 23);
Motores motoresRobot(8,41,27,7,25,24,6,23,22);

//Porterias
Porteria porteriaAzul;
Porteria porteriaAmarilla;
Porteria porterias[] = {porteriaAmarilla, porteriaAzul};
int indexColor = 1;

enum Estados {
  linea, 
  buscarPelota, 
  hasPelota,
  golPorteria, 
  nada
};

Estados estado = linea;

//SETUP------------------------------------------------------
void setup() {
  Serial2.begin(9600);
  Serial.begin(9600);

  //Iniciar clases
  motoresRobot.iniciar();
  pid.setKP(0.3);
  imu.iniciar();

  //InfraredSeeker::Initialize();

}

//LOOP-------------------------------------------------------
void loop() {

//Código para atacante
if (estado == linea) {
 // estado = (inLinea()) ? linea : hasPelota;
 estado = hasPelota;
}

if (estado == hasPelota) {
  //estado = (hasPosesion()) ? golPorteria : buscarPelota; 
  estado = golPorteria;
}

if (estado == buscarPelota) {
  lastSeen = buscar(lastSeen);
  //lastSeen = seeker(dirSeeker, lastSeen);
}

if (estado == golPorteria) {
  actualizarPorterias();
  gol(porteriaAzul, lastP);
  //correccionesImu(); 
}

if (estado == nada) {
  motoresRobot.movimientoLinealCorregido(45,velocidades,correccionesImu(), imu.isRight());
  //motoresRobot.apagarMotores();
  //correccionesImu();
  
//    motoresRobot.setAllMotorSpeed(velocidades);
//    motoresRobot.giroH();
//    delay(1000);
////    motoresRobot.apagarMotores();
////    delay(1000);
////    motoresRobot.giroAH();
////    delay(1000);
//    motoresRobot.apagarMotores();
//    delay(1000);
}

estado = linea;

}


//-------------------------- Funciones de estados --------------------------------------------------

//---------------------------------------IMU
int correccionesImu(){
  
  imu.readValues();
      int change = pid.calcularError(0,imu.getYaw(),velocidades);
//      Serial.print("Change: ");
//        Serial.println(change);

//        if (change > 0) {
//          motoresRobot.giro(change, imu.isRight());
//        } else {
//          motoresRobot.apagarMotores();
//        }

        return change;
}

//---------------------------------------GOL
char gol (Porteria p, char lastP) {
  
  //Serial.println(p.x);

  imu.readValues();
  int change = pid.calcularError(0,imu.getYaw(),velocidades);

  //Si no ve la portería
  if (p.x == -1) {
    if (change > 0)
      motoresRobot.giro(change, imu.isRight());
    else
      motoresRobot.apagarMotores();
    
//    motoresRobot.setAllMotorSpeed(100);
//   
//      if(lastP == 'i')
//        motoresRobot.giroAH();
//      else
//        motoresRobot.giroH();
    Serial.println("Nothing yet");
  }
  else {
    if (p.x > 220) { //Derecha
      motoresRobot.movimientoLinealCorregido(45, velocidades,change,imu.isRight());
      // digitalWrite(ledRojo, HIGH);
      //Serial.println(digitIn);
      Serial.println("der");
      lastP  = 'd';


    } else if (p.x < 100) {  //Izquierda
      motoresRobot.movimientoLinealCorregido(-45, velocidades,change,imu.isRight());
      //lastSeen = 'i';
      //digitalWrite(ledVerde, HIGH);
      //Serial.println(digitIn);
      Serial.println("izq");
      lastP = 'i';


    } else {  //Centro
      motoresRobot.movimientoLinealCorregido(0, velocidades,change,imu.isRight());
      //digitalWrite(ledVerde, LOW);
      //digitalWrite(ledRojo, LOW);
      Serial.println("Adelante");

    }
  }
  //}

  return lastP;

}

//---------------------------------------Porterias
void actualizarPorterias(){
      if (Serial2.available()) {   
        input =  Serial2.readStringUntil('\n');
        //Serial.println(input);
        if(input[0] == '0')
          porteriaAmarilla.actualizar(input);
        else
          porteriaAzul.actualizar(input);
     }
}


//-------------------------------------AROIR
int buscar(int last){
  double angulo = aroIR.getAngulo();
  
  if ((abs(angulo) <= 30)) {
    Serial.println("directo");
    motoresRobot.movimientoLineal(angulo, velocidades);

  } else if (abs(angulo) < 90) {
    Serial.println("casi directoo");
    dirGrados += (dirGrados > 0) ? 20 : -20;
    motoresRobot.movimientoLineal(dirGrados, velocidades);

  } else if (angulo < 110) {
    Serial.println("Atras");
    motoresRobot.setAllMotorSpeed(velocidades);
    motoresRobot.atras();
    
  } else {
    Serial.println("Diagonal");
    motoresRobot.movimientoLineal(last * 120, velocidades);
    Serial.println(last * 120);
  }

  if (angulo > 0)
    last = -1;
  else
    last = 1;

  return last;
  
}


//---------------------------------------Posesion
bool hasPosesion() {
      InfraredResult InfraredBall = InfraredSeeker::ReadAC();
      dirSeeker = InfraredBall.Direction;
  //    dirGrados = -150 + (dirSeeker*30);
      strSeeker = InfraredBall.Strength;
  //    Serial.println(dirGrados);
  //Serial.println(strSeeker);

  
  if ((dirSeeker == 5) && (strSeeker > 140)) {
    //Serial.println("true");
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledVerde, HIGH);

    return true;
  }
  
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledRojo, HIGH);

  return false;
}

//---------------------------------------SEEKER
int seeker(int dirSeeker, int last) {
  //long tiempoObjetivo = millis() + 1000;

  //    while(millis() < tiempoObjetivo){

  int dirGrados = -150 + (dirSeeker * 30);
  //Serial.println(dirSeeker);

  Serial.println(dirGrados);

  if (dirSeeker == 0) {
    Serial.println("Diagonal");
    motoresRobot.movimientoLineal(last * 120, velocidades);
    Serial.println(last * 120);


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
  //}

//---------------------------------------PID
  int change = pid.calcularError(0, imu.getYaw(), velocidades);
  motoresRobot.giro(change, imu.isRight());


  if (dirGrados > 0)
    last = -1;
  else
    last = 1;

  return last;

}
