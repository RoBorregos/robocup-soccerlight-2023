// Portero Robocup Soccer Lightweight 2023

//Librerías
#include "AroIR.h"
#include "Color.h"
#include "Motores.h"
#include "PID.h"
#include "Porteria.h"
#include "Imu.h"  
#include "Constantes.h"

//Objetos
Motores motoresRobot(5, 28, 27, 6, 26, 25, 4, 30, 29);    //robot imu
Imu gyro;
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
  iniciarObjetos();
  delay(1500);
  Serial.println("SETUP DONE");

}


//Código para atacante
//LOOP-------------------------------------------------------
void loop() {


  estado = nada;
  //Verificar si está en la línea y moverse si es necesario
  
  if (estado == linea) {
    int angle1 = color.checkForLineaPlaca2();
   
     if (angle1 != -1) {
      Serial.println(angle1);
      digitalWrite(Constantes::ledPin, HIGH);
      
      if (angle1 == 0 || angle1 == 60 || angle1 == -60) {
       flagAdelante = true;
       //salirLinea(angle1, 200);

      } else {
        salirLinea(angle1, 500);
      }


    } else {
      digitalWrite(Constantes::ledPin, LOW);
    }

    estado = inicio;


  }

  if (estado == inicio) {
    actualizarPorterias();
    int y1 = (atacar == amarillo) ? porteriaAzul.getY() : porteriaAmarilla.getY();   
    estado = (flagAdelante && y1 > 95) ? salir : regresar;
  
  }

  if (estado == regresar) {
    Serial.println("regresar");
    buscarPorteria();
    flagAdelante = false;

  }

  if (estado == salir) {
    Serial.println("salir");
    buscarB();
  }

  //Pruebas
  if (estado == nada) {
    tests();
  }



}
