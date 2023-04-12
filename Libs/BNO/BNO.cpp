#include "Arduino.h"
#include "BNO.h"

BNO::BNO() {
}

void BNO::iniciar() {
  if (!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
}

void BNO::readValues() {
  sensors_event_t orientationData;
  sensors_event_t magnetometerData;

  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);

  mag = (double)magnetometerData.magnetic.y;

  yaw = (double)orientationData.orientation.x;
  yaw = (yaw > 180) ? -1 * (360 - yaw) : yaw;
  //yaw *= -1;

  if (offset > 0) {
    yaw = (yaw > 0) ? (180 - yaw) * -1 : (-180 - yaw) * -1;
  }

  right = (yaw < 0) ? true : false;


}


double BNO::getYaw() {
  return yaw;
}

double BNO::getMag() {
  return mag;
}

void BNO::setOffset(double off) {
  offset = off;
}

bool BNO::isRight() {
  return right;
}
