#ifndef _MOTORPOSITIONSCHEULER_H_
#define _MOTORPOSITIONSCHEULER_H_

#include <math.h>

#include "MotorStateHandlerImpl.h"
#include "LegPacingMasterClock.h"

#include "LogStorage.h"

class MotorPositionScheduler: public MotorStateHandlerImpl,
    public Loggable // @suppress("Class has a virtual method and non-virtual destructor")
{
    public:
        virtual void init( MotorStateHandler * _handler, MotorDriver * _driver, 
            HomingEncoder * _encoder, MotorRegulator * _regulator, LegPacingMasterClock * _masterClock )
        {
            MotorStateHandlerImpl::init( _handler, _driver, _encoder, _regulator );
            masterClock = _masterClock;
        }

        virtual void restart( unsigned long int _now )
        {
            this->regulator->setMaxSpeed(255);
        }
        
        virtual void run(unsigned long int now)
        {				
            double masterAngleRev = masterClock->getAngleRev();            
            double angleRev = computeActualLegAngleRev ( masterAngleRev, 1 );
            regulator->setWantedPositionRev( angleRev, now );	
            regulator->run(now);
            log(now) << "Master Angle: " << masterAngleRev << 
            ", Actual wanted angle: " << angleRev << endl;
        }
        
    private:    
        LegPacingMasterClock * masterClock;

        double time_s = 0.5;                 
        double phi_s = 0.25;
        double phi_o = 0;
        

        double computeActualLegAngleRev( double masterAngleRev, double contactFactor )
        {
            double time_sp = time_s * contactFactor;
            double phi_r = 0;
            if ( masterAngleRev < time_sp ) 
            {
                phi_r = phi_o + phi_s / time_sp * masterAngleRev;
                //Log << "Scheduler (Contact): " << "MaterAngle: " << masterAngleRev << " Phi_r: " << phi_r << endl;
            } else {
                double phi_u = 1 - phi_s;
                double time_u = 1 - time_sp;
                phi_r = phi_o + phi_s + phi_u / time_u * (masterAngleRev - time_sp);
                //Log << "Scheduler (Air): " << "MaterAngle: " << masterAngleRev << " Phi_r: " << phi_r << endl;
            }
            return phi_r;  
        }

};

#endif