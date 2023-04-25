//-------------------------------------ARO IR
//Buscar y moverse según la posición de la pelota
void buscar() {
  int change = correccionesImu();
  aroIR.actualizarDatos();
  double angulo = aroIR.getAngulo();
  double str = aroIR.getStrength();


  if (str == 0) {
    motoresRobot.apagarMotores();
    return;
  } else if (str > 60 && abs(angulo) <= 90) {
    // esc.writeMicroseconds(800);
    dribbler.prender();
  } else {
    //esc.writeMicroseconds(0);
    dribbler.apagar();
  }

  int result = -1000;
  

    if (abs(angulo) <= 25) {
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
    if (abs(result) <= 135)
      result = result * 0.7;
 
  } else if (str < 50) {
    if (abs(result) <= 135)
      result = result * 0.8;
    
  } //else if (str < 60 && abs(angulo) < 110 && abs(angulo) > 70 && ultrasonico.getDistancia() < 30)
   //result = 0;

  //double distancia = map(str, 10,100, 0, 10);
  double distancia = str*0.1;
 // double val = 1 - pow(EULER,(0.9*(distancia-10)));
  double val = 1.087 + 1/((distancia-11.5));
  int velNuevas = Constantes::velocidades * val;

  velNuevas = max(Constantes::velMin, velNuevas);
  // if (velNuevas < 110){
  //   pid.setKP(0.1);
  //   //pid.setAngle(90);
  // } else   {
  //   pid.setKP(0.09);
  //   pid.setAngle(120);
  // }

  //Serial.print("factor: ");
  //Serial.println(distancia);

  // Serial.print("velocidad: ");
  //Serial.println(velNuevas);

  if (result == -1000) 
    motoresRobot.apagarMotores();
  else 
    motoresRobot.movimientoLinealCorregido(result - gyro.getYaw(), velNuevas, change, gyro.isRight());


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
  if (y1 > 90) {
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
  if (ultrasonico.getDistancia() < 50){
    aroIR.actualizarDatos();
    double angulo = aroIR.getAngulo();
    angle1 = (angulo > 0) ? 45 : -45;
  }

  int change = correccionesImu();
  unsigned long ms2 = millis();

  while ((millis() - ms2) < 500) {
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
            if(filterAnalogo.GetLowPass() > 700) {
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

    if (ultrasonico.getDistancia() < 15) 
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
  
  pid.setKP(0.2);
  pid.setMinToMove(40);
  gyro.iniciar();
  aroIR.iniciar();
  color.iniciar();
  aroIR.actualizarDatos();
  color.calibrar();
  dribbler.iniciar();
  motoresRobot.iniciar();


  ultrasonico.iniciar();



  pinMode(Constantes::analogo, INPUT);
  pinMode(Constantes::led, OUTPUT);

    // esc.attach(7);
    // delay(2000); 
    // esc.writeMicroseconds(780); // set the motor speed to minimum
    // delay(1500); // wait for 3 seconds

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
//DRIBBLER
//dribbler.prender();
//  esc.writeMicroseconds(780);


// ULTRASONICO
// Serial.println(ultrasonico.getDistancia());

// ANALOGO
//  Serial.print(detector());
//  Serial.print("\t\t");
//  Serial.println(filterAnalogo.GetLowPass());


  //ARO-IRRRR________________________________
    aroIR.actualizarDatos();
    double angulo = aroIR.getAngulo();
    Serial.print(angulo);
    Serial.print("\t\t");
    Serial.println(aroIR.getStrength());

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
      // motoresRobot.setAllMotorSpeed(Constantes::velocidades);
      // motoresRobot.mover1();
      // delay(1000);
      // motoresRobot.mover2();
      // delay(1000);
      // motoresRobot.mover3();


  //MOVIMIENTOLINEALCORREGIDO___________________
  //      int change = correccionesImu();
  //      motoresRobot.movimientoLinealCorregido(0, velocidades, change, gyro.isRight());


}
