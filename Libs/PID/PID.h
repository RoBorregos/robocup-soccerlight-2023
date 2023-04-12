#ifndef PID_h
  #define PID_h

class PID {
  private:
    double kP;
    double kI;
    double kD;
    double error;
    double errorPrev;
    const int minToMove = 110;
    bool r = false;


  public:

    PID();
    void setKP(double P);
    double getKP();
    int calcularError(int objetivo, int cur, int velocidad);
    void setR(bool r);
    bool getR();


};

#endif
