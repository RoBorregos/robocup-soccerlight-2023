//#include <ICM_20948.h>

//Librerías
//#include <ICM_20948.h>
#include "AroIR.h"
#include "Color.h"
#include "Motores.h"
#include "PID.h"
#include "Porteria.h"
//#include <HTInfraredSeeker.h>



//Selección de giroscopio
//Se debe descomentar lo que se va a usar y comentar lo contrario
//
#include "Imu.h"  
//Motores motoresRobot(2, 28, 26, 3, 22, 24, 4, 30, 32);  //robot imu
//
//#include "BNO.h"
//Motores motoresRobot(2, 23, 25, 3, 29, 27, 4, 22, 24);    //robot bno
Motores motoresRobot(5, 28, 27, 6, 26, 25, 4, 30, 29);    //robot imu
//Motores motoresRobot(6, 25, 26, 5, 28, 27, 4, 30, 29);    //robot bno


Imu gyro;

//También se deben cambiar los pines de COLOR.h!!

//Cambiar el color de la portería a la que se va a atacar
int velocidades = 90;
int velMin = 80;

//Variables
bool posesion = true;
String input = "";
char lastP = 1;
//int lastSeen = 1;
int limitSwitch = 35;
int limitSwitch2 = 6;
unsigned long ms = 0;
unsigned long ms2 = 0;
int last = 1;
int led = 9;
int led2 = 39;

double angle1 = -1;
bool flagAdelante = false;

int trigPin = 10;
int echoPin = 36;




//Objetos
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


Lados atacar = azul;
Estados estado;



//SETUP------------------------------------------------------
void setup() {

  Serial.begin(9600);
  Serial2.begin(9600);
  Serial2.setTimeout(100);
  Serial.setTimeout(100);

  pinMode(limitSwitch, INPUT);
  pinMode(led, OUTPUT);

  
  //Delay para la cámara
  delay(1500);

  //Iniciar objetos
  motoresRobot.iniciar();


  gyro.iniciar();

  //AROOO
  aroIR.iniciar2();
  aroIR.actualizarDatos2();
  //  aroIR.iniciar();
  // aroIR.actualizarDatos();


  pid.setKP(0.1);
  pid.setMinToMove(40);



  color.iniciar();
  color.calibrar();

  if (velocidades > 120) {
    pid.setAngle(120);
    pid.setKP(0.1);
  }

  actualizarPorterias();
  actualizarPorterias();

  //Verificar si se debe voltear
  // if ((atacar == amarillo && porteriaAzul.getX() == -1) || (atacar == azul && porteriaAmarilla.getX() == -1)) {
  //   gyro.setOffset(180);
  //   //digitalWrite(led, HIGH);
  //   Serial.println("Voltear");
  // }


  Serial.println("SETUP DONE");


}


//Código para atacante
//LOOP-------------------------------------------------------
void loop() {


  estado = nada;
  //Verificar si está en la línea y moverse si es necesario
  if (estado == linea) {
    angle1 = color.checkForLineaPlaca2();
   
     if (angle1 != -1) {
      Serial.println(angle1);
      digitalWrite(led, HIGH);
      
      if (angle1 == 0 || angle1 == 60 || angle1 == -60) {
       flagAdelante = true;
       //salirLinea(angle1, 200);

      } else {
        salirLinea(angle1, 500);

      }


    } else {
      digitalWrite(led, LOW);
    }

    estado = inicio;


  }

  if (estado == inicio) {
    actualizarPorterias();
    int y1 = (atacar == amarillo) ? porteriaAzul.getY() : porteriaAmarilla.getY();   
    //Serial.println(y1);
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

  // if (estado == linea) {

  //   angle1 = color.checkForLineaPlaca();
  //   actualizarPorterias();
  //    int y1 = (atacar == amarillo) ? porteriaAzul.getY() : porteriaAmarilla.getY();
   
  //   if (angle1 != -1) { //y1  > 97
  //   //Serial.println(angle1);
  //   digitalWrite(led, HIGH);
  //    //buscarA(400);
  //    buscarB();
  //    // motoresRobot.apagarMotores();
      
  //   } else {
  //     digitalWrite(led, LOW);
  //     Serial.println("nada");
  //     motoresRobot.apagarMotores();
  //     //estado = inPorteria;

  //   }

 
  // }

  // if (estado == buscarPelota) {
  //   buscarB();
  // }

  // if (estado == inPorteria) {
  //   //Serial.println("inPorteria");
  //   actualizarPorterias();
  //    int x1 = (atacar == amarillo) ? porteriaAzul.getX() : porteriaAmarilla.getX();
  //    int y1 = (atacar == amarillo) ? porteriaAzul.getY() : porteriaAmarilla.getY();
  //   //Serial.println(x1);
  //   //motoresRobot.apagarMotores();
  //   buscarPorteria(x1,y1);
  //   //estado = (buscarPorteria(x1,y1)) ? linea : inPorteria;
  // }


  //Pruebas
  if (estado == nada) {
    tests();
  }



}
