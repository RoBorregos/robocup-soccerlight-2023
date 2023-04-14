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


void buscarSeeker() {
  InfraredResult InfraredBall = InfraredSeeker::ReadAC(); 
  int anguloSeeker = InfraredBall.Direction;
  int strSeeker = InfraredBall.Strength;

  Serial.println(strSeeker);
    
  int change = correccionesImu();
  aroIR.actualizarDatos();
 // double angulo = aroIR.getLowPass();
  double angulo = aroIR.getAngulo();
//
//  //Serial.println(aroIR.getStrength());
//
 // Serial.println(angulo);
//
  if (aroIR.getStrength() == 0) {
    motoresRobot.apagarMotores();
    return;
  }

  int result = -1000;

  if (abs(angulo) <= 20) {
    result = 0;
  } else if (abs(angulo) <= 60) {
    result = (angulo > 0) ? 90 : -90;
  } else if (abs(angulo) <= 75) {
    result = (angulo > 0) ? 120 : -120;
  } else if (abs(angulo) <= 90) {
    result = (angulo > 0) ? 135 : -135;
  } else if (abs(angulo) <= 240) {
    result = 180;
  } else {
    result = last * 140;
  }

  
//    if (abs(angulo) <= 18) {
//      result = 0;
//    } else if (abs(angulo) <= 40) {
//      result = (angulo > 0) ? 80 : -80;
//    } else if (abs(angulo) <= 60) {
//      result = (angulo > 0) ? 90 : -90;
//    } else if (abs(angulo) <= 75) {
//      result = (angulo > 0) ? 120 : -120;
//    } else if (abs(angulo) <= 90) {
//      result = (angulo > 0) ? 135 : -135;
//    } else if (abs(angulo) <= 110) {
//      result = 180;
//    } else {
//      result = last*120;
//      //result = -1000;
//    } 
    
    if (strSeeker > 5 && strSeeker < 30) {
    double prop = map(strSeeker, 1, 40, 0.7, 1);
    if (!(abs(angulo) <= 18 || abs(angulo) >= 90 && abs(angulo) <= 110))
       result = result*prop;
  }

  if (result == -1000) 
    motoresRobot.apagarMotores();
  else 
    motoresRobot.movimientoLinealCorregido(result, velocidades, change, gyro.isRight());


  if (angulo > 0)
    last = 1;
  else
    last = -1;

}





//---------------------------------------Porterias

void gol(int px) {

  while ((millis() - ms) < 500) {

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
        motoresRobot.movimientoLinealCorregido(0, velocidades, change, pid.getR());

        Serial.println("der");


      } else if (px < 80) {  //Izquierda
        int change = correccionesImuTarget(-40);
        motoresRobot.setAllMotorSpeed(velocidades);
        motoresRobot.movimientoLinealCorregido(0, velocidades, change, pid.getR());
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
  error = min(error, 255);
  error = map(error, 0, 255, 110, 255);
  //Serial.println(error);
  return error;
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
         gyro.readValues();
       // Serial.println(gyro.getYaw());

       Serial.println(gyro.getMag());

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
