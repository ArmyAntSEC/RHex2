#ifndef _MOTORPOSITIONSCHEULER_H_
#define _MOTORPOSITIONSCHEULER_H_

#include "MotorStateHandlerImpl.h"
#include "LegPacingMasterClock.h"

class MotorPositionScheduler: public MotorStateHandlerImpl // @suppress("Class has a virtual method and non-virtual destructor")
{
    public:
        virtual void init( MotorStateHandler * _handler, MotorDriver * _driver, 
            HomingEncoder * _encoder, MotorPIDRegulator * _pid, LegPacingMasterClock * _masterClock )
        {
            MotorStateHandlerImpl::init( _handler, _driver, _encoder, _pid );
            masterClock = _masterClock;
        }

        virtual void run(unsigned long int now)
        {				
            double angleRev = masterClock->getAngleRev();
            pid->setWantedPositionRev( angleRev, now );	
            pid->run(now);
        }
        
    private:    
        LegPacingMasterClock * masterClock;

};

#endif