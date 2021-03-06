/*
  ==============================================================================

    TnpDelayLine.cpp
    Created: 4 Aug 2018 6:09:20pm
    Author:  Tiago Pestana

  ==============================================================================
*/

#include "TnpDelayLine.h"

TnpDelayLine::TnpDelayLine()
	: buffer(nullptr),
    bufferSize(0),
    sampleRate(0),
    delayReadPosition(0),
    delayWritePosition(0),
	targetDelayLength(0.0f),
	currentDelayLength(0.0f),
	delayInSamples(1),
	feedback(0.0),
	wetMix(0.0),
	smoothDelayLength(0.0f)
{
}

TnpDelayLine::~TnpDelayLine()
{
	delete[] buffer;
}

void TnpDelayLine::resetDelay(double sampleRate)
{
	if (buffer)
		memset(buffer, 0, bufferSize);

	delayReadPosition = 0.0;
	delayWritePosition = 0.0;

	this->sampleRate = sampleRate;
	//smoothDelayLength.reset(sampleRate, 0.5);
	initBuffer();
}

void TnpDelayLine::initBuffer()
{
	// Set 2 second max delay in samples.
	bufferSize = (int)(2.0 * sampleRate);
	if (bufferSize < 1)
		bufferSize = 1;
	if (buffer)
		delete[] buffer;

	// create new buffer
	buffer = new float[bufferSize];
	// flush buffer
	memset(buffer, 0, bufferSize);

	setupBuffer();
}

void TnpDelayLine::setFeedback(float feedback)
{
	this->feedback = feedback;
}

void TnpDelayLine::setWetMix(float wetMix)
{
	this->wetMix = wetMix;
}

void TnpDelayLine::setDelayTime(double delayTime_ms)
{
	//smoothDelayLength.setValue(delayTime_ms);
	targetDelayLength = delayTime_ms;
}

void TnpDelayLine::setupBuffer()
{
	delayInSamples = currentDelayLength * sampleRate;
	//delayInSamples = smoothDelayLength.getNextValue() * sampleRate;
	delayReadPosition = (int)(delayWritePosition - delayInSamples);

	// Wrap delay read position to the buffers range.
	if (delayReadPosition < 0)
		delayReadPosition += bufferSize;
}

void TnpDelayLine::processAudio(float* inputSample)
{
	// Sep up the buffer 
	//if (smoothDelayLength.isSmoothing())
	//	setupBuffer();

	if (currentDelayLength != targetDelayLength)
	{
		currentDelayLength += (targetDelayLength - currentDelayLength) / sampleRate;
		setupBuffer();
	}

	float output = 0.0;
	float yn = buffer[delayReadPosition];
	float xn = *inputSample;
	//if (smoothDelayLength.getNextValue() == 0)
	//	yn = xn;
	if (currentDelayLength == 0.0f)
		yn = xn;
	buffer[delayWritePosition] = xn + feedback * yn;
	output = (xn * (1 - wetMix)) + (yn * wetMix);
	*inputSample = output;

	delayReadPosition++;
	if (delayReadPosition >= bufferSize)
		delayReadPosition = 0;
	delayWritePosition++;
	if (delayWritePosition >= bufferSize)
		delayWritePosition = 0;
}
