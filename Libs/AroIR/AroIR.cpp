#include "Arduino.h"
#include "AroIr.h"
#include  "SingleEMAFilterLib.h"  

SingleEMAFilter<double> filterAngulo(0.6);
SingleEMAFilter<double> filterStr(0.6);

AroIR::AroIR() {
}

void AroIR::iniciar(unsigned long* current_time) {
  this->current_time = current_time;
  Serial3.begin(115200);
  Serial3.setTimeout(100);
}

void AroIR::iniciar2(unsigned long* current_time) {
  Serial1.begin(115200);
  Serial1.setTimeout(100);
}


//Leer datos del aro IR
void AroIR::actualizarDatos() {
  // if( *current_time - last_time < 80) 
  //   return;
  if (Serial3.available()) {
    //Serial3.println("serial1");
    String input = Serial3.readStringUntil('\n');

    if (input[0] == 'a') {
      angulo = input.substring(2, input.length()).toDouble();
      angulo += offset;
      filterAngulo.AddValue(angulo);
    }
    else if (input[0] == 'r'){
      strength = input.substring(2, input.length()).toDouble();
      filterStr.AddValue(strength);
    }
  }
  last_time = *current_time;
}

void AroIR::actualizarDatos2() {
  // if( *current_time - last_time < 90) 
  //   return;

  if (Serial1.available()) {
    //Serial3.println("serial1");
    String input = Serial1.readStringUntil('\n');

  
    if (input[0] == 'a') {
      angulo = cutString1(input);
      angulo -= offset;
      filterAngulo.AddValue(angulo);
    }
    else if (input[0] == 'r'){
      strength = cutString1(input);
      filterStr.AddValue(strength);
    }
  }

  last_time = *current_time;
}

void AroIR::setOffset(double offset) {
  this->offset = offset;
}

double AroIR::cutString1(String str) {
  String data = "";

  for (int i = 2; i < str.length(); i++) {
    if (str[i] == 'a' || str[i] == 'r')  
      return str.substring(2, i).toDouble();  
  }

  return str.substring(2, str.length()).toDouble();
}

//Getters
double AroIR::getAngulo() {
  //return filterAngulo.GetLowPass();
  return angulo;
}
double AroIR::getAnguloSinFiltro() {
  //return filterAngulo.GetLowPass();
  return angulo;
}
double AroIR::getHighPass() {
 return filterAngulo.GetHighPass();

}

double AroIR::getLowPass()  {
 return filterAngulo.GetLowPass();
}

double AroIR::getStrength() {
  return filterStr.GetLowPass();

  //return strength;
}
