#include "Arduino.h"

class AroIR {

  private:
    double angulo = 1.0;
    double strength = 1.0;
    double offset = 0.0;


  public:
    AroIR() {
    }

    void iniciar() {
      Serial2.begin(115200);
      Serial2.setTimeout(100);
    }


    //Leer datos del aro IR
    void actualizarDatos() {
      if (Serial2.available()) {
        Serial2.println("serial1");
        String input = Serial2.readStringUntil('\n');
       
        if (input[0] == 'a') {
          angulo = input.substring(2, input.length()).toDouble();
          angulo += offset;
        }
        else
          strength = input.substring(2, input.length()).toDouble();
      }
    }


    //Getters
    double getAngulo() {
      return angulo;
    }

    double getStrength() {
      return strength;
    }

    



};
