#ifndef AroIR_h
  #define AroIR_h
  #include "Arduino.h"
  
  //#include "SingleEMAFilterLib.h"
  
//  SingleEMAFilter<int> filterAngulo(0.6);
//  SingleEMAFilter<int> filterStr(0.6);
  
  
  class AroIR {
  
    private:
      double angulo = 1.0;
      double strength = 1.0;
      double offset = 0.0;
  
  
    public:
      AroIR();
      void iniciar();
      void iniciar2();
      void actualizarDatos();
      void actualizarDatos2();
      double getAngulo();
      double getHighPass();
      double getLowPass();
      double getStrength();
      double cutString1(String str);
  
  };
  
#endif
