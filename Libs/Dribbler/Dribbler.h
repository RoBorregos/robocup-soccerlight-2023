#ifndef Dribbler_h
  #define Dribbler_h

  #include <Arduino.h>
  #include <Servo.h>

  class Dribbler {
    private:
        Servo esc;
        int pin;

    public:
        Dribbler(int pin);
        void iniciar();
        void prender(int vel);
        void apagar();
        
  };


#endif