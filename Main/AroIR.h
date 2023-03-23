class AroIR {

  private:
    double angulo = 0.0;
    double strength = 0.0;


  public:
    AroIR() {
      Serial3.begin(115200);
    }


    //Leer datos del aro IR
    void actualizarDatos() {
      if (Serial3.available()) {
        String input = Serial3.readStringUntil('\n');
        if (input[0] == 'a') {
          angulo = input.substring(2, input.length()).toDouble();
          angulo -= 90;
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
