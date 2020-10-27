#ifndef _LEFTFORWARDLEG_H_
#define _LEFTFORWARDLEG_H_

#include "LegProcessor.h"
#include <pwm_lib.h>
using namespace arduino_due::pwm_lib;


class LegForwardLeft : public LegProcessor {
    private:
        pwm<pwm_pin::PWML7_PC24> driverPinPWM_HW; //Pin 6
        pwm_wrapper<pwm<pwm_pin::PWML7_PC24>> pwmWrapper;

    public:
        LegForwardLeft(): 
            pwmWrapper(driverPinPWM_HW)    
        {
            encoderPin1 = 2;
            encoderPin2 = 3;
            breakerPin = A0;
            legOffset = 0;
            
            driverPin1 = 4;
            driverPin2 = 5;   
            driverPWM = &pwmWrapper;     
        }

        void init()
        {
            LegProcessor::init<1>();
        }
        
};    
#endif