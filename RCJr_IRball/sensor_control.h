#ifndef __SENSOR_CONTROL__
#define __SENSOR_CONTROL__

#include "Arduino.h"
#define IR_NUM 12

// ハードウェア依存の変数
const uint8_t   SensorPins[IR_NUM]  = {14, 7, 16, 5, 17, 10, 2, 9, 6, 8, 15, 3};
const float     unitVectorX[IR_NUM] = {0.000, 0.500, 0.866, 1.000, 0.866, 0.500, 0.000, -0.500, -0.866, -1.000, -0.866, -0.500};
const float     unitVectorY[IR_NUM] = {1.000, 0.866, 0.500, 0.000, -0.500, -0.866, -1.000, -0.866, -0.500, 0.000, 0.500, 0.866};
const float     deltaPulseWidth     = 2.0;

typedef struct {
    int activeSensors;      // 反応したセンサの個数
    int maxPulseWidth;      // 最大のセンサ値
    int maxSensorNumber;    // 最大の値を観測したセンサの番号
    int avgPulseWidth;
} sensorInfo_t;

typedef struct {
    float x;
    float y;
} vectorXY_t;

typedef struct {
    float radius;
    float theta;
} vectorRT_t;

void setAllSensorPinsInput(void);
bool getSensorPin(uint8_t pin);
sensorInfo_t getAllSensorPulseWidth(float pulseWidth[IR_NUM], uint16_t timeLimit);
vectorXY_t calcVectorXYFromPulseWidth(float *pulseWidth);
vectorRT_t calcRTfromXY(vectorXY_t *vectorXY_p);

#endif
