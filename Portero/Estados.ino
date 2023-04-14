
//------------------------------------------------------------------------------------ Funciones de estados -------------------------------------------------------------------------------------

void buscarPorteria(int x1, int y1){

    
  int change = correccionesImu();
  //Serial.println(x1);

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
      digitalWrite(led2, HIGH);

      int ang = (error > 0)? 180 - (error*kP) : -180 - (error*kP);
      ang *= -1;
      //Serial.println(ang);
      motoresRobot.movimientoLinealCorregido(ang, velocidades, change, gyro.isRight());
    }

  }
   digitalWrite(led2, LOW);

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

    //Serial.println(input);
    if (input1[0] == '0')
      porteriaAmarilla.actualizar(input1);
    else
      porteriaAzul.actualizar(input1);
  }

}


void salirLinea(int angleC) {
  
  ms2 = millis();

  while ((millis() - ms2) < 500) {
    int change = correccionesImu();
    aroIR.actualizarDatos();
    double angulo = aroIR.getAngulo();

    if (aroIR.getStrength() == 0) {
      motoresRobot.apagarMotores();
      return;
    }

    int result = -1000;

  if (abs(angulo) <= 20) {
    result = 0;
  } else if (abs(angulo) <= 60) {
    result = (angulo > 0) ? 70 : -70;
  } else if (abs(angulo) <= 75) {
    result = (angulo > 0) ? 80 : -80;
  } else if (abs(angulo) <= 90) {
    result = (angulo > 0) ? 90 : -90;
  } else {
    result = angulo;
  }

   if (result == -1000) 
    motoresRobot.apagarMotores();
  else 
    motoresRobot.movimientoLinealCorregido(result, velocidades, change, gyro.isRight());

    
  }

}


bool isLimit() {
  //Serial.println(digitalRead(limitSwitch));
  if (digitalRead(limitSwitch) == 1 || digitalRead(limitSwitch2) == 1) {
    return true;
  }
  return false;
}





void actualizarPorterias2(int col) {
  
  //Serial.println(col);
  if (col == 1)
    Serial3.write("1");
  else 
    Serial3.write("0");

  int ms3 = millis();

  while ((millis() - ms3) < 50) {
    
  }

  if (Serial3.available()) { 
    input =  Serial3.readStringUntil('\n');
    Serial.println(input);

//    if (input[0] == '0')
//      porteriaAmarilla.actualizar(input);
//
//    else if (input[0] == '1')
//      porteriaAzul.actualizar(input);

    
  }
//  while(Serial3.available()) {
//    Serial.read();
//  }

} 






//__________________________________________________________-Para el estado de pruebas
void tests() {
 //    actualizarPorterias();
//       Serial.println(porteriaAzul.getX());

//Serial3.write("1");
//
//int ms3 = millis();
//
//  while ((millis() - ms3) < 10) {
//    
//  }
////Serial3.write("i");
////delay(700);
////
//  if (Serial3.available()) {
//          //Serial.println("serial1");
//           input = Serial3.readStringUntil('\n');
//          //Serial.println(input);
//          porteriaAzul.actualizar(input);
//  
//      }
    //  Serial.println(porteriaAzul.getX());

      

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
         aroIR.actualizarDatos();
        double angulo = aroIR.getAngulo();
        Serial.println(angulo);
//  //     Serial.println(aroIR.getStrength());

//         double high = aroIR.getHighPass();
//         double low = aroIR.getLowPass();
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
