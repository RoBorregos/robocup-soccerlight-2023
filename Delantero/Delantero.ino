
//Librerías
#include <ICM_20948.h>
#include "AroIR.h"
#include "Color.h"
#include "Motores.h"
#include "PID.h"
#include "Porteria.h"
#include <HTInfraredSeeker.h>
#include "Dribbler.h"
//#include "Constantes.h"

// int dirSeeker;
// int dirGrados;
// int strSeeker;
// int lastSeen = 1;


//Selección de giroscopio
//Se debe descomentar lo que se va a usar y comentar lo contrario
//
//#include "Imu.h"  
//Motores motoresRobot(2, 28, 26, 3, 22, 24, 4, 30, 32);  //robot imu
//
#include "BNO.h"
//Motores motoresRobot(2, 23, 25, 3, 29, 27, 4, 22, 24);    //robot bno
//Motores motoresRobot(4, 30, 29, 5, 28, 27, 6, 26, 25);    //robot bno
Motores motoresRobot(6, 25, 26, 5, 28, 27, 4, 30, 29);    //robot bno

BNO gyro;

//También se deben cambiar los pines de COLOR.h!!

//Cambiar el color de la portería a la que se va a atacar


//Variables
bool posesion = true;
int velocidades = 110;
String input = "";
int lastP = 1;
//int lastSeen = 1;
int limitSwitch = 40;
int limitSwitch2 = 40;
unsigned long ms = 0;
unsigned long ms2 = 0;
int last = 1;
int led = 37;
double angle1 = -1;
int atacarE = 1;


//Objetos
AroIR aroIR;
PID pid;
Color color;
Porteria porteriaAzul;
Porteria porteriaAmarilla;
Dribbler dribbler(7);


//Estados del robot
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
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial2.setTimeout(100);
  Serial.setTimeout(100);

  Serial3.begin(115200);
  Serial3.setTimeout(100);

  pinMode(limitSwitch, OUTPUT);
  // pinMode(limitSwitch2, INPUT);
  // pinMode(led, OUTPUT);
  
  //Delay para la cámara
  delay(1500);

  //Iniciar objetos
  motoresRobot.iniciar();
  pid.setKP(0.2);
  pid.setMinToMove(30);
  gyro.iniciar();
  aroIR.iniciar();
  color.iniciar();
  aroIR.actualizarDatos();
  color.calibrar();
  dribbler.iniciar();
  //InfraredSeeker::Initialize();

  //Capturar los valores de la cámara (2 veces pq una sola falla jaja)
  actualizarPorterias();
  actualizarPorterias();

  //Verificar si se debe voltear
  if ((atacar == amarillo && porteriaAzul.getX() != -1) || (atacar == azul && porteriaAmarilla.getColor() == 1)) {
    gyro.setOffset(180);
    digitalWrite(led, HIGH);
  }

}


//Código para atacante
//LOOP-------------------------------------------------------
void loop() {
  


  estado = linea;


  //estado = hasPelota;
  //Verificar si está en la línea y moverse si es necesario
  if (estado == linea) {
    angle1 = color.checkForLineaPlaca();

    if (angle1 != -1) {
    Serial.println("lineaa");

      salirLinea(angle1);
      //digitalWrite(led, HIGH);
    } else {
      //digitalWrite(led, LOW);
    //Serial.println("nada");
     estado = golPorteria;

    }

    
  }

  //Revisar si se tiene posesión de la pelota
  if (estado == hasPelota) {
    estado = (hasP()) ? golPorteria : buscarPelota;
  }

  //Buscar la pelota
  if (estado == buscarPelota) {
    buscar();
  }


  //Ir a la portería con la pelota
  if (estado == golPorteria) {
    ms = millis();
    actualizarPorterias();    
    int x1 = (atacar == amarillo) ? porteriaAmarilla.getX() : porteriaAzul.getX();
    int y1 = (atacar == amarillo) ? porteriaAmarilla.getY() : porteriaAzul.getY();
    gol(x1,y1);

  }

  //Pruebas
  if (estado == nada) {
//    Serial.println(digitalRead(limitSwitch));
   tests();
  }

  //estado = golPorteria;

}






//------------------------------------------------------------------------------------ Funciones de estados -------------------------------------------------------------------------------------
