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

SingleEMAFilter<int> filterAnalogo(0.6);
Motores motoresRobot(6, 25, 26, 5, 28, 27, 4, 30, 29);    //robot bno
BNO gyro;

//Variables
int lastP = 1;
int last = 1;
int atacarE = 1;


//Objetos
AroIR aroIR;
PID pid;
Color color;
Porteria porteriaAzul;
Porteria porteriaAmarilla;
Dribbler dribbler(7);
Ultrasonico ultrasonico(36,38);

enum Estados {
        linea,
        buscarPelota,
        hasPelota,
        golPorteria,
        nada
};

       enum Lados {
        amarillo = 0,
        azul = 1
};


Lados atacar = azul;
Estados estado;



//SETUP------------------------------------------------------
void setup() {
  Serial3.begin(115200);
  Serial3.setTimeout(100);
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial2.setTimeout(100);

  iniciarObjetos();
  delay(1600);
  voltear();  
 
}


//Código para atacante
//LOOP-------------------------------------------------------
void loop() {


  estado = linea;

  //Verificar si está en la línea y moverse si es necesario
  if (estado == linea) {

    int angle1 = color.checkForLineaPlaca();
    Serial.println(angle1);

    if (ultrasonico.getDistancia() < 30) {
      angle1 = 0;
    }

    if (angle1 != -1) {
    Serial.println("lineaa");

      salirLinea(angle1);
      digitalWrite(Constantes::led, HIGH);
    } else {
      digitalWrite(Constantes::led, LOW);
    Serial.println("nada");
     estado = hasPelota;

    }

  }

  //Revisar si se tiene posesión de la pelota
  if (estado == hasPelota) {
    aroIR.actualizarDatos();

    estado = (detector() > 5 && abs(aroIR.getAngulo()) < 20) ? golPorteria : buscarPelota;
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
    unsigned long ms = millis();
    actualizarPorterias();    
    int x1 = (atacar == amarillo) ? porteriaAmarilla.getX() : porteriaAzul.getX();
    int y1 = (atacar == amarillo) ? porteriaAmarilla.getY() : porteriaAzul.getY();
    gol(x1,y1);

  }

  //Pruebas
  if (estado == nada) {
   tests();
  }


}






//------------------------------------------------------------------------------------ Funciones de estados -------------------------------------------------------------------------------------
