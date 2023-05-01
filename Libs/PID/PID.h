#ifndef PID_h
  #define PID_h

class PID {
  private:
    double kP;
    double kI;
    double kD;
    double error;
    double errorPrev;
    int minToMove = 60;
    bool r = false;
    double angle = 100;


  public:

    PID();
    void setKP(double P);
    double getKP();
    double calcularError(int objetivo, int cur, int velocidad);
    void setR(bool r);
    bool getR();
    //double 
    void setMinToMove(int min);
    int getMinToMove();
    void setAngle(double angle);
    double calcularError2(int objetivo, int cur, int velocidad);


};

#endif
