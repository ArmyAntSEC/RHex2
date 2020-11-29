#ifndef _RIGHTFORWARDLEG_H_
#define _RIGHTFORWARDLEG_H_

#include "LegProcessor.h"

class LegForwardRight : public LegProcessor {
    private:
        pwm<pwm_pin::PWML4_PC21> driverPinPWM_HW; //Pin 9
        pwm_wrapper<pwm<pwm_pin::PWML4_PC21>> pwmWrapper;

    public:
        LegForwardRight( unsigned long int _rate, LegPacingMasterClock * _masterClock):
            LegProcessor(_rate, _masterClock),
            pwmWrapper(driverPinPWM_HW)  
        {        
            encoderPin1 = 10;
            encoderPin2 = 11;
            breakerPin = 13;
            outputPin = A1;
            legOffset = 400 + 3592*0.4; //Added 0.4 of a rotation to test the pacing.
            
            contactFactor = 1.2;

            driverPin1 = 8;
            driverPin2 = 7;  
            driverPWM = &pwmWrapper;          
        }

        void init()
        {
            LegProcessor::init<2>();
            this->initiator.setID("Initiator", "RF" );
            this->stepRegulator.setID("StepRegulator", "RF");
            this->pidRegulator.setID("PidRegulator", "RF");            
            this->scheduler.setID("Scheduler", "RF" );
        }
        
};    
#endif