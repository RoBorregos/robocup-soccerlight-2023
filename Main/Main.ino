//Librerías
#include <ICM_20948.h>
#include "AroIR.h"
#include "Color.h"
#include "Motores.h"
#include "PID.h"
#include "Porteria.h"



//#include "Imu.h"
#include "BNO.h"
BNO gyro;


Motores motoresRobot(2, 23, 25, 3, 29, 27, 4, 22, 24); //robot bno
//Motores motoresRobot(2, 28, 26, 3, 22, 24, 4, 30, 32); //robot imu



//Variables
bool posesion = true;
int velocidades = 180;
String input = "";
char lastP = "i";
int lastSeen = 1;
int limitSwitch = 35;
int limitSwitch2 = 6;
unsigned long ms = 0;
unsigned long ms2 = 0;
int last = 1;


//Objetos
AroIR aroIR;
PID pid;
Color color;
Porteria porteriaAzul;
Porteria porteriaAmarilla;


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
  Serial3.setTimeout(100);
  Serial.setTimeout(100);

  pinMode(limitSwitch, INPUT);
  pinMode(limitSwitch2, INPUT);


  //Iniciar objetos
  motoresRobot.iniciar();
  pid.setKP(0.3);
  gyro.iniciar();
  aroIR.iniciar();
  color.iniciar();
  color.calibrar();
  aroIR.actualizarDatos();


}


//Código para atacante
//LOOP-------------------------------------------------------
void loop() {


  //Verificar si está en la línea y moverse si es necesario
  if (estado == linea) {
    ms2 = millis();
     double angle1 = color.checkForLinea();
    Serial.println(angle1);
    
    if (angle1 != -1)
     salirLinea(angle1);
     
    estado = hasPelota;

  }

  //Revisar si se tiene posesión de la pelota
  if (estado == hasPelota) {
    estado = (isLimit()) ? golPorteria : buscarPelota;
  }

  //Buscar la pelota
  if (estado == buscarPelota) {
    buscar();
  }


  //Ir a la portería con la pelota
  if (estado == golPorteria) {
    ms = millis();
    //actualizarPorterias();
    int x1 = porteriaAzul.getX();
    //Serial.println(x1);
    gol(x1);

  }

  //Pruebas
  if (estado == nada) {
    tests();
  }

  estado = linea;

}






//------------------------------------------------------------------------------------ Funciones de estados -------------------------------------------------------------------------------------



//-------------------------------------ARO IR
//Buscar y moverse según la posición de la pelota
void buscar() {
  int change = correccionesImu();
  aroIR.actualizarDatos();
  double angulo = aroIR.getAngulo();
  
  //Serial.println(aroIR.getStrength());
  
 // Serial.println(angulo);

   if (aroIR.getStrength() == 0) {
    motoresRobot.apagarMotores();
    return;
  }
  

  if (abs(angulo) <= 20) {
    motoresRobot.movimientoLinealCorregido(0, velocidades, change, gyro.isRight());
  } else if (abs(angulo) <= 60) {
    int v = (angulo > 0) ? 90 : -90;
    motoresRobot.movimientoLinealCorregido(v, velocidades, change, gyro.isRight());   
  } else if (abs(angulo) <= 75) {
    int v = (angulo > 0) ? 120 : -120;
    motoresRobot.movimientoLinealCorregido(v, velocidades, change, gyro.isRight());
  } else if (abs(angulo) <= 90) {
    int v = (angulo > 0) ? 135 : -135;
    motoresRobot.movimientoLinealCorregido(v, velocidades, change, gyro.isRight());
  } else if (abs(angulo) <= 240) {
    motoresRobot.movimientoLinealCorregido(180, velocidades, change, gyro.isRight());
  } else {
    motoresRobot.movimientoLinealCorregido(last * 140, velocidades, change, gyro.isRight());
  }
  

   if (angulo > 0)
    last = 1;
   else
    last = -1;
  
}





//---------------------------------------Porterias

void gol(int px) {

   while((millis() - ms) < 500){

      int change = correccionesImu();
      bool r = gyro.isRight();
    
     // Serial.println(px);
     
    
      //Si no ve la portería
      if (px == -1) {
        motoresRobot.movimientoLinealCorregido(0, velocidades, change, r);
    
        Serial.println("Nothing yet");
      }
    
      else {
        if (px > 200) { //Derecha    
          int change = correccionesImuTarget(40);
          motoresRobot.setAllMotorSpeed(velocidades);
          motoresRobot.movimientoLinealCorregido(0,velocidades,change,pid.getR());
              
          Serial.println("der");
    
    
        } else if (px < 80) {  //Izquierda
          int change = correccionesImuTarget(-40);
          motoresRobot.setAllMotorSpeed(velocidades);
          motoresRobot.movimientoLinealCorregido(0,velocidades,change,pid.getR());
          Serial.println("izq");
    
    
        } else {  //Centro
          motoresRobot.movimientoLinealCorregido(0, velocidades, change, r);
    
          Serial.println("Adelante");
    
        }
    
      }
  }

}


//---------------------------------------IMU
//Devolver el error a corregir
int correccionesImu() {
  //actualizarPorterias();
  gyro.readValues();
  int change = pid.calcularError(0, gyro.getYaw(), velocidades);

  return change;
}


int correccionesImuTarget(int target) {
  //actualizarPorterias();
  gyro.readValues();
  //int change = pid.calcularError(target, gyro.getYaw(), velocidades);
  
  int error = abs(target - gyro.getYaw());
  bool r = ((target - gyro.getYaw()) > 0) ? true : false;
  pid.setR(r);
  if (error <= 5) return 0;
  error = 1.3 * error;
  error = min(error,255);
  error = map(error, 0, 255, 110, 255);
  //Serial.println(error);
  return error;
}


    

bool inLinea() {

  double angle1 = color.checkForLinea();
  Serial.println(angle1);
  int change = correccionesImu();

  if (angle1 == -1) {

    return false;

  } else {
    //Serial.println(angle1);

    while ((millis() - ms2) < 500) {
      gyro.readValues();
      motoresRobot.movimientoLinealCorregido(angle1, 190, correccionesImu(), gyro.isRight());
    }

  }
  return true;

}

void salirLinea(int angleC) {
  //double angleC = color.checkForLinea();
  int change = correccionesImu();
  
   while ((millis() - ms2) < 500) {
      gyro.readValues();
      motoresRobot.movimientoLinealCorregido(angleC, 190, change, gyro.isRight());
   }
    
}


bool isLimit() {
  //Serial.println(digitalRead(limitSwitch));
  if (digitalRead(limitSwitch) == 1 || digitalRead(limitSwitch2) == 1) {
    return true;
  }
  return false;
}



//Obtener y almacenar los datos de la cámara
void actualizarPorterias() {

  if (Serial3.available()) {
    String input1 =  Serial3.readStringUntil('\n');
    //Serial.println(input1);

    //Serial.println(input);
    if (input1[0] == '0')
      porteriaAmarilla.actualizar(input1);
    else
      porteriaAzul.actualizar(input1);
  }

}








//__________________________________________________________-Para el estado de pruebas
void tests() {
//     actualizarPorterias();
//        Serial.println(porteriaAzul.getX());


  //  Serial.println("Cam");
  //if (Serial3.available()) {
  //        Serial.println("serial1");
  //         input = Serial3.readStringUntil('\n');
  //        Serial.println(input);
  //
  //    }

  //Serial.println(hasPosesion());


  //ARO-IRRRR________________________________
//       aroIR.actualizarDatos();
//       double angulo = aroIR.getAngulo();
//       Serial.println(angulo);
  //     Serial.println(aroIR.getStrength());


  //CAMARA____________________________________
  //   actualizarPorterias();
  //     Serial.println("po");
  //     Serial.println(porteriaAzul.x);
  //     Serial.println(porteriaAmarilla.x);



  //IMU______________________________________
//       gyro.readValues();
//      Serial.println(gyro.getYaw());

//    int change = correccionesImuTarget(-40);
//    motoresRobot.setAllMotorSpeed(velocidades);
//    motoresRobot.movimientoLinealCorregido(0,velocidades,change,pid.getR());
    //motoresRobot.giro(change, pid.getR());





  //MOTORESS INDIVIDUAL______________________________________
  //     motoresRobot.setAllMotorSpeed(velocidades);
  //     motoresRobot.mover1();
  //     delay(1000);
  //     motoresRobot.mover2();
  //     delay(1000);
  //     motoresRobot.mover3();
  ////
  //   motoresRobot.giroH();




  //LIMIT SWITCH
//  if (digitalRead(limitSwitch) == 1 || digitalRead(limitSwitch2) == 1)
//    Serial.println("1");
//  Serial.println(digitalRead(limitSwitch2));


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

  //MOVIMIENTOLINEALCORREGIDO___________________
//      int change = correccionesImu();
//      //motoresRobot.movimientoLineal(0,velocidades);
//      motoresRobot.movimientoLinealCorregido(0, velocidades, change, gyro.isRight());
  //
}
