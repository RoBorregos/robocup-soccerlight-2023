
//------------------------------------------------------------------------------------ Funciones de estados -------------------------------------------------------------------------------------

void buscarPorteria(int x1, int y1){

    
  int change = correccionesImu();
        Serial.println(y1);

  if (y1 >= 110) {
    motoresRobot.apagarMotores();
    //return true;
  //} else if (y1 >= 90) {
    //motoresRobot.movimientoLinealCorregido(180, velocidades, change, gyro.isRight());

  } else {

    if (x1 == -1){
        motoresRobot.movimientoLinealCorregido(0, velocidades, change, gyro.isRight());

    }
    int error = x1 - 160;
//    Serial.print("x: ");
//    Serial.println(x1);
    
    double kP = 0.375;


    if (abs(error) < 10) {
      //Serial.println("back");
      motoresRobot.movimientoLinealCorregido(180, velocidades, change, gyro.isRight());
    } else {
      int ang = (error > 0)? 180 - (error*kP) : -180 - (error*kP);
      ang *= -1;
      //Serial.println(ang);
      motoresRobot.movimientoLinealCorregido(ang, velocidades, change, gyro.isRight());
    }

  }
  //return false;
}

//void desplazamiento() {
//  int change = correccionesImu();
//  aroIR.actualizarDatos();
//  double angulo = aroIR.getAngulo();
//
//  //Serial.println(aroIR.getStrength());
//
//  // Serial.println(angulo);
//
//  if (aroIR.getStrength() == 0) {
//    motoresRobot.apagarMotores();
//    return;
//  }
//
//
//  motoresRobot.movimientoLinealCorregido(0, velocidades, change, gyro.isRight());
//}



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
  error = min(error, 255);
  error = map(error, 0, 255, 110, 255);
  //Serial.println(error);
  return error;
}


//Obtener y almacenar los datos de la cámara
void actualizarPorterias() {

  if (Serial3.available()) {
    String input1 =  Serial3.readStringUntil('\n');
    //Serial.println(input1);

    if(input1.length() > 8) {
      if (input1[0] == '0')
        porteriaAmarilla.actualizar(input1);
      else  
        porteriaAzul.actualizar(input1);
    }
    
  }

} 

void salirLinea(int angleC) {
  int change = correccionesImu();
  ms2 = millis();

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












//__________________________________________________________-Para el estado de pruebas
void tests() {
       actualizarPorterias();
       Serial.println(porteriaAzul.getX());

//
//    Serial.println("Cam");
//  if (Serial3.available()) {
//          //Serial.println("serial1");
//           input = Serial3.readStringUntil('\n');
//          Serial.println(input);
//  
//      }

  //Serial.println(hasPosesion());


  //ARO-IRRRR________________________________
//         aroIR.actualizarDatos();
//         double high = aroIR.getHighPass();
//         double low = aroIR.getLowPass();
//         double angulo = aroIR.getAngulo();
//         Serial.println(angulo);
//  //     Serial.println(aroIR.getStrength());
//
//    Serial.print(angulo);
//    Serial.print(",\t\t");
//    Serial.print(high);
//    Serial.print(",\t\t");
//    Serial.println(low);


  //CAMARA____________________________________
//     actualizarPorterias();
//       Serial.println(porteriaAzul.getX());
//       Serial.println(porteriaAmarilla.getX());


  //IMU______________________________________
  //       gyro.readValues();
       // Serial.println(gyro.getYaw());

//       Serial.println(gyro.getMag());

  //    int change = correccionesImuTarget(-40);
  //    motoresRobot.setAllMotorSpeed(velocidades);
  //    motoresRobot.movimientoLinealCorregido(0,velocidades,change,pid.getR());
  //motoresRobot.giro(change, pid.getR());





  //MOTORESS INDIVIDUAL______________________________________
//       motoresRobot.setAllMotorSpeed(velocidades);
//       motoresRobot.mover1();
//       delay(1000);
//       motoresRobot.mover2();
//       delay(1000);
//       motoresRobot.mover3();
  //
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

  //SEEKER
//  InfraredResult InfraredBall = InfraredSeeker::ReadAC(); 
/////  int angulo = InfraredBall.Direction;
//  int strSeeker = InfraredBall.Strength;
//
// // Serial.print("Str: ");
//Serial.println(strSeeker);
//  Serial.print("Angulo: ");
//  Serial.println(angulo);
  //delay(500);
}
