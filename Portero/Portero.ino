// Portero Robocup Soccer Lightweight 2023

//Librerías
#include "AroIR.h"
#include "Color.h"
#include "Motores.h"
#include "PID.h"
#include "Porteria.h"
#include "Imu.h"  
#include "Constantes.h"
#include "Ultrasonico.h"
#include  "SingleEMAFilterLib.h"  
#include "BNO.h"


//Objetos
Motores motoresRobot(5, 28, 27, 6, 26, 25, 4, 30, 29);    
Ultrasonico ultrasonico(10, 36);
Ultrasonico ultrasonicoD(A3,A5);
Ultrasonico ultrasonicoI(8,50);
BNO gyro;
AroIR aroIR;
PID pid;
Color color;
Porteria porteriaAzul;
Porteria porteriaAmarilla;



//Estados del robot
enum Estados {
    linea,
    inicio,
    inPorteria,
    buscarPelota,
    hasPelota,
    golPorteria,
    salir,
    regresar,
    defender,
    detener,
    nada
};

enum Lados {
    amarillo = 0,
    azul = 1
};    

SingleEMAFilter<int> filterAnalogo(0.6);
Lados atacar;
Estados estado;
bool flagAdelante = false;


 

//SETUP------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial2.setTimeout(100);
  Serial.setTimeout(100);


  iniciarObjetos();
  delay(1500);
  //voltear();
  Serial.println("SETUP DONE");

}


//Código para atacante
//LOOP-------------------------------------------------------
void loop() {
  pid.setKP(0.03);
  atacar = amarillo;
  estado = linea;

  if (estado == linea) {
      if (color.checkForLineaPlaca2() == 180 || ultrasonico.getDistancia() < 20) 
        salirAdelante(0);
  
      else 
        estado = inicio;

  }

  if (estado == inicio) {
    actualizarPorterias();
    int y1 = (atacar == amarillo) ? porteriaAzul.getY() : porteriaAmarilla.getY();
    int largo = (atacar == amarillo) ? porteriaAzul.getLargo() : porteriaAmarilla.getLargo();

    //estado = (y1 < 75 || color.checkForLineaPlaca2() != 0) ? regresar : linea;
    Serial.println(color.checkForLineaPlaca2());
    Serial.println(y1);
    estado = (y1 < 70 || largo < 190) ? regresar : defender;

  }


//200


    //Verificar si está muy lejos de la portería 
 

  //Si está muy lejos de la portería, regresar
  if (estado == regresar) {
    //Serial.println("regresar");
    digitalWrite(Constantes::ledPin, LOW);
    buscarPorteria();
  }

  //Verificar que esté en la linea de la portería
 

  //Seguidor de línea para defender
  if (estado == defender) {
    Serial.print("defender ");
    digitalWrite(Constantes::ledPin, HIGH);
    // if (detector() > 5 && abs(aroIR.getAngulo()) < 20) //Si tiene posesión
    //   sacar();
    // else
      buscarC();
  }


  //Pruebas
  if (estado == nada) {
    tests();
  }

  if (estado == detener) {
    alinear();
  }


}
