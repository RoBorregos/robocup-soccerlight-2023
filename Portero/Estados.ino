//Setup de objetos
void iniciarObjetos(){ 
  pinMode(Constantes::ledPin, OUTPUT);
  motoresRobot.iniciar();
  gyro.iniciar();
  aroIR.iniciar2(&current_time);
  aroIR.actualizarDatos2();
  pid.setKP(Constantes::kP);
  pid.setMinToMove(40);
  color.iniciar();
  ultrasonico.iniciar();
  ultrasonicoD.iniciar();
  ultrasonicoI.iniciar();
  aroIR.setOffset(Constantes::offsetAro);
  color.calibrar();

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

//Verificar que no hayan robots a los lados al defender
bool checkUltrasonicos() {
  double distanciaD = ultrasonicoD.getDistancia();
  double distanciaI = ultrasonicoI.getDistancia();
  gyro.readValues();

  if (distanciaD + distanciaI > Constantes::largoCancha && distanciaD + distanciaI < Constantes::largoCancha + 20 && abs(gyro.getYaw()) < 30) //132 --> medida de la cancha
    return true;
  
  return false;
}


//Ir a la portería
void buscarPorteria(){
  actualizarPorterias();
  int x1 = (atacar == amarillo) ? porteriaAzul.getX() : porteriaAmarilla.getX();
  int largo = (atacar == amarillo) ? porteriaAzul.getLargo() : porteriaAmarilla.getLargo();
  int alto = (atacar == amarillo) ? porteriaAzul.getAlto() : porteriaAmarilla.getAlto();
  int change = correccionesImu();
    
    int error = x1 - 160;

    double kP = 0.375;
    int vel = Constantes::velocidades;
    if (ultrasonico.getDistancia() < 60)
        vel *= 0.85;

    int ang = 180;

    // Si no ve la portería
    if (x1 == -1){
      ang = (ultrasonicoD.getDistancia() < ultrasonicoI.getDistancia()) ? -55 : 55;

    //Si está muy lejos de la portería
    } else if (alto < 85) {
        if (abs(error) < 0)
           180;
        else {
          ang = (error > 0)? 180 - (error*kP) : -180 - (error*kP);
          ang *= -1;
        }

    //Si está cerca pero muy a los lados
    } else { 
      if (largo <  190)  // de acuerdo a las detecciones de que lado esta ir hacia la direcci[on y checar la prioridad conel otro if para saber si te arruina cuando estas lejos]
      ang = (x1 > 160) ? -90 : 90;

      if (alto > 50 && ((ultrasonicoD.getDistancia() < Constantes::ultrasonicoSalir || ultrasonicoI.getDistancia() < Constantes::ultrasonicoSalir))) {
      //ang = (x1 > 160) ? -45 : 45;
      ang = (ultrasonicoD.getDistancia() < ultrasonicoI.getDistancia()) ? -90 : 90;
      vel = Constantes::velocidades;
      }
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


//Detección de la pelota para saber si se tiene la posesión
// int detector() {
//   int pulseWidth = 0;
//   int deltaPulseWidth = 5;

//     const unsigned long startTime_us = micros();
//     do {       
//       //Serial.println(analogRead(analogo));
//             filterAnalogo.AddValue(analogRead(Constantes::analogo));
//             if(filterAnalogo.GetLowPass() > 600) {
//                 pulseWidth += deltaPulseWidth;
//             }
        
//     } while((micros() - startTime_us) < 833);

//   return pulseWidth;

// }

//Atacer si tiene posesión
void sacar() {
  int change = correccionesImu();
  int y1 = (atacar == amarillo) ? porteriaAzul.getY() : porteriaAmarilla.getY();

  if (y1 == -1) {
    motoresRobot.movimientoLinealCorregido(0, Constantes::velocidades, change, gyro.isRight());
    return;
  }
  while (y1 > 60){
      int change = correccionesImu();
      motoresRobot.movimientoLinealCorregido(0-gyro.getYaw(), Constantes::velocidades, change, gyro.isRight());
  }
  

};

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


  if (aroIR.getStrength() == 0) {
    motoresRobot.apagarMotores();
    return;
  }

  bool derecha = true;
 
      //Serial.println(getComponente());
    double componente = getComponente();
    if (abs(angulo) < 15 || abs(componente) < 2) 
      alinear();
    else 
        (aroIR.getAnguloSinFiltro() > 0)? seguidorD(componente) : seguidorI(componente);

    
}


//Salir de la linea
void salirAdelante(int angle) {
  int change = correccionesImu();
  unsigned long ms2 = millis();
  aroIR.actualizarDatos2();
    double angulo = aroIR.getAngulo();

  while ((millis() - ms2) < 300) {
   
    gyro.readValues();  
    angle = (angulo > 0) ? min(35, angulo) : max(-35, angulo);
    motoresRobot.movimientoLinealCorregido(angle-gyro.getYaw(), Constantes::velMin, change, gyro.isRight());
  }

}





void alinear() {
  int change = correccionesImu();
  motoresRobot.giro(change, gyro.isRight());
}

//Seguidor de linea (a la derecha)
void seguidorD(double componente) {
  pid.setKP(0.04);

  double k = 0.15; //0.2   a  110
  Serial.println("D");
  int vel = getErrorVelocidades(componente);
  int change = correccionesImu();
  int lectura = color.getValor(2, 2); //Foto transistor 2 placa 2 (derecha)
  double error = 90 + (Constantes::fotoDerecha-lectura)*k;
  
  motoresRobot.movimientoLinealCorregido(error-gyro.getYaw(), vel, change, gyro.isRight());

}

//Seguidor de linea (a la izquierda)
void seguidorI(double componente) {
  pid.setKP(0.04);
  double k = 0.15;
  Serial.println("I");
  int change = correccionesImu();
  int lectura = color.getValor(0, 1); //Foto transistor 1 placa 0 (izquierda)
  double error = -90 - (Constantes::fotoIzquierda-lectura)*k;
  int vel = getErrorVelocidades(componente);
  motoresRobot.movimientoLinealCorregido(error - gyro.getYaw(), vel, change, gyro.isRight());
}

//Obtener la velocidad para llegar a la pelota
double getErrorVelocidades(double componente) {
  aroIR.actualizarDatos2();
  double angulo = aroIR.getAngulo();
  double k = 0.2;
  double str = aroIR.getStrength();
  str = min(90-str, 50);

  if (abs(angulo) > 90) angulo = 90;
  angulo = map(abs(angulo), 0, 90, 0, 50);

  double errorFinal = (componente + angulo);
  errorFinal = k*errorFinal;
  int vel = min(errorFinal + Constantes::velMin,Constantes::velMax);
  
  //error = Constantes::velocidades - abs(error);
  // Serial.print(str);
  // Serial.print("\t\t");
  // Serial.print(angulo);
  // Serial.print("\t\t");
  // Serial.print(strX);
  // Serial.print("\t\t");
  // Serial.print(errorFinal);
  // Serial.print("\t\t");
  // Serial.println(vel);
   
  
  return vel;
}

double getComponente() {
  aroIR.actualizarDatos2();
  double angulo = aroIR.getAngulo();
  double k = 0.2;
  double str = aroIR.getStrength();
  str = 100 -str;
  double strX = (angulo > 0) ? (str)*sin((angulo) * PI / 180) : (str)*sin((angulo) * PI / 180);

  if (abs(angulo) > 90) angulo = 90;
  
  return strX;
}




//__________________________________________________________-Para el estado de pruebas
void tests() {
 // alinear();
//getComponente();
  // pid.setKP(0.03);
  // int change = correccionesImu();
  // motoresRobot.movimientoLinealCorregido(-90-gyro.getYaw(), Constantes::velocidades, change, gyro.isRight());

//ULTRASONICOS
// Serial.print(checkUltrasonicos());
// Serial.print("\t\t");
// Serial.print(ultrasonicoD.getDistancia());
// Serial.print("\t\t");
// Serial.print(ultrasonicoI.getDistancia());
// Serial.print("\t\t");
// Serial.println(ultrasonicoD.getDistancia()+ultrasonicoI.getDistancia());


//IMU______________________________________
      //   gyro.readValues();
      //  Serial.println(gyro.getYaw());

//FOTO TRANSISTORES____________________________________
// Serial.print("D: ");
// Serial.print(color.getValor(2, 2));
// Serial.print("\t\t");
// Serial.print("I: ");
// Serial.println(color.getValor(0, 1));


//ANALOGO
//  Serial.print(analogRead(analogo));
//  Serial.print("\t\t");
//  Serial.println(hasPosesion());


//ARO-IR ____________________________________
       aroIR.actualizarDatos2();
       double angulo = aroIR.getAngulo();
       double str = aroIR.getStrength();
       Serial.print(angulo);
       Serial.print("\t\t");
       Serial.print(aroIR.getAnguloSinFiltro());
       Serial.print("\t\t");
       Serial.println(str);
//SERIAL ____________________________________
//  if (Serial1.available()) {
//        input = Serial1.readStringUntil('\n');
//        Serial.println(input);  
//    }


//CAMARA____________________________________
    // actualizarPorterias();
    // Serial.println(porteriaAmarilla.getAlto());
//SERIAL
    // if (Serial2.available()) {
    //      //Serial.println("serial1");
    //      String input = Serial2.readStringUntil('\n');
    //      Serial.println(input);
    //  }



//MOTORESS INDIVIDUAL______________________________________
    //   motoresRobot.setAllMotorSpeed(Constantes::velocidades);
    //   motoresRobot.mover1();
    //   delay(1000);
    //   motoresRobot.mover2();
    //   delay(1000);
    //   motoresRobot.mover3();


  //MOVIMIENTOLINEALCORREGIDO___________________
  //      int change = correccionesImu();
  //      motoresRobot.movimientoLinealCorregido(0-gyro.getYaw(), Constantes::velocidades, change, gyro.isRight());

}
