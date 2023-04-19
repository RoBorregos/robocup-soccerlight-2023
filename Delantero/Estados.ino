//-------------------------------------ARO IR
//Buscar y moverse según la posición de la pelota
void buscar() {
  int change = correccionesImu();
  aroIR.actualizarDatos();
  double angulo = aroIR.getAngulo();
  double str = aroIR.getStrength();


  //Serial.println(aroIR.getStrength());

  //Serial.println(angulo);


  if (str == 0) {
    motoresRobot.apagarMotores();
    return;
  } else if (str > 80 && abs(angulo) <= 90) {
    dribbler.prender();
  } else {
    dribbler.apagar();
  }

  int result = -1000;
  

    if (abs(angulo) <= 20) {
      result = 0;
    } else if (abs(angulo) <= 50) {
      result = (angulo > 0) ? 90 : -90;
    } else if (abs(angulo) <= 75) {
      result = (angulo > 0) ? 120 : -120;
    } else if (abs(angulo) <= 90) {
      result = (angulo > 0) ? 135 : -135;
    } else if (abs(angulo) <= 240) {
      result = 180;
    } else {
      result = last*140;
    }

  
  if (str < 30) {
    if (result <= 90)
      result = result * 0.7;
    else if (result <= 180)
      result = (angulo > 0) ? result * 0.85: result * -0.85;
    
  } else if (str < 50) {
    if (result <= 90)
      result = result * 0.7;
    else if (result <= 180)
      result = (angulo > 0) ? result * 0.95: result * -0.95;
  } 

  double distancia = map(str, 10,100, 0, 10);
 // double val = 1 - pow(EULER,(0.9*(distancia-10)));
  double val = 1.087 + 1/((distancia-11.5));
  int velNuevas = velocidades * val;
  velNuevas = max(velMin, velNuevas);

   Serial.print("factor: ");
  Serial.println(distancia);

  // Serial.print("velocidad: ");
  // Serial.println(velNuevas);

  
  

  if (result == -1000) 
    motoresRobot.apagarMotores();
  else 
    motoresRobot.movimientoLinealCorregido(result, velNuevas, change, gyro.isRight());


  if (angulo > 0)
    last = 1;
  else
    last = -1;

}




//---------------------------------------Porterias

void gol(int px,int y1) {
 int change = correccionesImu();

Serial.print("GOL\t\t");
// Si no ve la porteria 
if (px == -1) {
    motoresRobot.movimientoLinealCorregido(170*lastP, velocidades, change, gyro.isRight());
  } else if (px < 10 || px > 310){
    motoresRobot.movimientoLinealCorregido(150*lastP, velocidades, change, gyro.isRight());
  } else {
    atacarGol(px,y1);
  }


 if (px < 160) {
    lastP = -1;
  } else {
    lastP = 1;
  }
 
}

void atacarGol(int px,int y1) {

 // while ((millis() - ms) < 400) {
    //Serial.print("GOL\t\t");

  //Si está muy cercca -> movimientos de gol
  if (y1 > 100) {
    dribbler.apagar();
  }

  if (y1 > 110 && atacarE < 100) {
    atacarE++;
     
    int change = correccionesImu();
    bool r = gyro.isRight();

    
      if (px > 200) { //Derecha
        int change = correccionesImuTarget(40);
        motoresRobot.movimientoLinealCorregido(0, velocidades, change, pid.getR());
        Serial.println("der");


      } else if (px < 120) {  //Izquierda
        int change = correccionesImuTarget(-40);
        motoresRobot.movimientoLinealCorregido(0, velocidades, change, pid.getR());
        Serial.println("izq");


      } else {  //Centro
        motoresRobot.movimientoLinealCorregido(0, velocidades, change, r);
        Serial.println("Adelante");

      }


    } else {

    //Moverse a la porteria
    int change = correccionesImu();
    int error = px - 160;
    double kP = 0.375;

    if (abs(error) < 25) {
      Serial.println("forward");
      motoresRobot.movimientoLinealCorregido(0, velocidades, change, gyro.isRight());
    
    } else {
      int ang = (error > 0)? (error*kP) : (error*kP);
      Serial.println("Move");
      motoresRobot.movimientoLinealCorregido(ang, velocidades, change, gyro.isRight());
    
    }

    if (y1 <= 100)
    atacarE = 0;

  }

 
  
// }
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

bool hasPosesion() {
  aroIR.actualizarDatos();
  double angulo = aroIR.getAngulo();
    // Serial.print(analogRead(analogo));


  if (analogRead(analogo) < 600 && abs(angulo) < 40) {
    return true;
  }
  return false;
}

bool hasP() {
  aroIR.actualizarDatos();
  double angulo = aroIR.getAngulo();
   Serial.print(analogRead(analogo));


  if (aroIR.getStrength() > 85 && abs(angulo) < 20) {
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
 Serial.print(analogRead(analogo));
 Serial.print("\t\t");
 Serial.println(hasPosesion());

    //  actualizarPorterias();
    //   Serial.println(porteriaAzul.getX());


  //  Serial.println("Cam");
  // if (Serial2.available()) {
  //        Serial.println("serial1");
  //         input = Serial2.readStringUntil('\n');
  //        Serial.println(input);
  
  //    }

//  if (Serial3.available()) {
//          //Serial.println("serial1");
//           input = Serial3.readStringUntil('\n');
//          Serial.println(input);
  
//      }
  //Serial.println(hasPosesion());


  //ARO-IRRRR________________________________
      //  aroIR.actualizarDatos();

        // double angulo = aroIR.getAngulo();
        // //Serial.print(angulo);
        // // Serial.print("\t\t");
        //  Serial.println(aroIR.getStrength());

    //     Serial.println("SSSS");

    //     if (Serial2.available()) {
    //       input = Serial2.readStringUntil('\n');
    //      Serial.println(input);
  
    //  }



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
    
   // Serial.println(isLimit());


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
