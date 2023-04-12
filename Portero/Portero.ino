#include <ICM_20948.h>

//Librerías
//#include <ICM_20948.h>
#include "AroIR.h"
#include "Color.h"
#include "Motores.h"
#include "PID.h"
#include "Porteria.h"
#include <HTInfraredSeeker.h>

int dirSeeker;
int dirGrados;
int strSeeker;
int lastSeen = 1;


//Selección de giroscopio
//Se debe descomentar lo que se va a usar y comentar lo contrario
//
//#include "Imu.h"  
//Motores motoresRobot(2, 28, 26, 3, 22, 24, 4, 30, 32);  //robot imu
//
#include "BNO.h"
Motores motoresRobot(2, 23, 25, 3, 29, 27, 4, 22, 24);    //robot bno

BNO gyro;

//También se deben cambiar los pines de COLOR.h!!

//Cambiar el color de la portería a la que se va a atacar


//Variables
bool posesion = true;
int velocidades = 180;
String input = "";
char lastP = "i";
//int lastSeen = 1;
int limitSwitch = 35;
int limitSwitch2 = 6;
unsigned long ms = 0;
unsigned long ms2 = 0;
int last = 1;
int led = 37;
double angle1 = -1;


//Objetos
//AroIR aroIR;
PID pid;
Color color;
Porteria porteriaAzul;
Porteria porteriaAmarilla;


//Estados del robot
enum Estados {
  linea,
  inPorteria,
  buscarPelota,
  hasPelota,
  golPorteria,
  nada
};

enum Lados {
  amarillo = 0,
  azul = 1
};


Lados atacar = amarillo;

Estados estado = linea;



//SETUP------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  Serial3.setTimeout(50);
  Serial.setTimeout(100);

  pinMode(limitSwitch, INPUT);
  pinMode(limitSwitch2, INPUT);
  pinMode(led, OUTPUT);
  
  //Delay para la cámara
  delay(1500);

  //Iniciar objetos
  motoresRobot.iniciar();
  pid.setKP(0.3);
  gyro.iniciar();
  //aroIR.iniciar();
  color.iniciar();
  //aroIR.actualizarDatos();
  color.calibrar();
  //InfraredSeeker::Initialize();

  //Capturar los valores de la cámara (2 veces pq una sola falla jaja)
  actualizarPorterias();
  actualizarPorterias();

  //Verificar si se debe voltear
  if ((atacar == azul && porteriaAzul.getX() != -1) || (atacar == amarillo && porteriaAmarilla.getColor() == 1)) {
    gyro.setOffset(180);
    digitalWrite(led, HIGH);
  }

  Serial.println("SETUP DONE");


}


//Código para atacante
//LOOP-------------------------------------------------------
void loop() {




  //estado = hasPelota;
  //Verificar si está en la línea y moverse si es necesario
  if (estado == linea) {
    angle1 = color.checkForLinea();

    if (angle1 != -1) {
      //Serial.println(angle1);
      salirLinea(angle1);
      digitalWrite(led, HIGH);
      
    } else {
      digitalWrite(led, LOW);
      Serial.println("nada");
      estado = inPorteria;

    }


     
    
  }

  if (estado == inPorteria) {
    actualizarPorterias();

    int x1 = (atacar == azul) ? porteriaAmarilla.getX() : porteriaAzul.getX();
    int y1 = (atacar == azul) ? porteriaAmarilla.getY() : porteriaAzul.getY();
//    Serial.print("x: ");
//    Serial.println(x1);
//    Serial.print("y: ");
//    Serial.println(y1);

    buscarPorteria(x1,y1);
    //estado = (buscarPorteria(x1,y1)) ? linea : inPorteria;
    //estado = inPorteria;
  }


  //Pruebas
  if (estado == nada) {
    tests();
  }

  estado = linea;


}
