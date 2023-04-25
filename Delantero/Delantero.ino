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
Motores motoresRobot(6, 25, 26, 5, 28, 27, 4, 30, 29);    //robot bno
BNO gyro;

AroIR aroIR;
PID pid;
Color color;
Porteria porteriaAzul;
Porteria porteriaAmarilla;
Dribbler dribbler(7);
Ultrasonico ultrasonico(40,38);


enum Estados {
        linea,
        buscarPelota,
        hasPelota,
        golPorteria,
        salir,
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

Lados atacar = azul;
Estados estado;



//SETUP------------------------------------------------------
void setup() {
  Serial3.begin(115200);
  Serial3.setTimeout(100);
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial2.setTimeout(100);


  iniciarObjetos();   //Set up de objetos
    // esc.attach(7);
    // delay(2000); 
    // esc.writeMicroseconds(780); // set the motor speed to minimum
    // delay(1500); 

  //delay(1600);
  //voltear();          //Verificar si el robot inició volteado y corregirlo
 
}


//LOOP-------------------------------------------------------
void loop() {

  //Estado inicial
  estado = buscarPelota;

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
    estado = (detector() > 5 && abs(aroIR.getAngulo()) < 15) ? golPorteria : buscarPelota;
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

}






//------------------------------------------------------------------------------------ Funciones de estados -------------------------------------------------------------------------------------
