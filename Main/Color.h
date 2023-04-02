class Color {

  private:
    const int pinLED = 52;
    const int pinBoton = 7;

    
    const int sig[3] = {A0, A1, A15}; // pines signal: DERECHA, PRINCIPAL, IZQUIERDA
    const int sC[3] = {8, 13, 53}; // pines de 'control' ABC
    const int sB[3] = {9, 11, 50}; //robot bno
    const int sA[3] = {10, 12, 51};

     
//    const int sig[3] = {A0, A1, A15}; // pines signal: DERECHA, PRINCIPAL, IZQUIERDA
//    const int sC[3] = {8, 12, 53}; // pines de 'control' ABC
//    const int sB[3] = {9, 11, 49}; //robot imu
//    const int sA[3] = {10, 13, 51};

      
    int foto[3][8] = {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}};
    int fotoMinV[3][8] = {{2, 6, 7, 8, 3, 6, 5, 8}, {0, 1, 1, 3, 4, 0, 3, 8}, {1, 7, 10, 6, 2, 6, 1, 10}};
    int fotoMaxV[3][8] = {{4, 8, 9, 10, 5, 8, 8, 10}, {1, 3, 4, 5, 5, 0, 5, 11}, {6, 13, 16, 12, 5, 10, 8, 16}};
    int fotoMinB[3][8] = {{27, 50, 43, 35, 32, 41, 43, 47}, {19, 44, 35, 58, 56, 100, 52, 58}, {25, 49, 52, 49, 31, 53, 45, 69}};
    int fotoMaxB[3][8] = {{30, 53, 46, 38, 35, 44, 46, 50}, {22, 47, 38, 62, 58, 100, 55, 60}, {33, 56, 63, 56, 36, 60, 52, 79}};
    // int fotoMinV[3][8] = {{100,100,100,100,100,100,100,100},{100,100,100,100,100,100,100,100},{100,100,100,100,100,100,100,100}};
    // int fotoMaxV[3][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
    // int fotoMinB[3][8] = {{100,100,100,100,100,100,100,100},{100,100,100,100,100,100,100,100},{100,100,100,100,100,100,100,100}};
    // int fotoMaxB[3][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
    double angulo[3][8] = {{271.2, 279.6, 287.8, 296.3, 307.6, 315.7, 323.8, 332.3}, {212.7, 205.8, 195.2, 180, 180, 167, 156, 149}, {44.3, 27.7, 36.2, 54.2, 63.7, 88.8, 72.2, 80.4}};

  int lectura(int ft, int placa) {  // 1: derecha (1,3,4)
      switch (ft) {
        case 0: binario(0, placa); return analogRead(sig[placa]); break;
        case 1: binario(1, placa); return analogRead(sig[placa]); break;
        case 2: binario(2, placa); return analogRead(sig[placa]); break;
        case 3: binario(3, placa); return analogRead(sig[placa]); break;
        case 4: binario(4, placa); return analogRead(sig[placa]); break;
        case 5: binario(5, placa); return analogRead(sig[placa]); break;
        case 6: binario(6, placa); return analogRead(sig[placa]); break;
        case 7: binario(7, placa); return analogRead(sig[placa]); break;
      }
    }

    void binario(int pin, int lado) {
      switch (pin) {
        case 0:
          digitalWrite(sC[lado], 0);
          digitalWrite(sB[lado], 0);
          digitalWrite(sA[lado], 0); break;
        case 1:
          digitalWrite(sC[lado], 0);
          digitalWrite(sB[lado], 0);
          digitalWrite(sA[lado], 1); break;
        case 2:
          digitalWrite(sC[lado], 0);
          digitalWrite(sB[lado], 1);
          digitalWrite(sA[lado], 0); break;
        case 3:
          digitalWrite(sC[lado], 0);
          digitalWrite(sB[lado], 1);
          digitalWrite(sA[lado], 1); break;
        case 4:
          digitalWrite(sC[lado], 1);
          digitalWrite(sB[lado], 0);
          digitalWrite(sA[lado], 0); break;
        case 5:
          digitalWrite(sC[lado], 1);
          digitalWrite(sB[lado], 0);
          digitalWrite(sA[lado], 1); break;
        case 6:
          digitalWrite(sC[lado], 1);
          digitalWrite(sB[lado], 1);
          digitalWrite(sA[lado], 0); break;
        case 7:
          digitalWrite(sC[lado], 1);
          digitalWrite(sB[lado], 1);
          digitalWrite(sA[lado], 1); break;
      }
    }



    // -------------------------------------------------------------------------------------------------------------




  public:

    Color() {}

    void iniciar() {
      for (int i = 0; i < 2.5; i++) {
        pinMode(sC[i], OUTPUT);
        pinMode(sB[i], OUTPUT);
        pinMode(sA[i], OUTPUT);
      }
      pinMode(pinBoton, INPUT);
      pinMode(pinLED, OUTPUT);
    }



    void calibrar() {

      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 8; j++) {
          int suma = 0;
          for (int k = 0; k < 100; k++) {
            suma += lectura(j, i);
          }
          foto[i][j] = suma / 100;
        }
      }
      foto[1][5] = 100;
      foto[1][1] = 7999;
      Serial.println("Calibración lista");

    }



    double checkForLinea() {
      double degree = 0;
      double count = 0;
      bool check = false;
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 8; j++) {
          int lec = lectura(j, i);
          if (lec >= 2.5 * foto[i][j]) {
            if (i == 2 && j == 0 && check == true){
              return 0;
            }
            else if (i == 0 && j == 7){
              check = true;
            }
            degree += angulo[i][j];
            count++;
          }
        }
      }
      if (count > 0) {
        return degree / count;
      }
      else {
        return -1;
      }
    }


    bool checkForLineaBool() {
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3; j++) {
          if (lectura(i, j) >= fotoMinB[j][i] && lectura(i, j) <= fotoMaxB[j][i]) {
            return true;
          }
        }
      }
      return false;
    }

    bool checkForLineaBool2() {
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3; j++) {
          int lec = lectura(i,j);
          if (lec >= 2.5*foto[i][j]) {
            return true;
          }
        }
      }
      return false;
    }

};
