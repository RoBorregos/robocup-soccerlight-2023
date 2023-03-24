class Posicion {

  private:
    const uint8_t pinsUltrasonicosTrigX[2] = {0, 0};
    const uint8_t pinsUltrasonicosEchoX[2] = {0, 0};
    const uint8_t pinsUltrasonicosTrigY[2] = {0, 0};
    const uint8_t pinsUltrasonicosEchoY[2] = {0, 0};

  public:
    Posicion(){}

    void iniciar(){
      for (byte i=0; i<2; i++){
        pinMode(pinsUltrasonicosTrigX[i],OUTPUT);
        pinMode(pinsUltrasonicosEchoX[i],INPUT);
        pinMode(pinsUltrasonicosTrigY[i],OUTPUT);
        pinMode(pinsUltrasonicosEchoY[i],INPUT);
      }
    }

    float getPosicionX(){
        digitalWrite(pinsUltrasonicosTrigX[0],LOW);
        delayMicroseconds(2);
        digitalWrite(pinsUltrasonicosTrigX[0],HIGH);
        delayMicroseconds(10);
        digitalWrite(pinsUltrasonicosTrigX[0],LOW);
        int duracion = pulseIn(pinsUltrasonicosEchoX[0],HIGH);
        float distancia1 = (duracion/2)/29.1;

        digitalWrite(pinsUltrasonicosTrigX[1],LOW);
        delayMicroseconds(2);
        digitalWrite(pinsUltrasonicosTrigX[1],HIGH);
        delayMicroseconds(10);
        digitalWrite(pinsUltrasonicosTrigX[1],LOW);
        duracion = pulseIn(pinsUltrasonicosEchoX[1],HIGH);
        float distancia2 = (duracion/2)/29.1;

        float posicionX = (distancia1 + (distancia2-243))/2;
        return posicionX;
    }

    float getPosicionY(){
        digitalWrite(pinsUltrasonicosTrigY[0],LOW);
        delayMicroseconds(2);
        digitalWrite(pinsUltrasonicosTrigY[0],HIGH);
        delayMicroseconds(10);
        digitalWrite(pinsUltrasonicosTrigY[0],LOW);
        int duracion = pulseIn(pinsUltrasonicosEchoY[0],HIGH);
        float distancia1 = (duracion/2)/29.1;

        digitalWrite(pinsUltrasonicosTrigY[1],LOW);
        delayMicroseconds(2);
        digitalWrite(pinsUltrasonicosTrigY[1],HIGH);
        delayMicroseconds(10);
        digitalWrite(pinsUltrasonicosTrigY[1],LOW);
        duracion = pulseIn(pinsUltrasonicosEchoY[1],HIGH);
        float distancia2 = (duracion/2)/29.1;

        float posicionY = (distancia1 + (distancia2))/2;
        return posicionY;
    }

};