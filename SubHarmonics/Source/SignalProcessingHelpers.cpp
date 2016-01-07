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
BiquadFilter::~BiquadFilter() { delete buffer; }


void BiquadFilter::processFilter(float *leSample, int leChannel) {
	float x = *leSample;
	float w1 = buffer[1][leChannel][0];
	float w2 = buffer[2][leChannel][0];
	
	float w = x - (coeffA1 * w1) - (coeffA2 * w2);
	float y = (coeffB0 * w) + (coeffB1 * w1) + (coeffB2 * w2);

	buffer[2][leChannel][0] = w1;
	buffer[1][leChannel][0] = w;

	if (filterOrder >= 4) {
		x = y;
		w1 = buffer[1][leChannel][1];
		w2 = buffer[2][leChannel][1];

		w = x - (coeffA1 * w1) - (coeffA2 * w2);
		y = (coeffB0 * w) + (coeffB1 * w1) + (coeffB2 * w2);
	
		buffer[2][leChannel][1] = w1;
		buffer[1][leChannel][1] = w;
	}
	if (filterOrder >= 6) {
		x = y;
		w1 = buffer[1][leChannel][2];
		w2 = buffer[2][leChannel][2];

		w = x - (coeffA1 * w1) - (coeffA2 * w2);
		y = (coeffB0 * w) + (coeffB1 * w1) + (coeffB2 * w2);

		buffer[2][leChannel][2] = w1;
		buffer[1][leChannel][2] = w;
	}
	if (filterOrder >= 8) {
		x = y;
		w1 = buffer[1][leChannel][3];
		w2 = buffer[2][leChannel][3];

		w = x - (coeffA1 * w1) - (coeffA2 * w2);
		y = (coeffB0 * w) + (coeffB1 * w1) + (coeffB2 * w2);

		buffer[2][leChannel][3] = w1;
		buffer[1][leChannel][3] = w;
	}
	if (filterOrder >= 10) {
		x = y;
		w1 = buffer[1][leChannel][4];
		w2 = buffer[2][leChannel][4];

		w = x - (coeffA1 * w1) - (coeffA2 * w2);
		y = (coeffB0 * w) + (coeffB1 * w1) + (coeffB2 * w2);

		buffer[2][leChannel][4] = w1;
		buffer[1][leChannel][4] = w;
	}

	*leSample = y;
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
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 5; k++) {
				buffer[i][j][k] = 0.f;
			}
		}
	}
}


//==============================================================================


Ramper::Ramper() : targetValue(0.0f),
				   stepDelta(0.0f),
				   stepAmount(-1)
				   {};

void Ramper::setStepAmount(int newStepAmount) { 
	stepAmount = newStepAmount; 
}