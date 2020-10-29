#ifndef _LEGPACINGMASTERCLOCK_H_
#define _LEGPACINGMASTERCLOCK_H_

#include <math.h>
#include "RecurringTask.h"

class LegPacingMasterClock: public RecurringTask 
{
    public:

        LegPacingMasterClock( unsigned int _rate ):
            RecurringTask(_rate)
        {}
        
        void init( unsigned long int now, double _rotationsPerMinute )
        {
            angleRev = 0;
            lastNow = now;
            rotationsPerMinute = _rotationsPerMinute;
            this->start();            
        }

        double getAngleRev()
        {
            return this->angleRev;
        }

        void setRPM ( double _rotationsPerMinute )
        {
            rotationsPerMinute = _rotationsPerMinute;
        }

        void run( unsigned long int now )
        {
            RecurringTask::run(now);
            unsigned long int timeDelta = now - lastNow;
            angleRev += timeDelta*1000*60*rotationsPerMinute;
            angleRev = fmod( angleRev, 2*M_PI );
        }

    private:
        double angleRev;
        unsigned long int lastNow;
        double rotationsPerMinute;
};

#endif