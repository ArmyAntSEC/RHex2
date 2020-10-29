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
        LegForwardLeft( unsigned long int _rate, LegPacingMasterClock * _masterClock ): 
            LegProcessor(_rate, _masterClock), pwmWrapper(driverPinPWM_HW)    
        {
            encoderPin1 = 2;
            encoderPin2 = 3;
            breakerPin = A0;
            legOffset = 1500;
            
            driverPin1 = 4;
            driverPin2 = 5;   
            driverPWM = &pwmWrapper;     

            Kp = 500;
            Ki = 0;
            Kd = 0;            
        }

        void init()
        {
            LegProcessor::init<1>();
        }
        
};    
#endif