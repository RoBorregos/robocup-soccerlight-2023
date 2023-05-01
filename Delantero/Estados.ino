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
  } else if (str > 80 && abs(angulo) <= 70) {
    dribbler.prender(Constantes::velDribbler);
  } else {
    dribbler.apagar();
  }

  int result = -1000;

    if (str < 40 && abs(angulo) <= 90) {
        result = angulo;
    } else {

    if (abs(angulo <= 100 && abs(angulo) >= 80 && ultrasonico.getDistancia() < 40 && str < 65))
      result = (angulo > 0) ? 45 : -45;
    else if (abs(angulo) <= 30) {
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
    
  } 


    } 

    

  
  

  //Regular velocidad
  double distancia = str*0.1;
  double val = 1.087 + 1/((distancia-11.5));
  int velNuevas = Constantes::velocidades * val;
  velNuevas = max(Constantes::velMin, velNuevas);

  int alto = (atacar == amarillo) ? porteriaAmarilla.getAlto() : porteriaAzul.getAlto();

  if (alto > 60) {
    velNuevas *= 0.8;
  }
  

//Verificar y mover motores
  if (result == -1000) 
    motoresRobot.apagarMotores();
  else 
    motoresRobot.movimientoLinealCorregido(result - gyro.getYaw(), velNuevas, change, gyro.isRight());


//Ultima posición en que se vió
  if (angulo > 0)
    last = 1;
  else
    last = -1;

}



//Estado para buscar la portería 
void gol() {
  actualizarPorterias();    
  int px = (atacar == amarillo) ? porteriaAmarilla.getX() : porteriaAzul.getX();
  int alto = (atacar == amarillo) ? porteriaAmarilla.getAlto() : porteriaAzul.getAlto();
  int largo = (atacar == amarillo) ? porteriaAmarilla.getLargo() : porteriaAzul.getLargo();
  int change = correccionesImu();

  Serial.print("GOL\t\t");

  // Si no ve la porteria o si está muy en las esquinas
  if (px == -1) {
      motoresRobot.movimientoLinealCorregido(170*lastP, Constantes::velocidades, change, gyro.isRight());
  } else if (largo < 65 && alto > 90){

      motoresRobot.movimientoLinealCorregido(150*lastP, Constantes::velocidades, change, gyro.isRight());
  } else {
      atacarGol(px,alto); 
  }


  //Ultima posición en que se vió
  if (px < 160) 
      lastP = -1;
  else 
      lastP = 1;
    
 
}

//Movimientos específicos para atacar a la portería
void atacarGol(int px,int y1) {
  int change = correccionesImu();
  

  //Movimientos para cuando esté muy cerca a la portería --> giros más bruscos
  if (y1 > 76) {
    dribbler.apagar();
  }

  if (y1 == -1) {
    motoresRobot.movimientoLinealCorregido(180-gyro.getYaw(), Constantes::velocidades, change, pid.getR());

  }

  if (y1 > 70 && atacarE < 100) {
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
    if (y1 <= 70)
      atacarE = 0;

  }

 
}


//Devolver el error a corregir
double correccionesImu() {
  gyro.readValues();
  
  double change = pid.calcularError(0, gyro.getYaw(), Constantes::velocidades);
 
  return change;
}

//Posición 0 con giroscopio
void alinear() {
  int change = correccionesImu();
  motoresRobot.giro(change, gyro.isRight());
}


//Devolver ángulo a corregir si se busca un ángulo específico
int correccionesImuTarget(int target) {
  gyro.readValues();

  int error = abs(target - gyro.getYaw());
  bool r = ((target - gyro.getYaw()) > 0) ? true : false;
  pid.setR(r);
  if (error <= 5) return 0;
  error = 0.9 * error;
  error = min(error, 255);
  error = map(error, 0, 255, pid.getMinToMove(), 255);
  return error;
}



//Moverse en dirección contraria a la linea detectada
void salirLinea() {
  actualizarPorterias();
  int alto = (atacar == amarillo) ? porteriaAmarilla.getAlto() : porteriaAzul.getAlto();
  int angle1 = 0;
  int angle2 = color.placasAtras();
  bool del = color.checkPlacaDelantera();
  int t = 350;
  

  if (alto > 125)
    angle1 = 180;

  else if (angle2 != -1 && del) 
    angle1 = (180 + angle2)/2;

  else if (color.checkPlacaDelantera()) {
    angle1 = 180;
    t = 400;
  }
  else 
    angle1 = angle2;
  

  int change = correccionesImu();
  unsigned long ms2 = millis();

  while ((millis() - ms2) < t) {
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

    if (input1[0] == 'a')
      porteriaAmarilla.actualizar(input1);
    else if (input1[0] == 'b')
      porteriaAzul.actualizar(input1);
  }

}


//Detectar si se está en linea o no
bool inLinea() {
  int angle1 = color.checkForLineaPlaca();
  actualizarPorterias();
  int alto = (atacar == amarillo) ? porteriaAmarilla.getAlto() : porteriaAzul.getAlto();

  Serial.print("salir: ");
  Serial.println(alto);

    if (angle1 != -1 || (alto > 112)) {
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
  
  pid.setKP(Constantes::kP);
  pid.setMinToMove(60);
  gyro.iniciar();
  aroIR.iniciar(&current_time);
  color.iniciar();
  aroIR.actualizarDatos();
  color.calibrar();
  dribbler.iniciar();
  motoresRobot.iniciar();


  ultrasonico.iniciar();



  pinMode(Constantes::analogo, INPUT);
  pinMode(Constantes::led, OUTPUT);

 

}

//Determinar si se debe voltear al inicio
void voltear() {
   //Capturar los valores de la cámara (2 veces pq una sola falla jaja)
  actualizarPorterias();
  actualizarPorterias();
  actualizarPorterias();
  Serial.println(porteriaAzul.getX());
  //Verificar si se debe voltear
  if ((atacar == amarillo && porteriaAzul.getLargo() > 30) || (atacar == azul && porteriaAmarilla.getLargo() > 30)) {
    gyro.setOffset(180);
    digitalWrite(Constantes::led, HIGH);
    Serial.println("Voltear");
  }

}




//___________________________________________________________Para el estado de pruebas
void tests() {
//Placas
//Serial.println(color.placasAtras());

//DRIBBLER
// dribbler.prender(900);
// delay(2000);
// dribbler.apagar();
// delay(2000);


// ULTRASONICO
// Serial.println(ultrasonico.getDistancia());

// ANALOGO
//  Serial.print(detector());
//  Serial.print("\t\t");
//  Serial.println(filterAnalogo.GetLowPass());


  //ARO-IRRRR________________________________
    //  aroIR.actualizarDatos();
    //  double angulo = aroIR.getAnguloSinFiltro();
    // Serial.print(angulo);
    // Serial.print("\t\t");
    // Serial.println(aroIR.getAngulo());

    //     if (Serial2.available()) {
    //       input = Serial2.readStringUntil('\n');
    //      Serial.println(input);
  
    //  }



  //CAMARA____________________________________
      // actualizarPorterias();
      // Serial.print(porteriaAmarilla.getAlto());
      // Serial.print("/t/t");
      // Serial.println(porteriaAmarilla.getLargo());

//       if (Serial2.available()) {
//       String input = Serial2.readStringUntil('\n');
//       Serial.println(input);

//  }

  //GYRO______________________________________
      //  gyro.readValues();
      //  Serial.println(gyro.getYaw());
//        gyro.displayCalStatus();


  //MOTORESS INDIVIDUAL______________________________________
      // motoresRobot.setAllMotorSpeed(Constantes::velocidades);
      // motoresRobot.mover1();
      // delay(1000);
      // motoresRobot.mover2();
      // delay(1000);
      // motoresRobot.mover3();


  //MOVIMIENTOLINEALCORREGIDO___________________
       double change = correccionesImu();
        Serial.print("CHange: ");
       Serial.println(change);
       motoresRobot.movimientoLinealCorregido(0-gyro.getYaw(), Constantes::velocidades, change, gyro.isRight());


}
