#include "sensor_control.h"

void setAllSensorPinsInput(void) {
    for (int i = 0; i < IR_NUM; ++i) {
        pinMode(SensorPins[i], INPUT);
    }
}

/*
 デジタル読み込みの高速化のためのポート操作
 詳しくは以下を参照
 http://www.musashinodenpa.com/arduino/ref/index.php?f=0&pos=849

 digitalRead()で一般化されていないため、ハードウェア依存の実装
 */
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

   // return digitalRead(SensorPins[pin]);
}

sensorInfo_t getAllSensorPulseWidth(float pulseWidth[IR_NUM], uint16_t timeLimit) {
  Serial.begin(115200);
    // 実験用データを格納する構造体
    sensorInfo_t sensorInfo;

    // pulseWidth[]は加算計算用変数なので最初に初期化する  //iniciar la variable
    for(int i = 0; i < IR_NUM; i++) {
        pulseWidth[i] = 0;
    }

//Todos 


     //do-whileで時間(833us)を監視しながらセンサの読み込み
//    const unsigned long startTime_us = micros();
//    do {
//        for (int i = 0; i < IR_NUM; i++) {
//            if(getSensorPin(i) == false) {
//                pulseWidth[i] += deltaPulseWidth;
//            }
//        }
//    } while((micros() - startTime_us) < timeLimit);


  //Usando otro deltaPulsewidth
        const unsigned long startTime_us = micros();
    do {
        for (int i = 0; i < IR_NUM; i++) {
            if(getSensorPin(i) == false) {
                pulseWidth[i] += deltaPulseWidth;
                //pulseWidth[i] += int(micros() - startTime_us);
            }
        }
    } while((micros() - startTime_us) < timeLimit);


//        for (int i = 0; i < IR_NUM; i++) {
//         unsigned long startTime_us = micros();
//
//          do {
//            if(getSensorPin(i) == false) {
//                pulseWidth[i] += deltaPulseWidth;
//            }
//            
//          } while((micros() - startTime_us) < timeLimit); 
//        }

        

    // ベクトル計算のみでセンシングを行うのであれば以下の実装は不要
    sensorInfo.activeSensors    = 0; // ボールに反応しているセンサの個数
    sensorInfo.maxPulseWidth     = 0; // 一番反応の強いセンサのパルス幅
    sensorInfo.maxSensorNumber  = -1; // 一番反応の強いセンサの番号
    for(int i = 0; i < IR_NUM; i++) {
        if(pulseWidth[i] > 0) {
            sensorInfo.activeSensors += 1;
        }
        if(pulseWidth[i] > sensorInfo.maxPulseWidth) {
            sensorInfo.maxPulseWidth = pulseWidth[i];
            sensorInfo.maxSensorNumber = i;
            
        }
    }
    //Serial.println(sensorInfo.maxSensorNumber);
    return sensorInfo;
}

vectorXY_t calcVectorXYFromPulseWidth(float *pulseWidth) {
    vectorXY_t rslt = {0, 0};
    for(int i = 0; i < IR_NUM; i++) {
        rslt.x += pulseWidth[i] * unitVectorX[i];
        rslt.y += pulseWidth[i] * unitVectorY[i];
    }

    return rslt;
}

//int getMaxSensor(float *pulseWidth) {
//    return sensorInfo_t.maxSensorNumber;
//} 

vectorRT_t calcRTfromXY(vectorXY_t *vectorXY_p) {
    vectorRT_t rslt;
    rslt.radius  = sqrt(pow(vectorXY_p->x, 2.0) + pow(vectorXY_p->y, 2.0));
    rslt.theta   = atan2(vectorXY_p->x, vectorXY_p->y) / PI * 180.0;

    return rslt;
}
