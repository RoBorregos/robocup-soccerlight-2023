const uint8_t pinsIR[12] = {14, 7, 16, 5, 17, 10, 2, 9, 6, 8, 15, 3};
int pulseWidth[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const float unitarioX[12] = {0.000, 0.500, 0.866, 1.000, 0.866, 0.500, 0.000, -0.500, -0.866, -1.000, -0.866, -0.500};
const float unitarioY[12] = {1.000, 0.866, 0.500, 0.000, -0.500, -0.866, -1.000, -0.866, -0.500, 0.000, 0.500, 0.866};

double vectorX = 0.0;
double vectorY = 0.0;
double angulo = 0.0;

unsigned long time = 0;


void setup() {
  Serial.begin(115200);
  
  for (short i=0; i<12; i++){ pinMode(pinsIR[i],INPUT); }

}


void loop() {

  // Serial.println(anguloIR());
    Serial.println(String(anguloIR()));

}


double anguloIR(){

  angulo = 0;
  vectorX = 0;
  vectorY = 0;

  for (int i=0; i<12; i++){
    pulseWidth[i] = 0;
  }

  const unsigned long startTime = micros();
  do {
    for (int i=0; i<12; i++){
      if (digitalRead(pinsIR[i]) == false){
        pulseWidth[i] += 2.0;
      }
    }
  } while((micros() - startTime) < 833);

  for (int i=0; i<12; i++){
    vectorX += pulseWidth[i] * unitarioX[i];
    vectorY += pulseWidth[i] * unitarioY[i];
  }

  angulo = atan2(vectorX,vectorY) * 180 / 3.14;
  
  return angulo;

}
