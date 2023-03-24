//Librerías
#include <ICM_20948.h>
#include <HTInfraredSeeker.h>
#include "Porteria.h"
#include "Motores.h"
#include "Imu.h"
#include "PID.h"
#include "AroIR.h"
#include "Posicion.h"
#include "Color.h"


//Variables
bool posesion = true;
int velocidades = 160;
String input = "";
char lastP = "i";
int degree = 0;
const short pinBoton = 0;

//LEDs
int ledRojo = 2;
int ledVerde = 3;

//Seeker
int dirSeeker;
int dirGrados;
int strSeeker;
int lastSeen = 1;

// Fototransistores
int sig[3] = {A0,A1,A2}; // pines signal: DERECHA, PRINCIPAL, IZQUIERDA 
int sC[3] = {49,5,38};  // pines de 'control' ABC 
int sB[3] = {12,2,43};
int sA[3] = {47,3,42};
int fotot[3][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
int fotoMinV[3][8] = {{2,6,7,8,3,6,5,8},{0,1,1,3,4,0,3,8},{1,7,10,6,2,6,1,10}};
int fotoMaxV[3][8] = {{4,8,9,10,5,8,8,10},{1,3,4,5,5,0,5,11},{6,13,16,12,5,10,8,16}};
int fotoMinB[3][8] = {{27,50,43,35,32,41,43,47},{19,44,35,58,56,100,52,58},{25,49,52,49,31,53,45,69}};
int fotoMaxB[3][8] = {{30,53,46,38,35,44,46,50},{22,47,38,62,58,100,55,60},{33,56,63,56,36,60,52,79}};
int fotoP[8] = {0,0,0,0,0,0,0,0};
int fotoPrev[8] = {0,0,0,0,0,0,0,0};
#define blanco 49      // detección del blanco
#define error 2     // rango de detección del blanco

//Objetos
Imu imu;
AroIR aroIR;
PID pid;
Posicion pos;
Color color;

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
  Serial.begin(9600);

  //Iniciar clases
  motoresRobot.iniciar();
  pid.setKP(0.3);
  imu.iniciar();
  pos.iniciar();
  color.iniciar();
  color.calibrar();

  //InfraredSeeker::Initialize();

  // Iniciar placas de fototransistores
  for (int i=0; i<3; i++){  
    pinMode(sC[i],OUTPUT);
    pinMode(sB[i],OUTPUT);
    pinMode(sA[i],OUTPUT);
  }

  delay(1000);

}

//LOOP-------------------------------------------------------
void loop() {
  
  degree = 0;
  motoresRobot.movimientoLinealCorregido(degree,velocidades,correccionesImu(),imu.isRight());
  Serial.println("Jaló1");
  
  lecturaF1();
  
  color.checkForLinea();

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



// FOTOTRANSISTORES
void lecturaF1(){

  for (int i=0; i<3; i++){
    for (int j=0; j<8; j++){
      if (lectura(j,i) >= fotoMinB[i][j] && lectura(j,i) <= fotoMaxB[i][j]){
        Serial.print("Jaló BLANCO. ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(j);
        motoresRobot.movimientoLineal(180,velocidades);
        delay(10);
      }
    }
  }


  //Serial.println(fotoP[4]);

  // for (int i=0; i<8; i++){
  //   if (abs(fotoP[i] - fotoPrev[i]) >= 20){
  //     motoresRobot.movimientoLineal(0,0);
  //     while (1){}
  //   }
  // }


  // for (int i=0; i<8; i++){
  //   fotoP[i] = lectura(i,0);
  // }
  // for (int i=0; i<8; i++){
  //   if ((fotoP[i] > blanco) && (fotoP[i] < (blanco+2))){
  //       motoresRobot.movimientoLineal(0,0);
  //       while (1){}
  //   }
  // }
}
void lecturaF(){

  for (int i=0; i<8; i++){
    for (int j=0; j<3; j++){
      if (lectura(i,j) >= fotoMinB[j][i] && lectura(i,j) <= fotoMaxB[j][i]){
        Serial.println("Jaló");
        motoresRobot.movimientoLinealCorregido(180,velocidades,correccionesImu(),imu.isRight());
        delay(500);
      }
    }
  }

  // Serial.println("empieza");
  // for (int i=0; i<8; i++){
  //   for (int j=0; j<3; j++){
  //     fotot[j][i] = lectura(i,j);
  //   }
  // }


  // for (int i=0; i<8; i++){
  //   for (int j=0; j<3; j++){
  //     Serial.println(fotot[j][i]);
  //     if (fotot[j][i] > blanco && fotot[j][i] < (blanco+2)){
  //       motoresRobot.movimientoLineal(0,0);
  //       while (1){}
  //     }
  //   }
  // }
}
int lectura(int ft, int placa){   // 1: derecha (1,3,4)
  switch (ft){
    case 0: binario(0,placa); return analogRead(sig[placa]); break;
    case 1: binario(1,placa); return analogRead(sig[placa]); break;
    case 2: binario(2,placa); return analogRead(sig[placa]); break;
    case 3: binario(3,placa); return analogRead(sig[placa]); break;
    case 4: binario(4,placa); return analogRead(sig[placa]); break;
    case 5: binario(5,placa); return analogRead(sig[placa]); break;
    case 6: binario(6,placa); return analogRead(sig[placa]); break;
    case 7: binario(7,placa); return analogRead(sig[placa]); break;
  }
}
void binario(int pin, int lado){
  switch (pin){
    case 0: 
      digitalWrite(sC[lado],0); 
      digitalWrite(sB[lado],0); 
      digitalWrite(sA[lado],0); break;
    case 1:
      digitalWrite(sC[lado],0); 
      digitalWrite(sB[lado],0); 
      digitalWrite(sA[lado],1); break;
    case 2:
      digitalWrite(sC[lado],0); 
      digitalWrite(sB[lado],1); 
      digitalWrite(sA[lado],0); break;
    case 3:
      digitalWrite(sC[lado],0); 
      digitalWrite(sB[lado],1); 
      digitalWrite(sA[lado],1); break;
    case 4: 
      digitalWrite(sC[lado],1); 
      digitalWrite(sB[lado],0); 
      digitalWrite(sA[lado],0); break;
    case 5: 
      digitalWrite(sC[lado],1); 
      digitalWrite(sB[lado],0); 
      digitalWrite(sA[lado],1); break;
    case 6: 
      digitalWrite(sC[lado],1); 
      digitalWrite(sB[lado],1); 
      digitalWrite(sA[lado],0); break;
    case 7: 
      digitalWrite(sC[lado],1); 
      digitalWrite(sB[lado],1); 
      digitalWrite(sA[lado],1); break;
  }
}


void calibracionFototransistores(){

  for (int i=0; i<3; i++){   // calibrar VERDE
    for (int j=0; j<8; j++){
      for (int k=0; k<1000; k++){
        int lec = lectura(j,i);
        if (lec < fotoMinV[i][j]){
          fotoMinV[i][j] = lec;
        }
        else if (lec > fotoMaxV[i][j]){
          fotoMaxV[i][j] = lec;
        }
      }
    }
  }

  for (int i=0; i<3; i++){   // calibrar BLANCO
    for (int j=0; j<8; j++){
      for (int k=0; k<1000; k++){
        int lec = lectura(j,i);
        if (lec < fotoMinB[i][j]){
          fotoMinB[i][j] = lec;
        }
        else if (lec > fotoMaxB[i][j]){
          fotoMaxB[i][j] = lec;
        }
      }
    }
    while (digitalRead(pinBoton) == 1){}
  }

}