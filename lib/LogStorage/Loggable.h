#ifndef _LOGGABLE_H_
#define _LOGGABLE_H_

#include <Arduino.h>
#include "LogStorage.h"

class Loggable
{
    private: 
        String className = "N/A";
        String ID = "N/A";

    public:
        void setID ( String _class, String _ID )
        {
            className = _class;
            ID = _ID;
        }
    
    protected:
        LogStorage& log(unsigned long int now)
        {
            Log << "[" << now/1000.0 <<  "] " << className << "(" << ID << "): ";
            return Log;
        }
};

#endif