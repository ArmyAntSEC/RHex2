#ifndef _MOTORPOSITIONSCHEULER_H_
#define _MOTORPOSITIONSCHEULER_H_

#include <math.h>

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
            double masterAngleRev = masterClock->getAngleRev();
            double angleRev = computeActualLegAngle ( masterAngleRev, 1 );
            pid->setWantedPositionRev( angleRev, now );	
            pid->run(now);
        }
        
    private:    
        LegPacingMasterClock * masterClock;

        double time_s = 2 * M_PI * 0.5;                 
        double phi_s = 2 * M_PI * 0.25;
        double phi_o = 0;
        

        double computeActualLegAngle( double masterAngle, double contactFactor )
        {
            double time_sp = time_s * contactFactor;
            double phi_r = 0;
            if ( masterAngle < time_sp ) 
            {
                phi_r = phi_o + phi_s / time_sp * masterAngle;
            } else {
                double phi_u = 2*M_PI - phi_s;
                double time_u = 2*M_PI - time_sp;
                phi_r = phi_o + phi_s + phi_u / time_u * (masterAngle - time_sp);
            }
            return phi_r;            
        }

};

#endif