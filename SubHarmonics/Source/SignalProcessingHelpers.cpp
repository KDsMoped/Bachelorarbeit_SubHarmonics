/*
  ==============================================================================

    SignalProcessingHelpers.cpp
    Created: 1 Oct 2015 6:58:01pm
    Author:  domin

  ==============================================================================
*/

#include "SignalProcessingHelpers.h"


//==============================================================================


BiquadFilter::BiquadFilter(int filterType, int filterOrder) : filterType(filterType), 
															  filterOrder(filterOrder) 
															  {}


void BiquadFilter::processFilter(float *sample, int channel) {
	float x = *sample;
	float w1 = buffer[0][channel][0];
	float w2 = buffer[1][channel][0];
	
	// Compute difference equation
	float w = x - (coeffA1 * w1) - (coeffA2 * w2);
	float y = (coeffB0 * w) + (coeffB1 * w1) + (coeffB2 * w2);

	// Shift buffer values
	buffer[1][channel][0] = w1;
	buffer[0][channel][0] = w;

	if (filterOrder >= 4) {
		x = y;
		w1 = buffer[0][channel][1];
		w2 = buffer[1][channel][1];

		w = x - (coeffA1 * w1) - (coeffA2 * w2);
		y = (coeffB0 * w) + (coeffB1 * w1) + (coeffB2 * w2);
	
		buffer[1][channel][1] = w1;
		buffer[0][channel][1] = w;
	}
	if (filterOrder >= 6) {
		x = y;
		w1 = buffer[0][channel][2];
		w2 = buffer[1][channel][2];

		w = x - (coeffA1 * w1) - (coeffA2 * w2);
		y = (coeffB0 * w) + (coeffB1 * w1) + (coeffB2 * w2);

		buffer[1][channel][2] = w1;
		buffer[0][channel][2] = w;
	}
	if (filterOrder >= 8) {
		x = y;
		w1 = buffer[0][channel][3];
		w2 = buffer[1][channel][3];

		w = x - (coeffA1 * w1) - (coeffA2 * w2);
		y = (coeffB0 * w) + (coeffB1 * w1) + (coeffB2 * w2);

		buffer[1][channel][3] = w1;
		buffer[0][channel][3] = w;
	}
	if (filterOrder >= 10) {
		x = y;
		w1 = buffer[0][channel][4];
		w2 = buffer[1][channel][4];

		w = x - (coeffA1 * w1) - (coeffA2 * w2);
		y = (coeffB0 * w) + (coeffB1 * w1) + (coeffB2 * w2);

		buffer[1][channel][4] = w1;
		buffer[0][channel][4] = w;
	}

	*sample = y;
}


void BiquadFilter::setFilterCoeffs(float sr, float f, float q) {
	float k = tan((float_Pi * f) / sr);

	if (filterType == filterTypeHighPass) {
		if (filterOrder == filterOrder1){
			coeffB0 = 1 / (k + 1);
			coeffB1 = coeffB0 * -1;
			coeffB2 = 0;
			coeffA1 = (k - 1) / (k + 1);
			coeffA2 = 0;
		}
		else {
			coeffB0 = q / ((pow(k, 2) * q) + k + q);
			coeffB1 = (-2 * q) / ((pow(k, 2) * q) + k + q);
			coeffB2 = coeffB0;
			coeffA1 = (2 * q * (pow(k, 2) - 1)) / ((pow(k, 2) * q) + k + q);
			coeffA2 = ((pow(k, 2) * q) - k + q) / ((pow(k, 2) * q) + k + q);
		}
	}
	if (filterType == filterTypeLowPass) {
		if (filterOrder == filterOrder1) {
			coeffB0 = k / (k + 1);
			coeffB1 = coeffB0;
			coeffB2 = 0;
			coeffA1 = (k - 1) / (k + 1);
			coeffA2 = 0;
		}
		else {
			coeffB0 = (pow(k, 2) * q) / ((pow(k, 2) * q) + k + q);
			coeffB1 = (2 * (pow(k, 2) * q)) / ((pow(k, 2) * q) + k + q);
			coeffB2 = coeffB0;
			coeffA1 = (2 * q * (pow(k, 2) - 1)) / ((pow(k, 2) * q) + k + q);
			coeffA2 = ((pow(k, 2) * q) - k + q) / ((pow(k, 2) * q) + k + q);
		}
	}
	if (filterType == filterTypeBandPass) {
		coeffB0 = k / ((pow(k, 2) * q) + k + q);
		coeffB1 = 0.f;
		coeffB2 = coeffB0 * -1;
		coeffA1 = (2 * q * (pow(k, 2) - 1)) / ((pow(k, 2) * q) + k + q);
		coeffA2 = ((pow(k, 2) * q) - k + q) / ((pow(k, 2) * q) + k + q);
	}
	if (filterType == filterTypeAllPass) {
		if (filterOrder == filterOrder1) {
			coeffB0 = (k - 1) / (k + 1);
			coeffB1 = 1;
			coeffB2 = 0;
			coeffA1 = coeffB0;
			coeffA2 = 0;
		}
		else {
			coeffB0 = ((pow(k, 2) * q) - k + q) / ((pow(k, 2) * q) + k + q);
			coeffB1 = (2 * q * (pow(k, 2) - 1)) / ((pow(k, 2) * q) + k + q);
			coeffB2 = 1;
			coeffA1 = coeffB1;
			coeffA2 = coeffB0;
		}
	}
}


void BiquadFilter::flushBuffer() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 5; k++) {
				buffer[i][j][k] = 0.f;
			}
		}
	}
}


//==============================================================================


float PeakDetector::calcEnvelope(float sample, float timeConstant, int sr) {
	float x = sample;
	float attack = .1f;
	float release = timeConstant;
	float dt = 1.f / sr;
	float tc = log10(0.01f);

	// Calculate rect, AT and RT values
	float rect = fabs(x);
	float at = 1 - exp((-2.2f * dt) / (attack / 1000));
	float rt = 1 - exp((-2.2f * dt) / (release / 1000));
	
	// Writing the output value before calculation starts imitates the one sample delay
	float y = vc;
	
	// Calculate the envelope value for attack and release case
	if (rect > vc) {
		vc = (1 - at) * vc + at * rect;
	}
	else {
		vc = (1 - rt) * vc;
	}

	return y;
}

void PeakDetector::flushVC() {
	vc = 0.f;
}


//==============================================================================


float Compressor::calcGain(float sample, float threshold, float ratio, float release, int sr) {
	// Set slope variable
	float cs = 1.f - (1.f / ratio);

	// -limiting?
	// -soft knee with lagrange?

	// Envelope detector
	float env = peakDetector->calcEnvelope(sample, release, sr);
	
	// Convert Envelope to logarithmic value
	env = 20 * log(env);

	// Compute gain; Threshold and detection values in dB
	float y = cs * (threshold - env);

	// Clamp; this allows ratios of 1:1 to still operate
	y = fminf(0.f, y);

	// Convert to linear values
	return pow(10.f, y / 20.f);
}

void Compressor::flushDetector() {
	peakDetector->flushVC();
}


//==============================================================================


Ramper::Ramper() : targetValue(0.0f),
				   stepDelta(0.0f),
				   stepAmount(-1)
				   {};


// Sets the step amount that the ramper will use. You can overwrite this value by supplying a step number in setTarget
void Ramper::setStepAmount(int newStepAmount) { 
	stepAmount = newStepAmount; 
}

// Sets the new target and recalculates the step size using either the supplied step number or the step amount previously set by setStepAmount()
void Ramper::setTarget(float currentValue, float newTarget, int numberOfSteps = -1) {
	if (numberOfSteps != -1) stepDelta = (newTarget - currentValue) / numberOfSteps;
	else if (stepAmount != -1) stepDelta = (newTarget - currentValue) / stepAmount;
	else jassertfalse; // Either the step amount should be set, or a new step amount should be supplied
	targetValue = newTarget;
}

// Sets the ramper value and the target to the new value and stops ramping
void Ramper::setValue(float newValue) {
	targetValue = newValue;
	stepDelta = 0.0f;
}

// Ramps the supplied value and returns true if the targetValue is reached
bool Ramper::ramp(float &valueToChange) {
	valueToChange += stepDelta;
	return abs(targetValue - valueToChange) > 0.001;
}