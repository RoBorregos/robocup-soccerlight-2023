
#include "sensor_control.h"


#define T_MODEA 833



unsigned long time_ms = 0;

void setup() {
    Serial.begin(115200);
    setAllSensorPinsInput();
}

void loop() {

    float           pulseWidth[IR_NUM]; 
    sensorInfo_t    sensorInfo;         
    vectorXY_t      vectorXY;          
    vectorRT_t      vectorRT;          
    vectorRT_t      vectorRTWithSma;    
    
    sensorInfo  = getAllSensorPulseWidth(pulseWidth, T_MODEA);
    vectorXY    = calcVectorXYFromPulseWidth(pulseWidth);
    vectorRT    = calcRTfromXY(&vectorXY);



    if (millis() - time_ms > 50) {
        time_ms = millis();

      printAngulo(&vectorRTWithSma);
      Serial.print("r ");
      Serial.print(sensorInfo.avgPulseWidth);
      Serial.print("\n");


    }
}

void printAngulo(vectorRT_t *self) {
    Serial.print("a ");
    Serial.print(self->theta);
    Serial.print("\n");
}
