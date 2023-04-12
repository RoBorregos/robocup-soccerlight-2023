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


int PID::calcularError(int objetivo, int cur, int velocidad) {
  error = abs(objetivo - cur);
  if (error <= 5) return 0;
  error = min(kP * error, 70);
  error = map(error, 0, 70, minToMove, 255);

  return error;
}

void PID::setR(bool r) {
  this->r = r;
}

bool PID::getR() {
  return r;
}
