//-------------------------------------ARO IR
//Buscar y moverse según la posición de la pelota
void buscar() {
  int change = correccionesImu();
  aroIR.actualizarDatos();
  double angulo = aroIR.getAngulo();

  //Serial.println(aroIR.getStrength());

   Serial.println(angulo);

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

void gol(int px,int y1) {
  
if (px == -1) {
    motoresRobot.apagarMotores();
    return;
  }
 while ((millis() - ms) < 400) {
  if (y1 > 110 && atacarE < 100) {
    atacarE++;
     
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
 // }
  } else {
    int change = correccionesImu();
    int error = px - 160;
//    Serial.print("x: ");
//    Serial.println(x1);
    
    double kP = 0.375;


    if (abs(error) < 25) {
      Serial.println("forward");
      motoresRobot.movimientoLinealCorregido(0, velocidades, change, gyro.isRight());
    } else {
      //digitalWrite(led2, HIGH);

      int ang = (error > 0)? (error*kP) : (error*kP);
      //ang *= -1;
      Serial.println(ang);
      motoresRobot.movimientoLinealCorregido(ang, velocidades, change, gyro.isRight());
    }
    if (y1 <= 100)
    atacarE = 0;

  
  }

  
 }
}

void golV2(int px,int y1) {
if (px == -1) {
    motoresRobot.apagarMotores();
    return;
  }
  
  if (y1 > 110 && atacarE < 100) {
    atacarE++;
     // while ((millis() - ms) < 500) {
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
 // }
  } else {
    int change = correccionesImu();
    int error = px - 160;
//    Serial.print("x: ");
//    Serial.println(x1);
    
    double kP = 0.3;



      int ang = (error*kP);
      change = correccionesImuTarget(ang);
      motoresRobot.movimientoLinealCorregido(ang, velocidades, change, pid.getR());

      //ang *= -1;
      Serial.println(ang);
    
    if (y1 <= 100)
    atacarE = 0;

  
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
  error = 0.9 * error;
  error = min(error, 255);
  error = map(error, 0, 255, pid.getMinToMove(), 255);
  //Serial.println(error);
  return error;
}




void salirLinea(int angleC) {
  int change = correccionesImu();
  ms2 = millis();

  while ((millis() - ms2) < 500) {
    gyro.readValues();
    motoresRobot.movimientoLinealCorregido(angleC, velocidades, change, gyro.isRight());
  }

}


bool isLimit() {
  Serial.println(digitalRead(limitSwitch));
  if (digitalRead(limitSwitch) == 1) {
    return true;
  }
  return false;
}



//Obtener y almacenar los datos de la cámara
void actualizarPorterias() {


  if (Serial2.available()) {
    String input1 =  Serial2.readStringUntil('\n');
    //Serial.println(input1);

    //Serial.println(input);
    if (input1[0] == 'a')
      porteriaAmarilla.actualizar(input1);
    else if (input1[0] == 'b')
      porteriaAzul.actualizar(input1);
  }

}








//__________________________________________________________-Para el estado de pruebas
void tests() {
    //  actualizarPorterias();
    //   Serial.println(porteriaAzul.getX());


  //  Serial.println("Cam");
  // if (Serial2.available()) {
  //        Serial.println("serial1");
  //         input = Serial2.readStringUntil('\n');
  //        Serial.println(input);
  
  //    }

//  if (Serial3.available()) {
//          Serial.println("serial1");
//           input = Serial3.readStringUntil('\n');
//          Serial.println(input);
  
//      }
  //Serial.println(hasPosesion());


  //ARO-IRRRR________________________________
        // aroIR.actualizarDatos();

        // double angulo = aroIR.getAngulo();
        // Serial.println(angulo);
    //     Serial.println("SSSS");

    //     if (Serial2.available()) {
    //       input = Serial2.readStringUntil('\n');
    //      Serial.println(input);
  
    //  }


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
        //  gyro.readValues();
        // Serial.println(gyro.getYaw());

      // Serial.println(gyro.getMag());

  //    int change = correccionesImuTarget(-40);
  //    motoresRobot.setAllMotorSpeed(velocidades);
  //    motoresRobot.movimientoLinealCorregido(0,velocidades,change,pid.getR());
  //motoresRobot.giro(change, pid.getR());





  //MOTORESS INDIVIDUAL______________________________________
      // motoresRobot.setAllMotorSpeed(velocidades);
      // motoresRobot.mover1();
      // delay(1000);
      // motoresRobot.mover2();
      // delay(1000);
      // motoresRobot.mover3();
  ////
  //   motoresRobot.giroH();




  //LIMIT SWITCH
  //Serial.println(digitalRead(limitSwitch));
    
    Serial.println(isLimit());


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
  // //

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
