//Librerías
#include <ICM_20948.h>
#include <HTInfraredSeeker.h>
#include "Porteria.h"
#include "Motores.h"

#include "PID.h"
#include "AroIR.h"
#include "Color.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//#include "Imu.h"
#include "BNO.h"


//Variables
bool posesion = true;
int velocidades = 220;
String input = "";
char lastP = "i";
int lastSeen = 1;
bool flag = false;
int limitSwitch = 45;
//Objetos
AroIR aroIR;
PID pid;
//Motores motoresRobot(8, 41, 27, 7, 25, 24, 6, 23, 22);
Motores motoresRobot(2, 29, 27, 3, 23, 25, 4, 22, 24);

unsigned long ms = 0;

Color color;
//Adafruit_BNO055 bno;
BNO gyro;

//Porterias
Porteria porteriaAzul;
Porteria porteriaAmarilla;
Porteria porterias[] = {porteriaAmarilla, porteriaAzul};
int indexColor = 1;

//Estados del robot
enum Estados {
  linea,
  buscarPelota,
  hasPelota,
  golPorteria,
  nada
};

Estados estado = hasPelota;

//SETUP------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  pinMode(limitSwitch, OUTPUT);
  
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  oled.clearDisplay(); // clear display

  oled.setTextSize(1);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(0, 10);        // position to display
  oled.println("Hello World!"); // text to display
  oled.display();               // show on OLED

  

  //Iniciar objetos
  motoresRobot.iniciar();
  pid.setKP(0.3);
  gyro.iniciar();
  aroIR.iniciar();
  color.iniciar();
  aroIR.actualizarDatos();


}


//Código para atacante
//LOOP-------------------------------------------------------
void loop() {

  oled.clearDisplay();
  oled.setCursor(0, 10);        // position to display
  oled.print("Imu: "); 
  oled.print(gyro.getYaw()); // text to display
  oled.display();    
    
  //Verificar si está en la línea y moverse si es necesario
  if (estado == linea) {
    estado = inLinea()? linea: hasPelota;
    estado = linea;
  }

  //Revisar si se tiene posesión de la pelota
  if (estado == hasPelota) {    
    estado = (isLimit()) ? golPorteria : buscarPelota;
    
    //estado = hasPelota;
    // estado = nada;
  }

  //Buscar la pelota
  if (estado == buscarPelota) {
//    if (flag) 
//      estado = golPorteria;
//    else 
      lastSeen = buscar(lastSeen);
    
  }

  //Ir a la portería con la pelota
  if (estado == golPorteria) {
        ms = millis();
       actualizarPorterias();
       lastP = gol(porteriaAzul, lastP);
  }

  //Pruebas
  if (estado == nada) {
      tests();
  }

  estado = hasPelota;

}






//-------------------------- Funciones de estados --------------------------------------------------



//---------------------------------------GOL
//Ir a la portería dependiendo de las lecturas de la OpenMV
//char gol(Porteria *p, char lastP) {  
//
//
//    //actualizarPorterias();
//    //gol(porteriaAzul, lastP);
//    
//  
   
//  return;
//
//   gyro.readValues();
//  int change = correccionesImu();
//  
//  //Si no ve la portería
//  if (p->x == -1) {
//    
//    if (change > 4)
//      motoresRobot.giro(change, gyro.isRight());
//    else
//      motoresRobot.apagarMotores();
//
//    Serial.println("Nothing yet");
//  }  else {
//    if (p->x > 220) { //Derecha
//      motoresRobot.movimientoLinealCorregido(45, velocidades, change, gyro.isRight());
//      // digitalWrite(ledRojo, HIGH);
//      //Serial.println(digitIn);
//      Serial.println("der");
//      lastP  = 'd';
//
//
//    } else if (p->x < 100) {  //Izquierda
//      motoresRobot.movimientoLinealCorregido(-45, velocidades, change, gyro.isRight());
//      //lastSeen = 'i';
//      //digitalWrite(ledVerde, HIGH);
//      //Serial.println(digitIn);
//      Serial.println("izq");
//      lastP = 'i';
//
//
//    } else {  //Centro
//      motoresRobot.movimientoLinealCorregido(0, velocidades, change, gyro.isRight());
//      //digitalWrite(ledVerde, LOW);
//      //digitalWrite(ledRojo, LOW);
//      Serial.println("Adelante");
//
//    }
//
//  }
//
//  return lastP;
//
//}


char gol(Porteria p, char lastP) {
//  oled.setCursor(0, 22);        // position to display
//  oled.print("Porteria: "); 
//  oled.print(p.x); // text to display
//  oled.display(); 
  //Serial.println(p.x);
  
  while((millis() - ms) < 1000){
  gyro.readValues();
  int change = correccionesImu();
  bool r = gyro.isRight();
  Serial.println(change);



  //Si no ve la portería
  if (p.x == -1) {
    if (change > 0)
      motoresRobot.giro(change, r);
    else
      motoresRobot.apagarMotores();

    Serial.println("Nothing yet");
  }

  else {
    if (p.x > 220) { //Derecha
      motoresRobot.movimientoLinealCorregido(45, velocidades, change, gyro.isRight());
      // digitalWrite(ledRojo, HIGH);
      //Serial.println(digitIn);
      Serial.println("der");
      lastP  = 'd';


    } else if (p.x < 100) {  //Izquierda
      motoresRobot.movimientoLinealCorregido(-45, velocidades, change, gyro.isRight());
      //lastSeen = 'i';
      //digitalWrite(ledVerde, HIGH);
      //Serial.println(digitIn);
      Serial.println("izq");
      lastP = 'i';


    } else {  //Centro
      motoresRobot.movimientoLinealCorregido(0, velocidades, change, gyro.isRight());
      //digitalWrite(ledVerde, LOW);
      //digitalWrite(ledRojo, LOW);
      Serial.println("Adelante");

    }

  }
  }

  return lastP;

}

bool isLimit(){
  Serial.println(digitalRead(limitSwitch));
  return (digitalRead(limitSwitch) == 1) ? true : false;
}

//-------------------------------------ARO IR
//Buscar y moverse según la posición de la pelota
int buscar(int last) {

  
  int change = correccionesImu();
  aroIR.actualizarDatos();
  double angulo = aroIR.getAngulo();
  Serial.println(angulo);
  //Serial.println(aroIR.getStrength());

  oled.setCursor(0, 20);        // position to display
  oled.print("Angulo IR: "); 
  oled.print(angulo); // text to display
  oled.display();               // show on OLED



  
  //Si no se está viendo la pelota
  if (aroIR.getStrength() == 0) {
    motoresRobot.apagarMotores();
    return;
  }

//    if (((angulo >= -20 && angulo <= 20) && (aroIR.getStrength() < 31))) {
//      actualizarPorterias();
//      gol(porteriaAzul,lastP); 
//    return;
//  } 

   //Pelota Adelante
  if ((abs(angulo) <= 30)) {
    Serial.println("directo");
    motoresRobot.movimientoLinealCorregido(angulo, velocidades, change, gyro.isRight());

    //Pelota Adelante diagonal
  } else if (abs(angulo) < 45) {
    angulo += (angulo > 0) ? 15 : -15;
    motoresRobot.movimientoLinealCorregido(angulo, velocidades, change, gyro.isRight());

    //Pelota Adelante a los lados
  } else if (abs(angulo) < 85) {
    Serial.println("casi directoo");
    angulo += (angulo > 0) ? 25 : -25;
    motoresRobot.movimientoLinealCorregido(angulo, velocidades, change, gyro.isRight());

    //Pelota a los lados
  } else if (abs(angulo) < 120) {
    Serial.println("Atras");
    motoresRobot.setAllMotorSpeed(velocidades);
    motoresRobot.movimientoLinealCorregido(180, velocidades, change, gyro.isRight());

    //Pelota atrás diagonal
  } else {
    Serial.println("Diagonal");
    motoresRobot.movimientoLinealCorregido(last * 120, velocidades, change, gyro.isRight());
    Serial.println(last * 120);
  }

  //Actualizar el último lado en que se vió la pelota
  if (angulo > 0)
    last = -1;
  else
    last = 1;

  return last;

}
//int buscar(int last) {
//
//  double prevStr = aroIR.getStrength();
// 
//  int change = correccionesImu();
//  aroIR.actualizarDatos();
//  double angulo = aroIR.getAngulo();
//  double str = aroIR.getStrength();
//
//
//     // motoresRobot.apagarMotores();
//
//
//  //Si no se está viendo la pelota
//  if (aroIR.getStrength() == 0) {
//    motoresRobot.apagarMotores();
//    return;
//  }
//
//
////  if (((angulo >= -20 && angulo <= 20) && (str < 31)) || ((prevStr < 20) && (aroIR.getStrength() - prevStr > 20))) {
////    estado = golPorteria;
////    return;
////  } 
//
//  //Pelota Adelante
//  if ((abs(angulo) <= 30)) {
//    Serial.println("directo");
//    motoresRobot.movimientoLinealCorregido(angulo, velocidades, change, gyro.isRight());
//
//    //Pelota Adelante diagonal
//  } else if (abs(angulo) < 45) {
//    angulo += (angulo > 0) ? 15 : -15;
//    motoresRobot.movimientoLinealCorregido(angulo, velocidades, change, gyro.isRight());
//
//    //Pelota Adelante a los lados
//  } else if (abs(angulo) < 85) {
//    Serial.println("casi directoo");
//    angulo += (angulo > 0) ? 25 : -25;
//    motoresRobot.movimientoLinealCorregido(angulo, velocidades, change, gyro.isRight());
//
//    //Pelota a los lados
//  } else if (abs(angulo) < 110) {
//    Serial.println("Atras");
//    motoresRobot.setAllMotorSpeed(velocidades);
//    motoresRobot.movimientoLinealCorregido(180, velocidades, change, gyro.isRight());
//
//    //Pelota atrás diagonal
//  } else {
//    Serial.println("Diagonal");
//    motoresRobot.movimientoLinealCorregido(last * 120, velocidades, change, gyro.isRight());
//    Serial.println(last * 120);
//  }
//
//  //Actualizar el último lado en que se vió la pelota
//  if (angulo > 0)
//    last = -1;
//  else
//    last = 1;
//
//  return last;
//
//}


//---------------------------------------Porterias
//Obtener y almacenar los datos de la cámara
void actualizarPorterias() {
 
    if (Serial3.available()) {
      input =  Serial3.readStringUntil('\n');
            Serial.println(input);

      //Serial.println(input);
      if (input[0] == '0')
        porteriaAmarilla.actualizar(input);
      else
        porteriaAzul.actualizar(input);
    }
    //actualizarPorterias();
    //gol(porteriaAzul, lastP);
  
}



bool inLinea(){
  
  double angle = color.checkForLinea();
  Serial.println(angle);
  int change = correccionesImu();
  
  if (angle == -1){
   // motoresRobot.apagarMotores();
    //motoresRobot.movimientoLineal(0,velocidades);
    return false;
  } else {

    //if (millis() - time_ms > 500) {
//        time_ms = millis();
        motoresRobot.movimientoLineal(angle, velocidades);
            motoresRobot.movimientoLinealCorregido(angle,velocidades,change,gyro.isRight());

    //}
  }
  return true;
  
}

//---------------------------------------IMU
//Devolver el error a corregir
int correccionesImu() {
  gyro.readValues();
  int change = pid.calcularError(0, gyro.getYaw(), velocidades);

  return change;
}

bool salirLinea(int angulo) {
  int change = correccionesImu();
  motoresRobot.movimientoLinealCorregido(angulo, velocidades, change, gyro.isRight());
  //estado = color.checkForLinea() ? linea : hasPelota;

  return true;
}



//---------------------------------------Posesion
//Retornar si se tiene la pelota o no
bool hasPosesion() {
  aroIR.actualizarDatos();
  double str = aroIR.getStrength();
  double angulo = aroIR.getAngulo();

  Serial.println(angulo);

  if ((angulo >= -20 && angulo <= 20) && (str < 21)) {
    Serial.println("true");
    return true;
  }

  Serial.println("nop");
  return false;
}



//---------------------------------------SEEKER
int seeker(int dirSeeker, int last) {
  //long tiempoObjetivo = millis() + 1000;

  //    while(millis() < tiempoObjetivo){

  int dirGrados = -150 + (dirSeeker * 30);
  //Serial.println(dirSeeker);

  Serial.println(dirGrados);

  if (dirSeeker == 0) {
    Serial.println("Diagonal");
    motoresRobot.movimientoLineal(last * 120, velocidades);
    Serial.println(last * 120);


  } else if ((abs(dirGrados) <= 30)) {
    Serial.println("directo");
    motoresRobot.movimientoLineal(dirGrados, velocidades);

  } else if ((dirGrados != 0) && (abs(dirGrados) < 90)) {
    Serial.println("casi directoo");
    dirGrados += (dirGrados > 0) ? 20 : -20;
    motoresRobot.movimientoLineal(dirGrados, velocidades);

  }

  //Transformar resultado del seeker a grados
  else {
    Serial.println("Atras");
    motoresRobot.setAllMotorSpeed(velocidades);
    motoresRobot.atras();

  }


  if (dirGrados > 0)
    last = -1;
  else
    last = 1;

  return last;

}


//Para el estado de pruebas
void tests() {
//  Serial.println("Cam");
//if (Serial3.available()) {
//        Serial.println("serial1");
//         input = Serial3.readStringUntil('\n');
//        Serial.println(input);
//       
//    }

//Serial.println(hasPosesion());
    
  
  //ARO-IRRRR________________________________
//     aroIR.actualizarDatos();
//     double angulo = aroIR.getAngulo();
     //Serial.println(angulo);
//     Serial.println(aroIR.getStrength());

   
  //CAMARA____________________________________
//     actualizarPorterias(); 
//     Serial.println("po");
//     Serial.println(porteriaAzul.x);
//     Serial.println(porteriaAmarilla.x);

  

  //IMU______________________________________
     gyro.readValues();
    Serial.println(gyro.getYaw());

  int change = correccionesImu();
  motoresRobot.giro(change, gyro.isRight());


  //MOTORES GIRO______________________________
  //   motoresRobot.setAllMotorSpeed(velocidades);
  //   motoresRobot.giroH();
  //   delay(1000);
  //   motoresRobot.apagarMotores();
  //   delay(1000);
  //   motoresRobot.giroAH();
  //   delay(1000);
  //   motoresRobot.apagarMotores();
  //   delay(1000);


  //MOTORESS INDIVIDUAL______________________________________
//     motoresRobot.setAllMotorSpeed(velocidades);
//     motoresRobot.mover1();
//     delay(1000);
//     motoresRobot.mover2();
//     delay(1000);
//     motoresRobot.mover3();
//  
  //   motoresRobot.giroH();


  //MOVIMIENTOLINEALCORREGIDO___________________
//    int change = correccionesImu();
//    motoresRobot.movimientoLineal(0,velocidades);
//    motoresRobot.movimientoLinealCorregido(0, velocidades, change, gyro.isRight());
  
}
