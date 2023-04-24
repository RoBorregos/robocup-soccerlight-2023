//Setup de objetos
void iniciarObjetos(){ 
  pinMode(Constantes::ledPin, OUTPUT);
  motoresRobot.iniciar();
  gyro.iniciar();
  aroIR.iniciar2();
  aroIR.actualizarDatos2();
  pid.setKP(0.1);
  pid.setMinToMove(40);
  color.iniciar();
  ultrasonico.iniciar();
  color.calibrar();

  if (Constantes::velocidades > 120) {
    pid.setAngle(120);
    pid.setKP(0.1);
  }
}

//Verificar si se debe voltear
void voltear() {
  actualizarPorterias();
  actualizarPorterias();

  if ((atacar == amarillo && porteriaAzul.getX() == -1) || (atacar == azul && porteriaAmarilla.getX() == -1)) {
    gyro.setOffset(180);
    Serial.println("Voltear");
  }

}

//Ir a la portería
void buscarPorteria(){
  actualizarPorterias();
  int x1 = (atacar == amarillo) ? porteriaAzul.getX() : porteriaAmarilla.getX();
  int y1 = (atacar == amarillo) ? porteriaAzul.getY() : porteriaAmarilla.getY();

  int change = correccionesImu();

  int ang = 180;
  Serial.println(x1);
    if (x1 == -1) {
      ang = 0;
      Serial.println("No hay porteria");
    } else if (x1 < 80) {
      ang = 135;
      Serial.println("Ir a la derecha");
    } else if (x1 > (240)) {
      ang = -135;
      Serial.println("Ir a la izquierda");
    } else {
      ang = 180;
      Serial.println("Ir atras");
    }

      motoresRobot.movimientoLinealCorregido(ang-gyro.getYaw(), Constantes::velocidades, change, gyro.isRight());

}


//Devolver el error a corregir
int correccionesImu() {
  gyro.readValues();
  int change = pid.calcularError(0, gyro.getYaw(), Constantes::velocidades);

  return change;
}

//Devolver ángulo a corregir si se busca un ángulo específico
int correccionesImuTarget(int target) {
  gyro.readValues();
  int error = abs(target - gyro.getYaw());
  bool r = ((target - gyro.getYaw()) > 0) ? true : false;
  pid.setR(r);
  if (error <= 5) return 0;
  error = 1.3 * error;
  error = min(error, 255);
  error = map(error, 0, 255, 110, 255);

  return error;
}


//Obtener y almacenar los datos de la cámara
void actualizarPorterias() {

  if (Serial2.available()) {
    String input1 =  Serial2.readStringUntil('\n');

    if (input1[0] == 'a')
      porteriaAmarilla.actualizar(input1);
    else if (input1[0] == 'b')
      porteriaAzul.actualizar(input1);
  }

}


void buscarC() {
  aroIR.actualizarDatos2();
  double angulo = aroIR.getAngulo();
  double distancia = ultrasonico.getDistancia();
  actualizarPorterias();
  int x1 = (atacar == amarillo) ? porteriaAzul.getX() : porteriaAmarilla.getX();
  int largo = (atacar == amarillo) ? porteriaAzul.getLargo() : porteriaAmarilla.getLargo();
  // Serial.print(distancia);
  // Serial.print("\t\t");
  // Serial.println(x1);


  if (aroIR.getStrength() == 0) {
    motoresRobot.apagarMotores();
    return;
  }

  bool derecha = true;
  if (largo < 110) {
    if (x1 > 200) 
      seguidorI();
    else if (x1 < 120)
      seguidorD();
  
  //if ((largo < 110) && ((x1 > 200 && angulo < 0) || (x1 < 120 && angulo >= 0))){
         //debe ir a la izquierda
     // motoresRobot.apagarMotores();
    //   seguidorI();
    // else if (x1 < 120) //debe ir a la derecha
    //   seguidorD();

  } else {
      (angulo > 0) ? seguidorD() : seguidorI();

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
  motoresRobot.movimientoLinealCorregido(ang - gyro.getYaw(), Constantes::velocidades, change, gyro.isRight());


}

void buscarA(int tiempo) {
  unsigned long ms2 = millis();
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
      motoresRobot.movimientoLinealCorregido(result - gyro.getYaw(), Constantes::velocidades, change, gyro.isRight());

 }

}


//Salir de la linea
void salirAdelante(int angleC) {
  int change = correccionesImu();
  unsigned long ms2 = millis();

  while ((millis() - ms2) < 400) {
    gyro.readValues();  
    motoresRobot.movimientoLinealCorregido(angleC-gyro.getYaw(), Constantes::velocidades, change, gyro.isRight());
  }

}

//Seguidor de linea (a la derecha)
void seguidorD() {
  double k = 0.6;
  Serial.println("D");

  int change = correccionesImu();
  int lectura = color.getValor(2, 2); //Foto transistor 2 placa 2 (derecha)
  double error = 90 + (80-lectura)*k;
  error = min(error, 100);
  error = max(80,error);
  int vel = getErrorVelocidades(1);
  motoresRobot.movimientoLinealCorregido(error - gyro.getYaw(), vel, change, gyro.isRight());


}

double getErrorVelocidades(int signo) {
  aroIR.actualizarDatos2();
  double angulo = aroIR.getAngulo();
  double k = 0.3;
  if (abs(angulo) > 90) angulo = 90;
  double error = (90 - signo*angulo)*k;
  error = Constantes::velocidades - abs(error);
  Serial.print(error);
  Serial.print("\t\t");
  Serial.println(angulo);

  //error = max(error,Constantes::minVelocidad);
  return error;
}

//Seguidor de linea (a la izquierda)
void seguidorI() {
  double k = 0.6;
  Serial.println("I");
  int change = correccionesImu();
  int lectura = color.getValor(0, 1); //Foto transistor 1 placa 0 (izquierda)
  double error = -90 - (70-lectura)*k;
  int vel = getErrorVelocidades(-1);
  motoresRobot.movimientoLinealCorregido(error - gyro.getYaw(), vel, change, gyro.isRight());
}

//Calcular el valor medio de los fotos
double getMid(double verde, double blanco) {
  return (verde + (blanco - verde)/2);
}



//__________________________________________________________-Para el estado de pruebas
void tests() {
  
//CAMARA____________________________________
//     actualizarPorterias();
//     // Serial.println(porteriaAmarilla.getX());
//       int x1 = (atacar == 0)? porteriaAzul.getX() : porteriaAmarilla.getX();
// Serial.println(x1);

    // int change = correccionesImu();
    // motoresRobot.movimientoLinealCorregido(0 - gyro.getYaw(), Constantes::velocidades, change, gyro.isRight());

    //  delay(800);
    //  seguidorI();
    //  delay(800);

    // int x = porteriaAmarilla.getX();
    //   Serial.println(porteriaAmarilla.getX());



//   int placa = 2;
//   int f = 2;

// //int mid = getMid(50,150);
// int mid = getMid(40,130);
// //Serial.println(mid);
// Serial.println(color.getValor(2, 2));
// // Serial.print("\t\t");
// seguidorD();

actualizarPorterias();
  int largo = porteriaAzul.getLargo();
  Serial.println(largo);

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
    // motoresRobot.setAllMotorSpeed(Constantes::velocidades);
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
  //       motoresRobot.movimientoLinealCorregido(-45-gyro.getYaw(), Constantes::velocidades, change, gyro.isRight());
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
