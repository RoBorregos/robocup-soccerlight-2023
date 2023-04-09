//#include "Arduino.h"
#include "SingleEMAFilterLib.h"

    SingleEMAFilter<int> filterAngulo(0.6);
    SingleEMAFilter<int> filterStr(0.6);


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
          filterAngulo.AddValue(angulo);
        }
        else {
          strength = input.substring(2, input.length()).toDouble();
          filterStr.AddValue(strength);
        }
      }
    }


    //Getters
    double getAngulo() {
      return angulo;
    }

    double getHighPass(){
      return filterAngulo.GetHighPass();

    }

    double getLowPass()  {
      return filterAngulo.GetLowPass();
    }

    double getStrength() {
      return strength;
    }

    



};
