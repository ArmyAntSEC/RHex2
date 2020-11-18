#ifndef MOTORREGULATOR_H_
#define MOTORREGULATOR_H_

#include <HomingEncoder.h>
#include "MotorDriver.h"

class MotorRegulator
{
    public:	
        virtual void run(unsigned long int now) = 0;
        
        virtual void init( MotorDriver* _driver, HomingEncoder * _encoder )
        {
            driver = _driver;
            encoder = _encoder;	
            setPointRev = 0;	
        }

        void setWantedPositionRev( float _setPointRev, unsigned long int now )
        {
            setPointRev = _setPointRev;
        }	
        
        virtual boolean hasSettled( unsigned long int now ) = 0;	

        virtual void setMaxSpeed( unsigned int _maxSpeed ) = 0;

        float getCurrentPosRev()
        {
            float currentPositionClicks = encoder->read();
            
            return currentPositionClicks/3591.84; //Convert clicks to rotations.            
        }
        
    protected:	
        MotorDriver * driver;
        HomingEncoder * encoder;	
        float setPointRev;	        

    protected:
        inline float angleDifferenceRev( float angle1Rev, float angle2Rev )
        {
            float diff = fmod(( angle2Rev - angle1Rev + 0.5 ), 1) - 0.5;
            return diff < -0.5 ? diff + 1 : diff;
        }
};


#endif