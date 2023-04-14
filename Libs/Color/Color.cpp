#include "Arduino.h"
#include "Color.h"

int Color::lectura(int ft, int placa) {  // 1: derecha (1,3,4)
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

void Color::binario(int pin, int lado) {
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

Color::Color() {};

void Color::iniciar() {
  for (int i = 0; i < 3; i++) {
    pinMode(sC[i], OUTPUT);
    pinMode(sB[i], OUTPUT);
    pinMode(sA[i], OUTPUT);
  }
  pinMode(pinBoton, INPUT);
  pinMode(pinLED, OUTPUT);
}



void Color::calibrar() {

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      int suma = 0;
      for (int k = 0; k < 100; k++) {
        suma += lectura(j, i);
      }
      foto[i][j] = suma / 100;
    }
  }
  //      foto[1][5] = 100;
  //      foto[1][1] = 7999;
  Serial.println("Calibración lista");

}



double Color::checkForLinea() {
  double degree = 0;
  double count = 0;
  bool check = false;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      int lec = lectura(j, i);
      if (lec - foto[i][j] >= 35) {
        if (i == 2 && j == 0 && check == true) {
          return 0;
        }
        else if (i == 0 && j == 7) {
          check = true;
        }
        degree += angulo[i][j];
        count++;
      }
    }
  }
  if (count > 1) {
    return degree / count;
  }
  else {
    return -1;
  }
}


bool Color::checkForLineaBool() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      if (lectura(i, j) >= fotoMinB[j][i] && lectura(i, j) <= fotoMaxB[j][i]) {
        return true;
      }
    }
  }
  return false;
}

double checkForLineaPlaca(){
      double degree = 0;
      int count = 0;
      for (int i=0; i<4; i++){
        for (int j=0; j<3; j++){
          if (lectura(i,j) >= 2.3*foto[j][i]){
            degree += anguloF[j];
            count++;
          }
        }
      }
      if (count > 0){
      double promedio = degree / count;
      return promedio;
      }
      else { return -1; }
      
    }
