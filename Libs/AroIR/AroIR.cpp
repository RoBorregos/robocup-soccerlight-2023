#include "Arduino.h"
#include "AroIr.h"

AroIR::AroIR() {
}

void AroIR::iniciar() {
  Serial3.begin(115200);
  Serial3.setTimeout(100);
}


//Leer datos del aro IR
void AroIR::actualizarDatos() {
  if (Serial3.available()) {
    //Serial3.println("serial1");
    String input = Serial3.readStringUntil('\n');

    if (input[0] == 'a') {
      angulo = input.substring(2, input.length()).toDouble();
      angulo += offset;
      //filterAngulo.AddValue(angulo);
    }
    else if (input[0] == 'r'){
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
