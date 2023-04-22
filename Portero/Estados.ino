
//------------------------------------------------------------------------------------ Funciones de estados -------------------------------------------------------------------------------------

void buscarPorteria(){
  actualizarPorterias();
  int x1 = (atacar == amarillo) ? porteriaAzul.getX() : porteriaAmarilla.getX();
  int y1 = (atacar == amarillo) ? porteriaAzul.getY() : porteriaAmarilla.getY();

  
  int change = correccionesImu();

  int ang = 180;
  Serial.println(x1);
    if (x1 == -1) {
      ang = 0;
    } if (x1 < 60) {
      ang = 135;
    } else if (x1 > (260)) {
      ang = -135;
    } else {
      ang = 180;
    }

    

      motoresRobot.movimientoLinealCorregido(ang - gyro.getYaw(), velocidades, change, gyro.isRight());




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

void buscarB() {
  aroIR.actualizarDatos2();
  double angulo = aroIR.getAngulo();
  int change = correccionesImu();
  int str = aroIR.getStrength();

  if (str == 0) {
    motoresRobot.apagarMotores();
    return;
  }

  int  ang = (angulo > 0) ? 88 : -88;

  actualizarPorterias();
  int x1 = (atacar == amarillo) ? porteriaAzul.getX() : porteriaAmarilla.getX();

  if (x1 == -1) {
    ang = 0;
  } else if (x1 < 60) {
    ang = -80;
  } else if (x1 > (260)) {
    ang = 80;
  } 

  // int ang = 0;
  // if (abs(angulo) <= 20) {
  //   ang = 0;
  // } else if (abs(angulo) <= 150) {
  //   ang = (angulo > 0) ? 90 : -90;
  // } 
  Serial.println(ang- gyro.getYaw());
  motoresRobot.movimientoLinealCorregido(ang - gyro.getYaw(), velocidades, change, gyro.isRight());


}

void buscarA(int tiempo) {
  ms2 = millis();
  aroIR.actualizarDatos2();
  double angulo = aroIR.getAngulo();
  Serial.println(angulo);
  int change = correccionesImu();


 while ((millis() - ms2) < tiempo) {
  change = correccionesImu();
  aroIR.actualizarDatos2();
  double str = aroIR.getStrength();

  if (str == 0) {
    motoresRobot.apagarMotores();
    return;
  }

  int result = -1000;
  

    if (abs(angulo) <= 20) {
      result = 0;
    } else if (abs(angulo) <= 45) {
      result = (angulo > 0) ? 60 : -60;
    } else if (abs(angulo) <= 65) {
      result = (angulo > 0) ? 70 : -70;
    } else if (abs(angulo) <= 90) {
      result = (angulo > 0) ? 80 : -80;
    } else {
      result = angulo;

    }

    if (result == -1000) 
      motoresRobot.apagarMotores();
    else 
      motoresRobot.movimientoLinealCorregido(result - gyro.getYaw(), velocidades, change, gyro.isRight());

 }

}

void salirLinea(int angleC, int tiempo) {
  int change = correccionesImu();
  ms2 = millis();
  //int t = (angleC == 0) ? 200 : tiempo;

  while ((millis() - ms2) < tiempo) {
    gyro.readValues();

    if (angleC == 120) angleC = angleC*-1;
    else if (angleC == 240) angleC = 120;
    motoresRobot.movimientoLinealCorregido(angleC-gyro.getYaw(), velocidades, change, gyro.isRight());
  }

}

double distanciaUltrasonico() {
   digitalWrite(trigPin, HIGH);
  unsigned long ms3 = micros();

   while ((micros() - ms3) < 10) {
   }

    digitalWrite(trigPin, LOW);


    long t; //timepo que demora en llegar el eco
    long d; //distancia en centimetros
    t = pulseIn(echoPin, HIGH); //obtenemos el ancho del pulso
    d = t / 59;
    Serial.println("ul");
   // delay(100);

    return d;
}

// void salirLinea(int angleC) {
//   Serial.println("salir linea");
//   ms2 = millis();

//   while ((millis() - ms2) < 700) {
//     int change = correccionesImu();
//     aroIR.actualizarDatos2();
//     double angulo = aroIR.getAngulo();

//     if (aroIR.getStrength() == 0) {
//       motoresRobot.apagarMotores();
//       return;
//     }

//     int result = -1000;

//   if (abs(angulo) <= 20) {
//     result = 0;
//     Serial.println("adelante");
//   } else if (abs(angulo) <= 60) {
//     result = (angulo > 0) ? 70 : -70;
//     Serial.println("60");
//   } else if (abs(angulo) <= 75) {
//         Serial.println("75");
//     result = (angulo > 0) ? 80 : -80;
//   } else if (abs(angulo) <= 90) {
//     result = (angulo > 0) ? 90 : -90;
//   } else {
//     result = angulo;
//   }

//    if (result == -1000) 
//     motoresRobot.apagarMotores();
//   else 
//     motoresRobot.movimientoLinealCorregido(result - gyro.getYaw(), velocidades, change, gyro.isRight());

    
//   }

// }


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

void seguidor(int mid, char lado) {
  double k = 0.6;
  double error;
      int change = correccionesImu();
int lectura;

  if (lado == 'd') {
    lectura = color.getValor(2, 2);

     error = 90 + (70-lectura)*k;
  } else {
    lectura = color.getValor(0, 2);
     error = -90 + (70-lectura)*k;
  }
      Serial.println(error);



      motoresRobot.movimientoLinealCorregido(error, velocidades, change, gyro.isRight());

}








//__________________________________________________________-Para el estado de pruebas
void tests() {
Serial.print(color.getValor(0, 0));
Serial.print("\t\t");
seguidor(75,'i');
  // Serial.print(analogRead(analogo));
//  Serial.print("\t\t");
//  Serial.println(hasPosesion());
//     actualizarPorterias();
//       Serial.println(porteriaAzul.getX());

// //Serial3.write("1");
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
//     Serial.println("Cam");
//  if (Serial2.available()) {
//          //Serial.println("serial1");
//           String input = Serial2.readStringUntil('\n');
//          Serial.println(input);
 
//      }

  //Serial.println(hasPosesion());


  //ARO-IRRRR________________________________
        //  aroIR.actualizarDatos2();
        // double angulo = aroIR.getAngulo();
        // Serial.println(angulo);
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
    // actualizarPorterias();
    //   Serial.println(porteriaAmarilla.getX());
//       Serial.println(porteriaAmarilla.getX());


  //IMU______________________________________
      //   gyro.readValues();
      //  Serial.println(gyro.getYaw());

//       Serial.println(gyro.getMag());

  //    int change = correccionesImuTarget(-40);
  //    motoresRobot.setAllMotorSpeed(velocidades);
  //    motoresRobot.movimientoLinealCorregido(0,velocidades,change,pid.getR());
  //motoresRobot.giro(change, pid.getR());



// double angle1 = color.checkForLineaPlaca2();
// Serial.println(angle1);


  //MOTORESS INDIVIDUAL______________________________________
    // motoresRobot.setAllMotorSpeed(velocidades);
    //   motoresRobot.mover1();
    //   delay(1000);
    //   motoresRobot.mover2();
    //   delay(1000);
    //   motoresRobot.mover3();
  // //
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
  // //     //motoresRobot.movimientoLineal(0,velocidades);
  //      // gyro.actualizar();
  //       motoresRobot.movimientoLinealCorregido(90-gyro.getYaw(), velocidades, change, gyro.isRight());
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
