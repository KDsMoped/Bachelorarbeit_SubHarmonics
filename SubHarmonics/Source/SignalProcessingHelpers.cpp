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
		coeffB0 = q / ((pow(k, 2) * q) + k + q);
		coeffB1 = (-2 * q) / ((pow(k, 2) * q) + k + q);
		coeffB2 = coeffB0;
		coeffA1 = (2 * q * (pow(k, 2) - 1)) / ((pow(k, 2) * q) + k + q);
		coeffA2 = ((pow(k, 2) * q) - k + q) / ((pow(k, 2) * q) + k + q);
	}
	if (filterType == filterTypeLowPass) {
		coeffB0 = (pow(k, 2) * q) / ((pow(k, 2) * q) + k + q);
		coeffB1 = (2 * (pow(k, 2) * q)) / ((pow(k, 2) * q) + k + q);
		coeffB2 = coeffB0;
		coeffA1 = (2 * q * (pow(k, 2) - 1)) / ((pow(k, 2) * q) + k + q);
		coeffA2 = ((pow(k, 2) * q) - k + q) / ((pow(k, 2) * q) + k + q);
	}
	if (filterType == filterTypeBandPass) {
		coeffB0 = k / ((pow(k, 2) * q) + k + q);
		coeffB1 = 0.f;
		coeffB2 = coeffB0 * -1;
		coeffA1 = (2 * q * (pow(k, 2) - 1)) / ((pow(k, 2) * q) + k + q);
		coeffA2 = ((pow(k, 2) * q) - k + q) / ((pow(k, 2) * q) + k + q);
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


AllPassFilter::AllPassFilter(int filterOrder) : filterOrder(filterOrder) {}
AllPassFilter::~AllPassFilter() { delete buffer; }


void AllPassFilter::processFilter(float *leSample, int ch) {
	
	float x = *leSample;
	
	float w1 = buffer[1][ch];
	float w2 = buffer[2][ch];

	float w = x - (coeffA1 * w1) - (coeffA2 * w2);
	float y = (coeffB0 * w) + (coeffB1 * w1) + (coeffB2 * w2);

	buffer[2][ch] = w1;
	buffer[1][ch] = w;

	*leSample = y;
}


void AllPassFilter::setFilterCoeffs(float sr, float f, float q) {
	float k = tan((float_Pi * f) / sr);
	if (filterOrder == filterOrder1) {
		float alpha = (k - 1) / (k + 1);
		coeffB0 = alpha;
		coeffB1 = 1;
		coeffB2 = 0;
		coeffA1 = alpha;
		coeffA2 = 0;
	}
	if (filterOrder == filterOrder2) {
		float d = 1 / q;
		float j = 1 / (1 + (d*k) + pow(k, 2));
		coeffB0 = (1 - k*d + pow(k, 2)) * j;
		coeffB1 = 2 * (pow(k, 2) - 1) * j;
		coeffB2 = 1;
		coeffA1 = coeffB1;
		coeffA2 = coeffB0;
	}
}


void AllPassFilter::flushBuffer() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			buffer[i][j] = 0.f;
		}
	}
}


//==============================================================================