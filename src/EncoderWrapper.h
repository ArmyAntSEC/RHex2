#ifndef _ENCODERWRAPPER_H_
#define _ENCODERWRAPPER_H_

#include <Encoder.h>
#include "legConfig.h"

class EncoderWrapper : public Encoder
{
    public:
    EncoderWrapper ( LegConfig * conf ):
        Encoder( conf->encoderPin1, conf->encoderPin2, conf->breakerPin )
    {}
};
#endif