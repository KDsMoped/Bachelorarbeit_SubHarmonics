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
	~BiquadFilter();

	void processFilter(float *leSample, int leChannel);
	void setFilterCoeffs(float sr, float f, float q);
	void flushRingBuffer();

private:
	int filterType;
	int filterOrder;
	
	float coeffB0;
	float coeffB1;
	float coeffB2;
	float coeffA1;
	float coeffA2;

	float ringBuffer[3][2][5];
};


//==============================================================================


/** A Ramper applies linear ramping to a value.
*    @ingroup utility
*
*/
class Ramper
{
public:
	Ramper() : targetValue(0.0f),
			   stepDelta(0.0f),
			   stepAmount(-1)
	{};

	/** Sets the step amount that the ramper will use. You can overwrite this value by supplying a step number in setTarget. */
	void setStepAmount(int newStepAmount) { stepAmount = newStepAmount; };
	
	/** sets the new target and recalculates the step size using either the supplied step number or the step amount previously set by setStepAmount(). */
	void setTarget(float currentValue, float newTarget, int numberOfSteps = -1) {
		if (numberOfSteps != -1) stepDelta = (newTarget - currentValue) / numberOfSteps;
		else if (stepAmount != -1) stepDelta = (newTarget - currentValue) / stepAmount;
		else jassertfalse; // Either the step amount should be set, or a new step amount should be supplied
		targetValue = newTarget;
	};
	
	/** Sets the ramper value and the target to the new value and stops ramping. */
	void setValue(float newValue) {
		targetValue = newValue;
		stepDelta = 0.0f;
	};

	/** ramps the supplied value and returns true if the targetValue is reached. */
	inline bool ramp(float &valueToChange) {
		valueToChange += stepDelta;
		return abs(targetValue - valueToChange) > 0.001;
	};

private:
	float targetValue, stepDelta;
	int stepAmount;
};

	//==============================================================================

#endif  // SIGNALPROCESSINGHELPERS_H_INCLUDED
