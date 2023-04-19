#ifndef PID_h
  #define PID_h

class PID {
  private:
    double kP;
    double kI;
    double kD;
    double error;
    double errorPrev;
    int minToMove = 110;
    bool r = false;
    double angle = 90;


  public:

    PID();
    void setKP(double P);
    double getKP();
    int calcularError(int objetivo, int cur, int velocidad);
    void setR(bool r);
    bool getR();
    void setMinToMove(int min);
    int getMinToMove();
    void setAngle(double angle);


};

#endif
