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
Motores motoresRobot(5, 28, 27, 6, 26, 25, 4, 30, 29);    //robot imu
Ultrasonico ultrasonico(10, 36);
Ultrasonico ultrsonicoD(A4,A5);
Ultrasonico ultrsonicoI(8,50);
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
        nada
    };

    enum Lados {
        amarillo = 0,
        azul = 1
    };    

Lados atacar = amarillo;
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


  estado = inicio;
  //Verificar si está en la línea y moverse si es necesario
  if (estado == inicio) {
    actualizarPorterias();
    int y1 = (atacar == amarillo) ? porteriaAzul.getY() : porteriaAmarilla.getY();
   Serial.println(y1);

    if (y1 < 80)
      estado = regresar;
    
    else 
      estado = linea;
    //flagAdelante = false;

  }

  if (estado == linea) {
      int angle1 = color.checkForLineaPlaca2();
      if (angle1 == 180) {
        salirAdelante(0);
      else if (checkUltrasonicos() && ultrasonicoD.getDistancia < 50)
        salirAdelante(-90);
      else if (checkUltrasonicos() && ultrasonicoI.getDistancia < 50)
        salirAdelante(90);
     } else {
        estado = defender;
    }

  }

  if (estado == regresar) {
    //Serial.println("regresar");
    digitalWrite(Constantes::ledPin, LOW);
    buscarPorteria();
  }

  if (estado == defender) {
    Serial.println("defender");
    buscarC();
    digitalWrite(Constantes::ledPin, HIGH);

  }


  // if (estado == linea) {
  //   int angle1 = color.checkForLineaPlaca2();
   
  //    if (angle1 != -1) {
  //     Serial.println(angle1);
  //     digitalWrite(Constantes::ledPin, HIGH);
      
  //     if (angle1 == 0 || angle1 == 60 || angle1 == -60) {
  //      flagAdelante = true;
  //      //salirLinea(angle1, 200);

  //     } else {
  //       salirLinea(angle1, 500);
  //     }


  //   } else {
  //     digitalWrite(Constantes::ledPin, LOW);
  //   }

  //   estado = inicio;


  // }

  // if (estado == inicio) {
  //   actualizarPorterias();
  //   int y1 = (atacar == amarillo) ? porteriaAzul.getY() : porteriaAmarilla.getY();   
  //   estado = (flagAdelante && y1 > 95) ? salir : regresar;
  
  // }


  if (estado == salir) {
    Serial.println("salir");
    //buscar();
  }


  //Pruebas
  if (estado == nada) {
    tests();
  }



}
