class PID {
  private:
    double kP;
    double kI;
    double kD;
    double error;
    double errorPrev;
    const int minToMove = 100;


  public:
    PID() {
    }

    PID(double P) {
      kP = P;
    }
    void setPID(double kP, double kD) {
      setKP(kP);
      setKD(kD);
    }

    void setKP(double P) {
      kP = P;
    }

    void setKD(double kD) {
      this->kD = kD;
    }

    double getKP() {
      return kP;
    }

    double getKD() {
      return kD;
    }

    int calcularError(int objetivo, int cur, int velocidad) {
      error = abs(objetivo - cur);
      if (error <= 5) return 0;
      error = min(kP * error, 60);
      error = map(error, 0, 60, minToMove, 255);


      return error;
    }

    //    int calcularError2(int objetivo, int cur, int velocidad){
    //          error = objetivo - cur;
    //          error = min(error,255);
    //          int d = kD*(error - errorPrev);
    //          errorPrev = error;
    //
    //          return kP*error + d;
    //    }


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
    //    }

};
