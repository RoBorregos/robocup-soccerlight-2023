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
#include "Imu.h"


//Objetos
Motores motoresRobot(4, 29, 30, 6, 25, 26, 5, 27, 28);    
Ultrasonico ultrasonico(2, 11);
Ultrasonico ultrasonicoD(12,3);
Ultrasonico ultrasonicoI(13,18);
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
Lados atacar = amarillo;
Estados estado;
bool flagAdelante = false;


unsigned long current_time = 0;

//SETUP------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial2.setTimeout(100);
  Serial.setTimeout(100);

  current_time = millis();


  iniciarObjetos();
  delay(1500);
  voltear();
  Serial.println("SETUP DONE");

}


//LOOP-------------------------------------------------------
void loop() {
  current_time = millis();
  //pid.setKP(0.02);
  estado = inicio;

  /*Calibrar camara
  Calibrar fotos
  Cambiar color
  Cambiar lado a atacar
  Estado = inicio*/


  actualizarPorterias();
  int largo = (atacar == amarillo) ? porteriaAzul.getLargo() : porteriaAmarilla.getLargo();
  int alto = (atacar == amarillo) ? porteriaAzul.getAlto() : porteriaAmarilla.getAlto();
  int x1 = (atacar == amarillo) ? porteriaAzul.getX() : porteriaAmarilla.getX();

  if (estado == inicio) {
    Serial.print(alto);
    Serial.print("\t\t");
    Serial.println(largo);

    estado = (alto < 70 || largo < 170) ? regresar : linea;
  }

  if (estado == linea) {
      int alto = (atacar == amarillo) ? porteriaAzul.getAlto() : porteriaAmarilla.getAlto();
      Serial.println(alto);
      if (color.checkPlacaDelantera() || alto > 120) 
        salirAdelante(0);
  
      else 
        estado = defender;

  }

  //Si está muy lejos de la portería, regresar
  if (estado == regresar) {
    digitalWrite(Constantes::ledPin, LOW);

    buscarPorteria();
  }

  //Verificar que esté en la linea de la portería
 

  //Seguidor de línea para defender
  if (estado == defender) {
    //Serial.print("defender ");
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
