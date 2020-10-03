#ifndef _LEGCONFIG_H_
#define _LEGCONFIG_H_

struct LegConfig {
    LegConfig ( int _ID, int _encoderPin1, int _encoderPin2, int _breakerPin, 
        int _driverPin1, int _driverPin2, int _driverPinPWM, 
        float _zeroPositionFromBreaker ):
        ID(_ID), encoderPin1(_encoderPin1), encoderPin2(_encoderPin2), 
        breakerPin(_breakerPin), driverPin1(_driverPin1), 
        driverPin2(_driverPin2), driverPinPWM(_driverPinPWM), 
        zeroPositionFromBreaker(_zeroPositionFromBreaker)
    {}

    int ID;

    int encoderPin1;
    int encoderPin2;
    int breakerPin;

    int driverPin1;
    int driverPin2;
    int driverPinPWM;
    
    float zeroPositionFromBreaker;
    
};

#endif