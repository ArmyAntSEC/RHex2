#ifndef _ENCODERWRAPPER_H_
#define _ENCODERWRAPPER_H_

#include <HomingEncoder.h>
#include "legConfig.h"

class EncoderWrapper : public HomingEncoder
{
    public:
    template <int N> void init( LegConfig * conf )        
    {
        HomingEncoder::init<N>( conf->encoderPin1, conf->encoderPin2, conf->breakerPin );
    }
};
#endif