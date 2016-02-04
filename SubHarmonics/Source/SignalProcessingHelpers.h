/*
  ==============================================================================

    SignalProcessingHelpers.h
    Created: 1 Oct 2015 6:57:23pm
    Author:  domin

  ==============================================================================
*/

#ifndef SIGNALPROCESSINGHELPERS_H_INCLUDED
#define SIGNALPROCESSINGHELPERS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================


enum {
	filterTypeHighPass = 0,
	filterTypeLowPass,
	filterTypeBandPass,
	filterTypeAllPass,

	filterOrder1 = 1,
	filterOrder2 = 2,
	filterOrder4 = 4,
	filterOrder6 = 6,
	filterOrder8 = 8,
	filterOrder10 = 10
};


//==============================================================================


class BiquadFilter {
public:
	BiquadFilter(int filterType, int filterOrder);
	~BiquadFilter(){}

	void processFilter(float *sample, int channel);
	void setFilterCoeffs(float sr, float f, float q);
	void flushBuffer();

private:
	int filterType;
	int filterOrder;
	
	float coeffB0;
	float coeffB1;
	float coeffB2;
	float coeffA1;
	float coeffA2;

	float buffer[2][2][5]; // <- Buffer array for 2 delays, 2 channels and 5 instances of computation
};


//==============================================================================


class PeakDetector {
public:
	PeakDetector() {}
	~PeakDetector() {}

	float calcEnvelope(float sample, float timeConstant, int sr);
	void flushVC();

private:
	float vc;
};


//==============================================================================


class Compressor {
public:
	Compressor() : peakDetector(new PeakDetector) {}
	~Compressor() { delete peakDetector; }

	float calcGain(float sample, float threshold, float ratio, float release, int sr);
	void flushDetector();

private:
	PeakDetector *peakDetector;
};


//==============================================================================


/** A Ramper applies linear ramping to a value.
*    @ingroup utility
*
*/
class Ramper
{
public:
	Ramper();
	~Ramper() {}

	void setStepAmount(int newStepAmount);
	void setTarget(float currentValue, float newTarget, int numberOfSteps);
	void setValue(float newValue);

	bool ramp(float &valueToChange);

private:
	float targetValue;
	float stepDelta;
	int stepAmount;
};


//==============================================================================

float convertDBtoFloat(float dbValue) {
	return pow(10, dbValue / 20);
}

//==============================================================================


#endif  // SIGNALPROCESSINGHELPERS_H_INCLUDED
