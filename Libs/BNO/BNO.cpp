#include "Arduino.h"
#include "BNO.h"

BNO::BNO() {
}

void BNO::iniciar() {
  if (!bno.begin(OPERATION_MODE_IMUPLUS)) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  //NUevo
  bno.setExtCrystalUse(true);
}

void BNO::iniciar2() {
  if (!bno.begin(OPERATION_MODE_NDOF_FMC_OFF)) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  //NUevo
  bno.setExtCrystalUse(true);
}


void BNO::readValues() {
  
  sensors_event_t orientationData;
  //sensors_event_t magnetometerData;

  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  //bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);

  //mag = (double)magnetometerData.magnetic.y;

  yaw = (double)orientationData.orientation.x;
  yaw = (yaw > 180) ? -1 * (360 - yaw) : yaw;
  //yaw *= -1;

  if (offset > 0) {
    yaw = (yaw > 0) ? (180 - yaw) * -1 : (-180 - yaw) * -1;
  }

  right = (yaw < 0) ? true : false;


}

void BNO::readValues2() {
  sensors_event_t event;
  bno.getEvent(&event);

  

  yaw = (double)event.orientation.x;
  yaw = (yaw > 180) ? -1 * (360 - yaw) : yaw;
  //yaw *= -1;

  if (offset > 0) {
    yaw = (yaw > 0) ? (180 - yaw) * -1 : (-180 - yaw) * -1;
  }

  right = (yaw < 0) ? true : false;

}

void BNO::displayCalStatus()
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
   Serial.print("\t");
  // if (!system)
  // {
  //   Serial.print("! ");
  // }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.println(mag, DEC);
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
