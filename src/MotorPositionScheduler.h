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
            HomingEncoder * _encoder, MotorRegulator * _regulator, 
            LegPacingMasterClock * _masterClock, float _contactFactor )
        {
            MotorStateHandlerImpl::init( _handler, _driver, _encoder, _regulator );
            masterClock = _masterClock;
            contactFactor = _contactFactor;
        }

        virtual void restart( unsigned long int _now )
        {
            this->regulator->setMaxSpeed(255);
        }
        
        virtual void run(unsigned long int now)
        {				
            float masterAngleRev = masterClock->getAngleRev();            
            float angleRev = computeActualLegAngleRev ( masterAngleRev, contactFactor );
            regulator->setWantedPositionRev( angleRev, now );	
            regulator->run(now);
            log(now) << "Master Angle: " << masterAngleRev << 
            ", Actual wanted angle: " << angleRev << endl;
        }
        
    private:    
        LegPacingMasterClock * masterClock;

        float time_s = 0.5;                 
        float phi_s = 0.25;
        float phi_o = 0;

        float contactFactor = 1;
        

        float computeActualLegAngleRev( float masterAngleRev, float contactFactor )
        {            
            if ( masterAngleRev > 1 )
                masterAngleRev = masterAngleRev - 1;

            //Give an offset to the masterAngleRev to make sure 
            float time_sp = time_s * contactFactor;
            float phi_r = 0;
            if ( masterAngleRev < time_sp ) 
            {
                phi_r = phi_o + phi_s / time_sp * masterAngleRev;
                //Log << "Scheduler (Contact): " << "MaterAngle: " << masterAngleRev << " Phi_r: " << phi_r << endl;
            } else {
                float phi_u = 1 - phi_s;
                float time_u = 1 - time_sp;
                phi_r = phi_o + phi_s + phi_u / time_u * (masterAngleRev - time_sp);
                //Log << "Scheduler (Air): " << "MaterAngle: " << masterAngleRev << " Phi_r: " << phi_r << endl;
            }
            
            return phi_r;  
        }

};

#endif