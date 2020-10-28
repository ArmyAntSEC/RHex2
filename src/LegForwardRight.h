#ifndef _RIGHTFORWARDLEG_H_
#define _RIGHTFORWARDLEG_H_

#include "LegProcessor.h"

class LegForwardRight : public LegProcessor {
    private:
        pwm<pwm_pin::PWML4_PC21> driverPinPWM_HW; //Pin 9
        pwm_wrapper<pwm<pwm_pin::PWML4_PC21>> pwmWrapper;

    public:
        LegForwardRight():
            pwmWrapper(driverPinPWM_HW)  
        {        
            encoderPin1 = 10;
            encoderPin2 = 11;
            breakerPin = A1;
            legOffset = 1500;
            
            driverPin1 = 8;
            driverPin2 = 7;  
            driverPWM = &pwmWrapper;

            Kp = 1000;
            Ki = 0;
            Kd = 0;
            sampleTime = 100;
        }

        void init()
        {
            LegProcessor::init<2>();
        }
        
};    
#endif