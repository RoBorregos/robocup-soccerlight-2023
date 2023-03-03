class PID {
  private:
    int kP;
    int kI;
    int kD;
    int error;
    int errorPrev;
    const int minToMove = 100;

    
  public:
    PID() {
    }
    
    PID(int kP) {
      this->kP = kP;
    }
    void setPID(int kP, int kD){
      setKP(kP);
      setKD(kD);
    }
    
    void setKP(int kP){
      this->kP = kP;
    }

    void setKD(int kD) {
      this->kD = kD;
    }
    
    int getKP() {
      return kP;
    }

    int getKD() {
      return kD;
    }

    int calcularError(int objetivo, int cur, int velocidad){
      error = abs(objetivo - cur);
      if (error <= 5) return 0;
      error = min(kP*error, 30);
      error = map(error, 0, 30, minToMove, 255);   
      
      return error;
    }

    int calcularError2(int objetivo, int cur, int velocidad){
          error = objetivo - cur;  
          error = min(error,255);   
          int d = kD*(error - errorPrev);
          errorPrev = error;
          
          return kP*error + d;        
    }
    

    //    int giroMasPreciso(int objetivo, int velocidad, int prevError){
//
//      int kP = 0.2;
//      
//      int actual = imu.readValue();
//      int error = objetivo - actual;
//      
//      int kD = (error - prevError)/millis();
//      int velocidadMotores = kP*error;
//
//      while (error != 0){
//        giro(velocidadMotores);
//        error = objetivo - imu.readValue();
//        velocidadMotores = kP*error + kD;
//        
//      }
//
//        apagarMotores();
//      //return velocidadMotores;
//
//   
//    }
    
};
