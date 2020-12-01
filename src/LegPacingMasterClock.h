#ifndef _LEGPACINGMASTERCLOCK_H_
#define _LEGPACINGMASTERCLOCK_H_

#include <math.h>
#include "RecurringTask.h"
#include "Loggable.h"

class LegPacingMasterClock: public RecurringTask, public Loggable
{
    public:

        LegPacingMasterClock( unsigned int _rate ):
            RecurringTask(_rate)
        {}
        
        void init( unsigned long int now, double _rotationsPerMinute )
        {
            RecurringTask::init( now );            
            angleRev = 0;
            lastNow = now;
            revolutions = 0;
            rotationsPerMinute = _rotationsPerMinute;                       
        }

        void restart( unsigned long int _now )
        {
            angleRev = 0;
            revolutions = 0;
            lastNow = _now;
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
            lastNow = now;
            angleRev += rotationsPerMinute * timeDelta/1000.0/60.0;
            if ( angleRev > 1 ) {
                revolutions++;
            }

            if ( revolutions > 1 ) { //Just take one step at the time.
                this->stop();
            }
            
            angleRev = fmod( angleRev, 1 );
            //log(now) << " AngleRev: " << angleRev << endl;
        }

    private:
        double angleRev;
        unsigned long int lastNow;
        double rotationsPerMinute;
        int revolutions;
};

#endif