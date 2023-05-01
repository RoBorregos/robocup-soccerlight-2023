#include "Arduino.h"
#include "PID.h"

PID::PID() {
}

void PID::setKP(double P) {
  kP = P;
}

double PID::getKP() {
  return kP;
}


double PID::calcularError(int objetivo, int cur, int velocidad) {
  error = abs(objetivo - cur);
  //if (error <= 5) return 0;
  error = min(error,angle);
  error *= kP;
  //error = min(kP * error, angle); //Máximo rango de 70 grados
  //error = map(error, 0, angle, minToMove, 255);

  return error;
}

double PID::calcularError2(int objetivo, int cur, int velocidad) {
  error = abs(objetivo - cur);
  if (error <= 5) return 0;
  error = min(error,angle);
  //error *= kP;
  error = min(kP * error, angle); //Máximo rango de 70 grados
  error = map(error, 0, angle, minToMove, 255);

  return error;
}



void PID::setR(bool r) {
  this->r = r;
}

void PID::setMinToMove(int min) {
  minToMove = min;
}

bool PID::getR() {
  return r;
}

int PID::getMinToMove() {
  return minToMove;
}

void PID::setAngle(double angle) {
  this->angle = angle;
}
