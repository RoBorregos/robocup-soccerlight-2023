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
    } else if (abs(angulo) <= 140) {
      result = 180;
    } else {
      result = (angulo > 0) ? -140 : 140;
    }

  
  if (str < 30) {
    if (result <= 90)
      result = result * 0.7;
    //else if (result <= 180)
      //result = (angulo > 0) ? result * 0.85: result * -0.85;
    
  } else if (str < 50) {
    if (result <= 90)
      result = result * 0.7;
    //else if (result <= 180)
      //result = (angulo > 0) ? result * 0.95: result * -0.95;
  } 

//   double distancia = map(str, 10,100, 0, 10);
//  // double val = 1 - pow(EULER,(0.9*(distancia-10)));
//   double val = 1.087 + 1/((distancia-11.5));
//   int velNuevas = velocidades * val;
//   velNuevas = max(velMin, velNuevas);

  //Serial.print("factor: ");
  //Serial.println(distancia);

  // Serial.print("velocidad: ");
  // Serial.println(velNuevas);

  if (result == -1000) 
    motoresRobot.apagarMotores();
  else 
    motoresRobot.movimientoLinealCorregido(result - gyro.getYaw(), Constantes::velocidades, change, gyro.isRight());


  if (angulo > 0)
    last = 1;
  else
    last = -1;

}



//Estado para buscar la portería 
void gol() {
  actualizarPorterias();    
  int px = (atacar == amarillo) ? porteriaAmarilla.getX() : porteriaAzul.getX();
  int y1 = (atacar == amarillo) ? porteriaAmarilla.getY() : porteriaAzul.getY();
  int change = correccionesImu();

  Serial.print("GOL\t\t");

  // Si no ve la porteria o si está muy en las esquinas
  if (px == -1) {
      motoresRobot.movimientoLinealCorregido(170*lastP, Constantes::velocidades, change, gyro.isRight());
  } else if (px < 10 || px > 310){
      motoresRobot.movimientoLinealCorregido(150*lastP, Constantes::velocidades, change, gyro.isRight());
  } else {
      atacarGol(px,y1); 
  }


  if (px < 160) 
      lastP = -1;
  else 
      lastP = 1;
    
 
}

//Movimientos específicos para atacar a la portería
void atacarGol(int px,int y1) {
  int change = correccionesImu();

  //Movimientos para cuando esté muy cerca a la portería --> giros más bruscos
  if (y1 > 95) {
    dribbler.apagar();
  }

  if (y1 == -1) {
    motoresRobot.movimientoLinealCorregido(180-gyro.getYaw(), Constantes::velocidades, change, pid.getR());

  }

  if (y1 > 110 && atacarE < 100) {
    atacarE++;
     
    bool r = gyro.isRight();

      if (px > 200) { //Derecha
        int change = correccionesImuTarget(40);
        motoresRobot.movimientoLinealCorregido(0, Constantes::velocidades, change, pid.getR());
        Serial.println("der");


      } else if (px < 120) {  //Izquierda
        int change = correccionesImuTarget(-40);
        motoresRobot.movimientoLinealCorregido(0, Constantes::velocidades, change, pid.getR());
        Serial.println("izq");


      } else {  //Centro
        motoresRobot.movimientoLinealCorregido(0-gyro.getYaw(), Constantes::velocidades, change, r);
        Serial.println("Adelante");

      }


    //Movimientos para cuando esté muy lejos de la portería
    } else {

    //Moverse a la porteria
    int change = correccionesImu();
    int error = px - 160;
    double kP = 0.375;

    if (abs(error) < 35) {
      Serial.println("forward");
      motoresRobot.movimientoLinealCorregido(0 - gyro.getYaw(), Constantes::velocidades, change, gyro.isRight());
    
    } else {
      int ang = (error > 0)? (error*kP) : (error*kP);
      Serial.println("Move");
      motoresRobot.movimientoLinealCorregido(ang - gyro.getYaw(), Constantes::velocidades, change, gyro.isRight());
    
    }

    //Flag
    if (y1 <= 100)
      atacarE = 0;

  }

 
}


//Devolver el error a corregir
int correccionesImu() {
  gyro.readValues();
  int change = pid.calcularError(0, gyro.getYaw(), Constantes::velocidades);
  return change;
}


//Devolver ángulo a corregir si se busca un ángulo específico
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



//Moverse en dirección contraria a la linea detectada
void salirLinea() {
  int angle1 = color.checkForLineaPlaca();
  int change = correccionesImu();
  unsigned long ms2 = millis();

  while ((millis() - ms2) < 700) {
    gyro.readValues();
    motoresRobot.movimientoLinealCorregido(angle1, Constantes::velocidades, change, gyro.isRight());
  }

}


//Detección de la pelota para saber si se tiene la posesión
int detector() {
  int pulseWidth = 0;
  int deltaPulseWidth = 5;

    const unsigned long startTime_us = micros();
    do {       
      //Serial.println(analogRead(analogo));
            filterAnalogo.AddValue(analogRead(Constantes::analogo));
            if(filterAnalogo.GetLowPass() > 600) {
                pulseWidth += deltaPulseWidth;
            }
        
    } while((micros() - startTime_us) < 833);

  return pulseWidth;

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


//Detectar si se está en linea o no
bool inLinea() {
  int angle1 = color.checkForLineaPlaca();
    Serial.println(angle1);

    if (ultrasonico.getDistancia() < 30) 
      angle1 = 0;

    if (angle1 != -1) {
      Serial.println("lineaa");
      digitalWrite(Constantes::led, HIGH);
      return true;

    } else {
      digitalWrite(Constantes::led, LOW);
      Serial.println("nada");
      return false;
    }
}

//Set up de objetos 
void iniciarObjetos() {
  
  dribbler.iniciar();
  pid.setKP(0.2);
  pid.setMinToMove(40);
  gyro.iniciar();
  aroIR.iniciar();
  color.iniciar();
  aroIR.actualizarDatos();
  color.calibrar();
  motoresRobot.iniciar();
  ultrasonico.iniciar();


  pinMode(Constantes::analogo, INPUT);
  pinMode(Constantes::led, OUTPUT);

  if (Constantes::velocidades > 120) {
    pid.setAngle(120);
    pid.setKP(0.09);
  }

}

//Determinar si se debe voltear al inicio
void voltear() {
   //Capturar los valores de la cámara (2 veces pq una sola falla jaja)
  actualizarPorterias();
  actualizarPorterias();
  actualizarPorterias();
  Serial.println(porteriaAzul.getX());
  //Verificar si se debe voltear
  if ((atacar == amarillo && porteriaAzul.getX() != -1) || (atacar == azul && porteriaAmarilla.getX() != -1)) {
    gyro.setOffset(180);
    digitalWrite(Constantes::led, HIGH);
    Serial.println("Voltear");
  }

}




//__________________________________________________________-Para el estado de pruebas
void tests() {

// ULTRASONICO
    //Serial.println(distanciaUltrasonico());

// ANALOGO
//  Serial.print(detector());
//  Serial.print("\t\t");
//  Serial.println(filterAnalogo.GetLowPass());


  //ARO-IRRRR________________________________
    // aroIR.actualizarDatos();
    // double angulo = aroIR.getAngulo();
    // Serial.print(angulo);
    // Serial.print("\t\t");
    // Serial.println(aroIR.getStrength());

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
//       actualizarPorterias();
//       Serial.println(porteriaAzul.getX());
//       Serial.println(porteriaAmarilla.getX());

//       if (Serial2.available()) {
//        input = Serial2.readStringUntil('\n');
//       Serial.println(input);

//  }

  //IMU______________________________________
        // gyro.readValues();
        // Serial.println(gyro.getYaw());


  //MOTORESS INDIVIDUAL______________________________________
      // motoresRobot.setAllMotorSpeed(velocidades);
      // motoresRobot.mover1();
      // delay(1000);
      // motoresRobot.mover2();
      // delay(1000);
      // motoresRobot.mover3();


  //MOVIMIENTOLINEALCORREGIDO___________________
  //      int change = correccionesImu();
  //      motoresRobot.movimientoLinealCorregido(0, velocidades, change, gyro.isRight());


}
