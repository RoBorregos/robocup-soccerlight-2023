//Motores
int motor1Speed = 8;
int motor1P1 = 24;
int motor1P2 = 25;

int motor2Speed = 9;
int motor2P1 = 26;
int motor2P2 = 27;

int motor3Speed = 10;
int motor3P1 = 22;
int motor3P2 = 23;

int degree = 0;
int comple = 0;   // ángulo complementario
int tri = 0;   // ángulo de triángulo
int spd = 0;    // velocidad

//Variables
double MPI = 3.141516;
double DIAMETRO = 0.11; //m

int sig[3] = {A1,A0,A2}; // pines signal: DERECHA, IZQUIERDA, PRINCIPAL

int sC[3] = {1,7,1};  // pines de 'control' ABC 
int sB[3] = {1,8,1};
int sA[3] = {1,9,1};

int fotoD[8] = {0,0,0,0,0,0,0,0};     // lecturas de los fototransistores
int fotoI[8] = {0,0,0,0,0,0,0,0}; 
int fotoP[8] = {0,0,0,0,0,0,0,0}; 

#define blanco 0      // detección del blanco
#define verde 0
#define error 10     // rango de detección del blanco



void setup(){
  Serial.begin(9600);
  
  for (int i=0; i<3; i++){  
    pinMode(sC[i],OUTPUT);
    pinMode(sB[i],OUTPUT);
    pinMode(sA[i],OUTPUT);
    pinMode(sig[i],INPUT);
  }

}



void loop(){

  degree = 145;
  comple = complementario(degree);
  tri = triangulo(degree);
  spd = 5; 

  movimientoLineal(degree,spd);

  for (int i=0; i<8; i++){    // checar placa derecha
  /*
    Serial.print("Lectura del ft DERECHA");
    Serial.print(i);
    Serial.print(" : ");
    fotoD[i] = lectura(i,0);
    Serial.println(lectura(i,0));
    delay(1);
    
    if (fotoD[i] <= blanco+error and fotoD[i] >= blanco-error){ // si detecta blanco 
     // movimientoLineal(tri,spd);
      movimientoLineal(comple,spd);
    }
    */
  }

  for (int i=0; i<8; i++){    // checar placa izquierda
  
    Serial.print("Lectura del ft IZQUIERDA");
    Serial.print(i);
    Serial.print(" : ");
    fotoI[i] = lectura(i,0);
    Serial.println(lectura(i,0));
    delay(1);
    
    if (fotoI[i] <= blanco+10 and fotoI[i] >= blanco-10){ // si detecta blanco 
     // movimientoLineal(tri,spd);
      movimientoLineal(comple,spd);
    }
    
  }

  for (int i=0; i<8; i++){    // checar placa principal
  /*
    Serial.print("Lectura del ft PRINCIPAL");
    Serial.print(i);
    Serial.print(" : ");
    fotoP[i] = lectura(i,2);
    Serial.println(lectura(i,2));
    delay(1);
    
    if (fotoP[i] <= blanco+10 and fotoP[i] >= blanco-10){ // si detecta blanco 
     // movimientoLineal(tri,spd);
      movimientoLineal(comple,spd);
    }
    */
  }

  
}




int lectura(int ft, int placa){
  switch (ft){
    case 0: binario(0,placa); delay(10); return analogRead(sig[placa]); break;
    case 1: binario(1,placa); delay(10); return analogRead(sig[placa]); break;
    case 2: binario(2,placa); delay(10); return analogRead(sig[placa]); break;
    case 3: binario(3,placa); delay(10); return analogRead(sig[placa]); break;
    case 4: binario(4,placa); delay(10); return analogRead(sig[placa]); break;
    case 5: binario(5,placa); delay(10); return analogRead(sig[placa]); break;
    case 6: binario(6,placa); delay(10); return analogRead(sig[placa]); break;
    case 7: binario(7,placa); delay(10); return analogRead(sig[placa]); break;
  }
}


void binario(int pin, int lado){
  switch (pin){
    case 0: 
      digitalWrite(sC[lado],0); 
      digitalWrite(sB[lado],0); 
      digitalWrite(sA[lado],0); break;
    case 1:
      digitalWrite(sC[lado],0); 
      digitalWrite(sB[lado],0); 
      digitalWrite(sA[lado],1); break;
    case 2:
      digitalWrite(sC[lado],0); 
      digitalWrite(sB[lado],1); 
      digitalWrite(sA[lado],0); break;
    case 3:
      digitalWrite(sC[lado],0); 
      digitalWrite(sB[lado],1); 
      digitalWrite(sA[lado],1); break;
    case 4: 
      digitalWrite(sC[lado],1); 
      digitalWrite(sB[lado],0); 
      digitalWrite(sA[lado],0); break;
    case 5: 
      digitalWrite(sC[lado],1); 
      digitalWrite(sB[lado],0); 
      digitalWrite(sA[lado],1); break;
    case 6: 
      digitalWrite(sC[lado],1); 
      digitalWrite(sB[lado],1); 
      digitalWrite(sA[lado],0); break;
    case 7: 
      digitalWrite(sC[lado],1); 
      digitalWrite(sB[lado],1); 
      digitalWrite(sA[lado],1); break;
  }
}


int complementario(int degree){
  if (degree < 180){
    return degree + 180;
  }
  else{
    return degree - 180;
  }
}

int triangulo(int degree){
  if (degree < 90){
    return degree + 270;
  }
  else{
    return degree - 90;
  }
}




void movimientoLineal(int degree, int velocidad) {

    //En base a los grados se definen las velocidades de cada motor
    float m1 = cos(((150-degree) * PI / 180));
    float m2 = cos(((30-degree) * PI / 180));;
    float m3 = cos(((270-degree) * PI / 180));
    int speedA = abs(int(m1*velocidad));
    int speedB = abs(int(m2*velocidad));
    int speedC = abs(int(m3*velocidad));

    //Definir la velocidad de cada motor
    analogWrite(motor1Speed, speedA);
    analogWrite(motor2Speed, speedB);
    analogWrite(motor3Speed, speedC);

    //Mover motores según la velocidad (positiva o negativa)
    if (m1 >= 0){
        motor1Adelante();
    } else {
        motor1Atras();
    }
    
    if (m2 >= 0){
        motor2Adelante();
    } else {
        motor2Atras();
    } 
    
    if (m3 >= 0){
        motor3Adelante();
    } else {
        motor3Atras();
    }
}


//----------------------------Movimientos básicos

//Definir la velocidad de todos los motores
void setAllMotorSpeed(int allSpeed){
  analogWrite(motor1Speed, allSpeed);
  analogWrite(motor2Speed, allSpeed);
  analogWrite(motor3Speed, allSpeed);
}

void adelante(){
  resetMotors();
  delay(800);
  
  motor1Atras();
  motor2Adelante();
}

void atras(){
  resetMotors();
  delay(800);
  
  motor1Adelante();
  motor2Atras();
}

//Rotación horaria
void giroH(){
  resetMotors();
  delay(800);

  motor1Adelante();
  motor2Adelante();
  motor3Adelante();

}

//Rotación antihoraria
void giroAH(){
  resetMotors();
  delay(800);

  motor1Atras();
  motor2Atras();
  motor3Atras();

}

//Noreste
void NE(){
  resetMotors();
  delay(800);

  motor2Adelante();
  motor3Atras();
}

//Noroeste
void NO(){
  resetMotors();
  delay(800);

  motor3Adelante();
  motor1Atras();
}

//Sureste
//void SurE(){
//  resetMotors();
//  delay(800);
//
//  motor3Atras();
//  motor1Adelante();
//}

//Suroeste
void SO(){
  resetMotors();
  delay(800);

  motor2Atras();
  motor3Adelante();
}

//-----------------------------------------CONTROL
//Apagar motores
void resetMotors(){
  digitalWrite(motor1P1, LOW);
  digitalWrite(motor1P2, LOW);

  digitalWrite(motor2P1, LOW);
  digitalWrite(motor2P2, LOW);

  digitalWrite(motor3P1, LOW);
  digitalWrite(motor3P2, LOW);
}

void motor1Adelante(){
  digitalWrite(motor1P1, HIGH);
  digitalWrite(motor1P2, LOW);
}

void motor2Adelante(){
  digitalWrite(motor2P1, HIGH);
  digitalWrite(motor2P2, LOW);
}

void motor3Adelante(){
  digitalWrite(motor3P1, HIGH);
  digitalWrite(motor3P2, LOW);
}

void motor1Atras(){
  digitalWrite(motor1P1, LOW);
  digitalWrite(motor1P2, HIGH);
}

void motor2Atras(){
  digitalWrite(motor2P1, LOW);
  digitalWrite(motor2P2, HIGH);
}

void motor3Atras(){
  digitalWrite(motor3P1, LOW);
  digitalWrite(motor3P2, HIGH);
}
