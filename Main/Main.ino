//Librerías
#include <ICM_20948.h>
#include <HTInfraredSeeker.h>
#include "Porteria.h"
#include "Motores.h"
#include "Imu.h"
#include "PID.h"
#include "AroIR.h"
#include "Color.h"
//#include "BNO.h"


//Variables
bool posesion = true;
int velocidades = 230;
String input = "";
char lastP = "i";
int lastSeen = 1;

//Objetos
AroIR aroIR;
PID pid;
//Motores motoresRobot(8, 41, 27, 7, 25, 24, 6, 23, 22);
Motores motoresRobot(2, 29, 27, 3, 23, 25, 4, 22, 24);


Color color;
//Adafruit_BNO055 bno;
Imu gyro;

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

Estados estado = linea;

//SETUP------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);

  //Iniciar objetos
  motoresRobot.iniciar();
  pid.setKP(0.4);
  gyro.iniciar();
  aroIR.iniciar();
  color.iniciar();

}


//Código para atacante
//LOOP-------------------------------------------------------
void loop() {


    
  //Verificar si está en la línea y moverse si es necesario
  if (estado == linea) {
    estado = inLinea()? linea: hasPelota;
    estado = linea;
  }

  //Revisar si se tiene posesión de la pelota
  if (estado == hasPelota) {
    estado = (hasPosesion()) ? golPorteria : buscarPelota;
    // estado = nada;
  }

  //Buscar la pelota
  if (estado == buscarPelota) {
    lastSeen = buscar(lastSeen);
    //lastSeen = seeker(dirSeeker, lastSeen);
  }

  //Ir a la portería con la pelota
  if (estado == golPorteria) {
    actualizarPorterias();
    gol(porteriaAzul, lastP);
  }

  //Pruebas
  if (estado == nada) {
      tests();
  }

  estado = linea;

}


//-------------------------- Funciones de estados --------------------------------------------------


bool inLinea(){
  double angle = color.checkForLinea();
  Serial.println(angle);
  int change = correccionesImu();
  
  if (angle == -1){
    motoresRobot.movimientoLineal(0,velocidades);
    return false;
  } else {
    motoresRobot.movimientoLinealCorregido(angle,velocidades,change,gyro.isRight());
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


//---------------------------------------GOL
//Ir a la portería dependiendo de las lecturas de la OpenMV
char gol(Porteria p, char lastP) {

  //Serial.println(p.x);

  gyro.readValues();
  int change = correccionesImu();

  //Si no ve la portería
  if (p.x == -1) {
    if (change > 0)
      motoresRobot.giro(change, gyro.isRight());
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

  return lastP;

}

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




//-------------------------------------ARO IR
//Buscar y moverse según la posición de la pelota
int buscar(int last) {

 
  int change = correccionesImu();
  aroIR.actualizarDatos();
  double angulo = aroIR.getAngulo();

  //Si no se está viendo la pelota
  if (aroIR.getStrength() == 0) {
    motoresRobot.apagarMotores();
    return;
  }

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
//  
//if (Serial3.available()) {
//        Serial.println("serial1");
//         input = Serial3.readStringUntil('\n');
//        Serial.println(input);
//       
//    }

//Serial.println(hasPosesion());
    
  
  //ARO-IRRRR________________________________
     aroIR.actualizarDatos();
     double angulo = aroIR.getAngulo();
     Serial.println(angulo);
     Serial.println(aroIR.getStrength());

   
  //CAMARA____________________________________
   //  actualizarPorterias(); 
     //Serial.println(porteriaAzul.x);
  

  //IMU______________________________________
  //   gyro.readValues();
  //   Serial.println(gyro.getYaw());


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


  //MOTORESS______________________________________
//     motoresRobot.setAllMotorSpeed(velocidades);
//     motoresRobot.mover1();
//     delay(1000);
//     motoresRobot.mover2();
//     delay(1000);
//     motoresRobot.mover3();
  
  //   motoresRobot.giroH();


  //MOVIMIENTOLINEALCORREGIDO___________________
  //  int change = correccionesImu();
  //  motoresRobot.movimientoLinealCorregido(0, velocidades, change, gyro.isRight());
  
}
