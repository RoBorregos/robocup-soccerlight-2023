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
int lastSeen = 1;

//Objetos
Imu imu;
AroIR aroIR;
PID pid;
Motores motoresRobot(8, 41, 27, 7, 25, 24, 6, 23, 22);

//Porterias
Porteria porteriaAzul;
Porteria porteriaAmarilla;
Porteria porterias[] = {porteriaAmarilla, porteriaAzul};
int indexColor = 1;

//Estados del robot
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
  Serial2.begin(9600);

  //Iniciar objetos
  motoresRobot.iniciar();
  pid.setKP(0.3);
  imu.iniciar();

}


//Código para atacante
//LOOP-------------------------------------------------------
void loop() {
  

  //Verificar si está en la línea y moverse si es necesario
  if (estado == linea) {
    // estado = (inLinea()) ? linea : hasPelota;
    estado = hasPelota;
  }

  //Revisar si se tiene posesión de la pelota
  if (estado == hasPelota) {
    estado = (hasPosesion()) ? golPorteria : buscarPelota;
    // estado = nada;
  }

  //Buscar la pelota 
  if (estado == buscarPelota) {
    lastSeen = buscar(lastSeen);
    //lastSeen = seeker(dirSeeker, lastSeen);
  }

  //Ir a la portería con la pelota
  if (estado == golPorteria) {
    actualizarPorterias();
    gol(porteriaAzul, lastP);
  }

  //Pruebas
  if (estado == nada) {
    lastSeen = buscar(lastSeen);
    //tests();
  }

  estado = linea;

}


//-------------------------- Funciones de estados --------------------------------------------------

//---------------------------------------IMU
//Devolver el error a corregir
int correccionesImu() {
  imu.readValues();
  int change = pid.calcularError(0, imu.getYaw(), velocidades);

  return change;
}


//---------------------------------------GOL
//Ir a la portería dependiendo de las lecturas de la OpenMV
char gol (Porteria p, char lastP) {

  //Serial.println(p.x);

  imu.readValues();
  int change = pid.calcularError(0, imu.getYaw(), velocidades);

  //Si no ve la portería
  if (p.x == -1) {
    if (change > 0)
      motoresRobot.giro(change, imu.isRight());
    else
      motoresRobot.apagarMotores();

    Serial.println("Nothing yet");   
  }
  
  else {
    if (p.x > 220) { //Derecha
      motoresRobot.movimientoLinealCorregido(45, velocidades, change, imu.isRight());
      // digitalWrite(ledRojo, HIGH);
      //Serial.println(digitIn);
      Serial.println("der");
      lastP  = 'd';


    } else if (p.x < 100) {  //Izquierda
      motoresRobot.movimientoLinealCorregido(-45, velocidades, change, imu.isRight());
      //lastSeen = 'i';
      //digitalWrite(ledVerde, HIGH);
      //Serial.println(digitIn);
      Serial.println("izq");
      lastP = 'i';


    } else {  //Centro
      motoresRobot.movimientoLinealCorregido(0, velocidades, change, imu.isRight());
      //digitalWrite(ledVerde, LOW);
      //digitalWrite(ledRojo, LOW);
      Serial.println("Adelante");

    }
   
  }

  return lastP;

}

//---------------------------------------Porterias
//Obtener y almacenar los datos de la cámara
void actualizarPorterias() {
  if (estado == golPorteria) {
    if (Serial2.available()) {
      input =  Serial2.readStringUntil('\n');
      //Serial.println(input);
      if (input[0] == '0')
        porteriaAmarilla.actualizar(input);
      else
        porteriaAzul.actualizar(input);
    }
    actualizarPorterias();
    gol(porteriaAzul, lastP);
  }
}




//-------------------------------------ARO IR
//Buscar y moverse según la posición de la pelota
int buscar(int last) {
  
  int change = correccionesImu();
  aroIR.actualizarDatos();
  double angulo = aroIR.getAngulo();

  //Si no se está viendo la pelota
  if (aroIR.getStrength() == 0) {
    motoresRobot.apagarMotores();
    return;
  }

  //Pelota Adelante 
  if ((abs(angulo) < 45)) {
    Serial.println("< 45");
    // motoresRobot.movimientoLinealCorregido(angulo, velocidades, change, imu.isRight());
    do {
      motoresRobot.movimientoLinealCorregido(45, velocidades, change, imu.isRight());
    } while (aroIR.getAngulo() > 2);

  //Pelota Adelante diagonal
  } else if (abs(angulo) < 140) {
    Serial.println("< 140");
    do{
      motoresRobot.movimientoLinealCorregido(180, velocidades, change, imu.isRight());
    } while (aroIR.getAngulo() > 45);
    short i = 0;
    do{
      motoresRobot.movimientoLinealCorregido(i, velocidades, change, imu.isRight());
      delay(20);
      i += 1;
    } while (aroIR.getAngulo() > 2);

  //Pelota Adelante a los lados
  } else if (abs(angulo) < 250) {
    Serial.println("< 250");
    do{
      motoresRobot.movimientoLinealCorregido(180, velocidades, change, imu.isRight());
    } while (aroIR.getAngulo() > 90);
    short i = 0;
    do{
      motoresRobot.movimientoLinealCorregido(i, velocidades, change, imu.isRight());
      delay(20);
      i += 1;
    } while (aroIR.getAngulo() > 2);

  //Pelota a los lados
  } else if (abs(angulo) < 360) {
        Serial.println("< 360");
    do {
      motoresRobot.movimientoLinealCorregido(225, velocidades, change, imu.isRight());
    } while (aroIR.getAngulo() > 2);

  //Pelota atrás diagonal
  } else {
    Serial.println("Diagonal");
    motoresRobot.movimientoLinealCorregido(last * 120, velocidades, change, imu.isRight());
    Serial.println(last * 120);
  }

  //Actualizar el último lado en que se vió la pelota
  if (angulo > 0)
    last = -1;
  else
    last = 1;

  return last;

}


//---------------------------------------Posesion
//Retornar si se tiene la pelota o no
bool hasPosesion() {
  aroIR.actualizarDatos();
  double str = aroIR.getStrength();
  double angulo = aroIR.getAngulo();

  if ((angulo >= -20 && angulo <= 20) && (str > 24)) {
    Serial.println("true");
    return true;
  }
  
  Serial.println("nop");
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


  if (dirGrados > 0)
    last = -1;
  else
    last = 1;

  return last;

}


//Para el estado de pruebas
void tests() {
  //ultrasonicos.calcularDatos();
  //Serial.println(ultrasonicos.getAngulo(20,20));
  aroIR.actualizarDatos();
  double angulo = aroIR.getAngulo();
  Serial.println(aroIR.getStrength());
  Serial.println(angulo);
  //
  //double str = aroIR.getStrength();
  // Serial.println(str);
  // motoresRobot.movimientoLinealCorregido(45,velocidades,correccionesImu(), imu.isRight());
  //motoresRobot.apagarMotores();
  //correccionesImu();

  motoresRobot.setAllMotorSpeed(velocidades);
  //    motoresRobot.giroH();
  //    delay(1000);
  ////    motoresRobot.apagarMotores();
  ////    delay(1000);
  ////    motoresRobot.giroAH();
  ////    delay(1000);
  //    motoresRobot.apagarMotores();
  //    delay(1000);
}




// FOTOTRANSISTORES
void lecturaF(){
  for (int i=0; i<3; i++){
    for (int j=0; j<8; j++){
      if (lectura(j,i) >= fotoMinB[i][j] && lectura(j,i) <= fotoMaxB[i][j]){
        Serial.print("Jaló el (P  F): ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(j);
        motoresRobot.movimientoLineal(180,velocidades);
        delay(10);
      }
    }
  }
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
