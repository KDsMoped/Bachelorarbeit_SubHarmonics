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


#endif  // SIGNALPROCESSINGHELPERS_H_INCLUDED
