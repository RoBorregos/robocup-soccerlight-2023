// Delantero Robocup Soccer Lightweight 2023

//Librerías
#include "AroIR.h"
#include "Color.h"
#include "Motores.h"
#include "PID.h"
#include "Porteria.h"
#include "Dribbler.h"
#include "SingleEMAFilterLib.h"
#include "BNO.h"
#include "Constantes.h"
#include "Ultrasonico.h"

 #include <Servo.h>

  Servo esc;


//Objetos
SingleEMAFilter<int> filterAnalogo(0.6);
Motores motoresRobot(5, 28, 27, 6, 25, 26, 4, 30, 29);    //robot bno
BNO gyro;

AroIR aroIR;
PID pid;
Color color;
Porteria porteriaAzul;
Porteria porteriaAmarilla;
Dribbler dribbler(7);
Ultrasonico ultrasonico(11,2);


enum Estados {
        linea,
        buscarPelota,
        hasPelota,
        golPorteria,
        salir,
        detener,
        nada
};

       enum Lados {
        amarillo = 0,
        azul = 1
};


//Variables
int lastP = 1;
int last = 1;
int atacarE = 1;
unsigned long current_time = 0;


Lados atacar = amarillo;
Estados estado;



//SETUP------------------------------------------------------
void setup() {
  Serial3.begin(115200);
  Serial3.setTimeout(100);
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial2.setTimeout(100);


  iniciarObjetos();   //Set up de objetos
  voltear();          //Verificar si el robot inició volteado y corregirlo
 
}


//LOOP-------------------------------------------------------
void loop() {
  current_time = millis();


  /*Calibrar camara
  Cambiar color
  Cambiar lado a atacar
  Estado = linea*/
  

  //Estado inicial
  estado = linea;

  //Verificar si está en la línea y moverse si es necesario
  if (estado == linea) {
    estado = (inLinea()) ? salir : hasPelota;
  }

  if (estado == salir) {
    salirLinea();
  }

  //Revisar si se tiene posesión de la pelota
  if (estado == hasPelota) {
    aroIR.actualizarDatos();
    estado = (detector() >= 5 && abs(aroIR.getAngulo()) < 15) ? golPorteria : buscarPelota;
  }

  //Buscar la pelota
  if (estado == buscarPelota) {
    digitalWrite(Constantes::led, LOW);
    Serial.println("buscar");
    buscar();
  }


  //Ir a la portería con la pelota
  if (estado == golPorteria) {
    digitalWrite(Constantes::led, HIGH);
    gol();

  }



  //Pruebas
  if (estado == nada) {
  
   tests();
  }

  //Detener para pruebas
  if (estado == detener) {
    alinear();
  }

}



