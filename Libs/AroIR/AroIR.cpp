#include "Arduino.h"
#include "AroIr.h"

AroIR::AroIR() {
}

void AroIR::iniciar() {
  Serial2.begin(115200);
  Serial2.setTimeout(100);
}


//Leer datos del aro IR
void AroIR::actualizarDatos() {
  if (Serial2.available()) {
    Serial2.println("serial1");
    String input = Serial2.readStringUntil('\n');

    if (input[0] == 'a') {
      angulo = input.substring(2, input.length()).toDouble();
      angulo += offset;
      //filterAngulo.AddValue(angulo);
    }
    else {
      strength = input.substring(2, input.length()).toDouble();
      //filterStr.AddValue(strength);
    }
  }
}


//Getters
double AroIR::getAngulo() {
  return angulo;
}

//double AroIR::getHighPass() {
//  return filterAngulo.GetHighPass();
//
//}
//
//double AroIR::getLowPass()  {
//  return filterAngulo.GetLowPass();
//}

double AroIR::getStrength() {
  return strength;
}
