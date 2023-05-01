#include "sensor_control.h"

//Set de todos los pines
void setAllSensorPinsInput(void) {
    for (int i = 0; i < IR_NUM; ++i) {
        pinMode(SensorPins[i], INPUT);
    }
}

//Obtener los valores directamente de los registros
bool getSensorPin(uint8_t pin) {
    switch(pin) {
        case 0:   return PINC&(1<<0);
        case 1:   return PIND&(1<<7);
        case 2:   return PINC&(1<<2);
        case 3:   return PIND&(1<<5);
        case 4:   return PINC&(1<<3);
        case 5:   return PINB&(1<<2);
        case 6:   return PIND&(1<<2);
        case 7:   return PINB&(1<<1);
        case 8:   return PIND&(1<<6);
        case 9:   return PINB&(1<<0);
        case 10:  return PINC&(1<<1);
        case 11:  return PIND&(1<<3);
    }

}

//Obtener el ancho de pulso
sensorInfo_t getAllSensorPulseWidth(float pulseWidth[IR_NUM], uint16_t timeLimit) {
    sensorInfo_t sensorInfo;

    for(int i = 0; i < IR_NUM; i++) {
        pulseWidth[i] = 0;
    }    


    const unsigned long startTime_us = micros();
    do {
        for (int i = 0; i < IR_NUM; i++) {
            if(getSensorPin(i) == false) {
                pulseWidth[i] += deltaPulseWidth;
            }
        }
    } while((micros() - startTime_us) < timeLimit);


    sensorInfo.activeSensors    = 0; 
    sensorInfo.maxPulseWidth     = 0; 
    sensorInfo.maxSensorNumber  = 0; 
    sensorInfo.avgPulseWidth = 0;
    
    for(int i = 0; i < IR_NUM; i++) {
        if(pulseWidth[i] > 0) {
            sensorInfo.activeSensors += 1;
            sensorInfo.avgPulseWidth += pulseWidth[i];
        }
        if(pulseWidth[i] > sensorInfo.maxPulseWidth) {
            sensorInfo.maxPulseWidth = pulseWidth[i];
            sensorInfo.maxSensorNumber = i;
        }
    }


    return sensorInfo;
}

//Cálculo del vector resultante 
vectorXY_t calcVectorXYFromPulseWidth(float *pulseWidth) {
    vectorXY_t rslt = {0, 0};
    for(int i = 0; i < IR_NUM; i++) {
        rslt.x += pulseWidth[i] * unitVectorX[i];
        rslt.y += pulseWidth[i] * unitVectorY[i];
    }

    return rslt;
}

//Cálculo del radio y el ángulo 
vectorRT_t calcRTfromXY(vectorXY_t *vectorXY_p) {
    vectorRT_t rslt;
    rslt.radius  = sqrt(pow(vectorXY_p->x, 2.0) + pow(vectorXY_p->y, 2.0));
    rslt.theta   = atan2(vectorXY_p->x, vectorXY_p->y) / PI * 180.0;

    return rslt;
}
