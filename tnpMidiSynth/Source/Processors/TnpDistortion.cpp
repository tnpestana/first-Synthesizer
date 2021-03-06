/*
  ==============================================================================

    TnpDistortion.cpp
    Created: 11 Aug 2019 6:22:42pm
    Author:  Tiago Pestana

  ==============================================================================
*/

#include "TnpDistortion.h"

TnpDistortion::TnpDistortion()
: type(0),
inputGain(0.0f),
mix(0)
{
    
}

TnpDistortion::~TnpDistortion()
{
    
}

void TnpDistortion::updateParameters(int type, float inputGain, float mix)
{
    this->type = type;
    this->inputGain = powf(10.0f, inputGain / 20.0f);
    this->mix = mix * 0.01;
}

void TnpDistortion::prepareToPLay(double samplerate)
{
    
}

void TnpDistortion::processAudioBlock(AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        float* channelData = buffer.getWritePointer(channel);
        
        for(int i = 0; i < buffer.getNumSamples(); i++)
        {
            const float in = *channelData * inputGain;
            float out;
    
            switch(type)
            {
                case 0: /*hard clipping*/
                {
                    float threshold = 1.0f;
                    
                    if (in > threshold)
                        out = threshold;
                    else if (in < -threshold)
                        out = -threshold;
                    else
                        out = in;
                    break;
                }
                case 1: /*soft clipping*/
                {
                    float threshold1 = 1.0f/3.0f;
                    float threshold2 = 2.0f/3.0f;
                    
                    if (in > threshold2)
                        out = 1.0f;
                    else if (in > threshold1)
                        out = (3.0f - (2.0f - 3.0f * in) * (2.0f - 3.0f * in)) / 3.0f;
                    else if (in < -threshold2)
                        out = -1.0;
                    else if (in < -threshold1)
                        out = -(3.0f - (2.0f + 3.0f * in) * (2.0f + 3.0f * in)) / 3.0f;
                    else
                        out = 2.0f * in;
                    break;
                }
                case 2: /*soft clipping exponential*/
                {
                    if (in > 0)
                        out = 1.0f - expf(-in);
                    else
                        out = -1.0f + expf(in);
                    break;
                }
                case 3: /*full wave rectifier*/
                {
                    out = fabsf(in);
                    break;
                }
                case 4: /*half wave rectifier*/
                {
                    if (in > 0)
                        out = in;
                    else
                        out = 0;
                    break;
                }
            }
            
            // Compensate for gain losses due to low input gain
            // out = out * ((powf(10.0f, 6.0f / 20.0f) + powf(10.0f, -48.0f / 20.0f)) - powf(10.0f, inputGain / 20.0f));
            
            *channelData = (out * mix) + (in * (1 - mix));
            channelData++;
        }
    }
}
