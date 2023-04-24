#ifndef Ultrasonico_h
  #define Ultrasonico_h

    #include "Arduino.h"
    


  class Ultrasonico {

    private:
      int trigPin;
      int echoPin;


    public:
      Ultrasonico(int trigPin, int echoPin);
      void iniciar();
      double getDistancia();
      
  
  };

#endif