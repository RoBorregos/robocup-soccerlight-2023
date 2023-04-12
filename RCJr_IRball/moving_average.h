#ifndef __MOVING_AVERAGE__
#define __MOVING_AVERAGE__

#include "SingleEMAFilterLib.h"

SingleEMAFilter<int> filterAngulo(0.6);
SingleEMAFilter<int> filterStr(0.6);

class MovingAverage {
private:
    float *dataArray;
    uint16_t sizeOfDataArray;
    uint16_t count;
public:
    MovingAverage(uint16_t num_array);
    float updateData(float value);
    void reset(void);
};

MovingAverage::MovingAverage(uint16_t num_array) {
    dataArray = new float[num_array];
    sizeOfDataArray = num_array;
    count = 0;
}

float MovingAverage::updateData(float value) {
    //filterAngulo.AddValue(value);
    
    count++;
    if (count >= sizeOfDataArray) count = 0;
    dataArray[count] = value;
 
    float rslt = 0;
    for (int i = 0; i < sizeOfDataArray; ++i) {
        rslt += dataArray[i];
    }
    
    return rslt / sizeOfDataArray;

      //return filterAngulo.getLowPass();
    
    
}

void MovingAverage::reset(void) {
    for (int i = 0; i < sizeOfDataArray; ++i) {
        dataArray[i] = 0;
    }
}

#endif
