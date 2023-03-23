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

    void setKP(double P) {
      kP = P;
    }

    double getKP() {
      return kP;
    }


    int calcularError(int objetivo, int cur, int velocidad) {
      error = abs(objetivo - cur);
      if (error <= 5) return 0;
      error = min(kP * error, 60);
      error = map(error, 0, 60, minToMove, 255);

      return error;
    }


};
