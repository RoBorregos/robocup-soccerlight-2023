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
      unsigned long* current_time;
      unsigned long last_time = 0;
  
  
    public:
      AroIR();
      void iniciar(unsigned long* current_time);
      void iniciar2(unsigned long* current_time);
      void actualizarDatos();
      void actualizarDatos2();
      void setOffset(double offset);
      double getAngulo();
      double getAnguloSinFiltro();
      double getHighPass();
      double getLowPass();
      double getStrength();
      double cutString1(String str);
  
  };
  
#endif
