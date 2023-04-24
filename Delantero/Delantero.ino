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

Constantes::Lados atacar = Constantes::azul;
Constantes::Estados estado;



//SETUP------------------------------------------------------
void setup() {

  iniciarObjetos();
  delay(1600);
  voltear();  
 
}


//Código para atacante
//LOOP-------------------------------------------------------
void loop() {


  estado = Constantes::linea;

  //Verificar si está en la línea y moverse si es necesario
  if (estado == Constantes::linea) {

    int angle1 = color.checkForLineaPlaca();
    Serial.println(angle1);
    if (distanciaUltrasonico() < 30) {
      angle1 = 0;
    }

    if (angle1 != -1) {
    Serial.println("lineaa");

      salirLinea(angle1);
      //digitalWrite(led, HIGH);
    } else {
      //digitalWrite(led, LOW);
    Serial.println("nada");
     estado = Constantes::hasPelota;

    }
  }

  //Revisar si se tiene posesión de la pelota
  if (estado == Constantes::hasPelota) {
    aroIR.actualizarDatos();

    estado = (detector() > 15 && abs(aroIR.getAngulo()) < 20) ? Constantes::golPorteria : Constantes::buscarPelota;
  }

  //Buscar la pelota
  if (estado == Constantes::buscarPelota) {
    digitalWrite(Constantes::led, LOW);
    Serial.println("buscar");
    buscar();
  }


  //Ir a la portería con la pelota
  if (estado == Constantes::golPorteria) {
    digitalWrite(Constantes::led, HIGH);
    unsigned long ms = millis();
    actualizarPorterias();    
    int x1 = (atacar == Constantes::amarillo) ? porteriaAmarilla.getX() : porteriaAzul.getX();
    int y1 = (atacar == Constantes::amarillo) ? porteriaAmarilla.getY() : porteriaAzul.getY();
    gol(x1,y1);

  }

  //Pruebas
  if (estado == Constantes::nada) {
   tests();
  }


}






//------------------------------------------------------------------------------------ Funciones de estados -------------------------------------------------------------------------------------
