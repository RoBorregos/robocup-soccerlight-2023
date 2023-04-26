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


//Código para atacante
//LOOP-------------------------------------------------------
void loop() {
  current_time = millis();
  pid.setKP(0.02);
  atacar = amarillo;
  estado = inicio;

  //Calibrar camara
  //Calibrar fotos
  //Cambiar color
  //Cambiar lado a atacar

  // long tiempoInicial = micros();
  // long tiempoInicio = -1;
  // long tiempoLinea = -1;
  // long tiempoRegresar = -1;
  // long tiempoDefender = -1;
  // long despuesDefender = -1;

  actualizarPorterias();
  int largo = (atacar == amarillo) ? porteriaAzul.getLargo() : porteriaAmarilla.getLargo();
  int alto = (atacar == amarillo) ? porteriaAzul.getAlto() : porteriaAmarilla.getAlto();
  int x1 = (atacar == amarillo) ? porteriaAzul.getX() : porteriaAmarilla.getX();

  if (estado == inicio) {
    estado = (alto < 75 || largo < 190) ? regresar : linea;
    //tiempoInicio = micros() - tiempoInicial;
  }

  if (estado == linea) {
      if (color.checkPlacaDelantera() || ultrasonico.getDistancia() < 9) 
        salirAdelante(0);
  
      else 
        estado = defender;

   // tiempoLinea = micros() - tiempoInicial;

  }

  //Si está muy lejos de la portería, regresar
  if (estado == regresar) {
    digitalWrite(Constantes::ledPin, LOW);
   // tiempoRegresar = micros() - tiempoInicial;

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
     // tiempoDefender = micros() - tiempoInicial;

      buscarC();
     // despuesDefender = micros() - tiempoInicial;


  }
  // Serial.print("t0: ");
  // Serial.print(tiempoInicial);
  //   Serial.print("\t");

  // Serial.print("tInicio: " );
  // Serial.print(tiempoInicio);
  //   Serial.print("\t");

  // Serial.print(" tLinea: " );
  // Serial.print(tiempoLinea);
  //   Serial.print("\t");

  // Serial.print(" tRegresar: " );
  // Serial.print(tiempoRegresar);
  //   Serial.print("\t");

  // Serial.print(" tDefender: " );
  // Serial.print(tiempoDefender);
  //  Serial.print("\t\t");

  // Serial.print(" tDespues: " );
  // Serial.println(despuesDefender);

  //Pruebas
  if (estado == nada) {
    tests();
  }

  if (estado == detener) {
    alinear();
  }


}
