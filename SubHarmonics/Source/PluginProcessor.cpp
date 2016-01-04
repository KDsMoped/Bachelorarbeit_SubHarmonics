/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


class FloatParameter : public AudioProcessorParameter {
public:

	FloatParameter(float defaultValue, int numSteps, const String& name)
		: defaultValue(defaultValue),
		value(defaultValue),
		numSteps(numSteps),
		name(name) {
	}

	float getValue() const override {
		return value;
	}

	void setValue(float newValue) override {
		value = newValue;
	}

	float getDefaultValue() const override {
		return defaultValue;
	}

	int getNumSteps() const override {
		if (numSteps > 1) {
			return numSteps;
		}
		else {
			return AudioProcessor::getDefaultNumParameterSteps();
		}
	}

	String getName(int /* maximumStringLength */) const override {
		return name;
	}

	String getLabel() const override {
		return String();
	}

	float getValueForText(const String& text) const override {
		return text.getFloatValue();
	}

private:
	float defaultValue, value;
	int numSteps;
	String name;
};

const float defaultMasterBypass = 0.f;
const float defaultSoloSub = 0.f;
const float defaultInputGain = 0.5f;
const float defaultOutputGain = 0.5f;
const float defaultSubPreGain = 0.5f;
const float defaultbpFreq = 1000;
const float defaultbpQ = 0.707f;
const float defaultHyst = 0.025f;
const float defaultColour = 400.f;

//==============================================================================
PrototypeAudioProcessor::PrototypeAudioProcessor() : biquadPreSubHPF(new BiquadFilter(filterTypeHighPass, filterOrder6)),
													 biquadPreSubLPF(new BiquadFilter(filterTypeLowPass, filterOrder6)),
													 biquadPostSubLPF(new BiquadFilter(filterTypeLowPass, filterOrder6)),
													 biquadPostSubHPF(new BiquadFilter(filterTypeHighPass, filterOrder6)),
													 biquadPreSubBPF(new BiquadFilter(filterTypeBandPass, filterOrder8))
													 {
	// Set up our parameters. The base class will delete them for us.
	addParameter(masterBypass = new FloatParameter(defaultMasterBypass, 2, "Master Bypass"));
	addParameter(soloSub = new FloatParameter(defaultSoloSub, 2, "Solo Sub"));
	addParameter(inputGain = new FloatParameter(defaultInputGain, 0, "Input Gain"));
	addParameter(outputGain = new FloatParameter(defaultOutputGain, 0, "Output Gain"));
	addParameter(subPreGain = new FloatParameter(defaultSubPreGain, 0, "Sub Pre Gain"));
	addParameter(bpFreq = new FloatParameter(defaultbpFreq, 0, "BP Frequency"));
	addParameter(bpQ = new FloatParameter(defaultbpQ, 0, "BP Q"));
	addParameter(hyst = new FloatParameter(defaultHyst, 0, "Hysteresis"));
	addParameter(colour = new FloatParameter(defaultColour, 0, "Colour"));
}

PrototypeAudioProcessor::~PrototypeAudioProcessor() {
}

//==============================================================================
const String PrototypeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

/*
int PrototypeAudioProcessor::getNumParameters()
{
    return 0;
}

float PrototypeAudioProcessor::getParameter (int index)
{
    return 0.0f;
}

void PrototypeAudioProcessor::setParameter (int index, float newValue)
{
}

const String PrototypeAudioProcessor::getParameterName (int index)
{
    return String();
}

const String PrototypeAudioProcessor::getParameterText (int index)
{
    return String();
}
*/

const String PrototypeAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String PrototypeAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool PrototypeAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool PrototypeAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool PrototypeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PrototypeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PrototypeAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double PrototypeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PrototypeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PrototypeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PrototypeAudioProcessor::setCurrentProgram (int index)
{
}

const String PrototypeAudioProcessor::getProgramName (int index)
{
    return String();
}

void PrototypeAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PrototypeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

	// Reset envelope detector capacitor voltage
	vc = 0.f;
	
	triggerChangeCount = 0;
	sign = 1;
	signumGain = 1.f;
	schmittTriggerStatus = 0;
	yk1 = 0.f;
	
	ramper = new Ramper();

	// Clearing buffers
	biquadPostSubHPF->flushBuffer();
	biquadPostSubLPF->flushBuffer();
	biquadPreSubHPF->flushBuffer();
	biquadPreSubLPF->flushBuffer();
	biquadPreSubBPF->flushBuffer();
}

void PrototypeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	delete ramper;
}

void PrototypeAudioProcessor::reset()
{
	// Use this method as the place to clear any delay lines, buffers, etc, as it
	// means there's been a break in the audio's continuity.
	
	// Reset envelope detector capacitor voltage
	vc = 0.f;

	triggerChangeCount = 0;
	sign = 1;
	schmittTriggerStatus = 0;
	yk1 = 0.f;
	
	// Clearing buffers

}

void PrototypeAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	const int numSamples = buffer.getNumSamples();

    // Clearing obsolete output channels
	for(int i = getNumInputChannels(); i < getNumOutputChannels(); i++) {
		buffer.clear(i, 0, numSamples);
	}


	float monoData[4096];
	for (int i = 0; i < getBlockSize(); i++) {
		monoData[i] = 0;
	}


	// Debug Signal Buffer
	float debugData[4096][2];
	for (int i = 0; i < getBlockSize(); i++) {
		for (int j = 0; j < 2; j++) {
			monoData[i] = 0;
		}
	}

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
	
	for (int ch = 0; ch < getNumInputChannels(); ch++) {
		// Retrieve pointers to modify each buffers channel data
		float* channelData = buffer.getWritePointer(ch);
		
		for (int i = 0; i < getBlockSize(); i++) {
			// Check if bypassed
			if (masterBypass->getValue() == 0) {
				debugData[i][0] = channelData[i];
				// Apply Input Gain
				channelData[i] *= (inputGain->getValue() * 2);
				// Mono Sum
				monoData[i] += (channelData[i] / 2);
			}
		}
	}
				

	for (int i = 0; i < getBlockSize(); i++) {
		// Check if bypassed
		if (masterBypass->getValue() == 0) {
			// Store current sample value in buffers for the various signal paths
			float effectBufferedSample = monoData[i];

			// Applay Sub Pre Gain
			effectBufferedSample *= (subPreGain->getValue() * 2);

			// TODO: Signal Conditioning
			/*
			// Pre Sub HPF
			biquadPreSubHPF->setFilterCoeffs(getSampleRate(), hpfFreq->getValue(), 0.71f);
			biquadPreSubHPF->processFilter(&effectBufferedSample, 0);

			// Pre Sub LPF
			biquadPreSubLPF->setFilterCoeffs(getSampleRate(), lpfFreq->getValue(), 0.71f);
			biquadPreSubLPF->processFilter(&effectBufferedSample, 0);
			*/
			// Pre Sub BPF
			biquadPreSubBPF->setFilterCoeffs(getSampleRate(), bpFreq->getValue(), bpQ->getValue());
			biquadPreSubBPF->processFilter(&effectBufferedSample, 0);
			
			// Forking effect signal paths
			float rectifierBufferedSample = effectBufferedSample;
			float triggerBufferedSample = effectBufferedSample;


			//Envelope Detector
			double dt = 1. / getSampleRate();
			float rc = 2.e-3; // X ms release time
			double coeff = rc / (rc + dt);

			rectifierBufferedSample = vc;
			double y = effectBufferedSample;
			double rect = fabs(y); // Rectifier(diodes)
			vc = (rect > vc ? rect : coeff*vc);

			if (rectifierBufferedSample < 0.f) {
				rectifierBufferedSample = 0.f;
			}

			// Summing Unit
			effectBufferedSample = (effectBufferedSample + rectifierBufferedSample) / 2;

			// Square Root Extractor
			if (effectBufferedSample < 0.f) {
				effectBufferedSample = 0.f;
			}
			effectBufferedSample = sqrtf(effectBufferedSample);

			/*
			// TODO: Signal Conditioning
			// Pre Sub HPF
			biquadPreSubHPF->setFilterCoeffs(getSampleRate(), hpfFreq->getValue(), 0.71);
			biquadPreSubHPF->processFilter(&triggerBufferedSample, 0);

			// Pre Sub LPF
			biquadPreSubLPF->setFilterCoeffs(getSampleRate(), lpfFreq->getValue(), 0.71);
			biquadPreSubLPF->processFilter(&triggerBufferedSample, 0);
			*/
			

			// TODO: first order allpass
			// ...


			// Trigger Circuit
			// Schmitt-Trigger
			float posHyst = hyst->getValue();
			float negHyst = posHyst * -1;

			if (triggerBufferedSample > posHyst) {
				if (schmittTriggerStatus == 0) {
					schmittTriggerStatus = 1;
					triggerChangeCount++;
				}
			}
			if (triggerBufferedSample < negHyst) {
				if (schmittTriggerStatus == 1) {
					schmittTriggerStatus = 0;
					triggerChangeCount++;
				}
			}

			// Counter
			if (triggerChangeCount == 2) {
				sign *= -1;
				ramper->setTarget(signumGain, sign, 10);
				triggerChangeCount = 0;
			}
			
			// Variable Amplifier
			ramper->ramp(signumGain);
			if (signumGain > 1.f) { signumGain = 1.f; }
			if (signumGain < -1.f) { signumGain = -1.f; }
			effectBufferedSample *= signumGain;
			

			// Post Filter
			// Calculate static filter coefficients
			biquadPostSubLPF->setFilterCoeffs(getSampleRate(), colour->getValue(), 0.71f);
			biquadPostSubHPF->setFilterCoeffs(getSampleRate(), 20.f, 0.71f);

			biquadPostSubLPF->processFilter(&effectBufferedSample, 0);
			//biquadPostSubHPF->processFilter(&effectBufferedSample, 0);
			
			//monoData[i] = triggerBufferedSample;
			monoData[i] = effectBufferedSample;
			debugData[i][1] = monoData[i];
		}
	}

	for (int ch = 0; ch < getNumInputChannels(); ch++) {
		// Retrieve pointers to modify each buffers channel data
		float* channelData = buffer.getWritePointer(ch);

		for (int i = 0; i < getBlockSize(); i++) {
			// Check if bypassed
			if (masterBypass->getValue() == 0) {
				
				if (soloSub->getValue() == 1) {
					channelData[i] = monoData[i];
				}
				else {
					 // Mixing Amplifier
					channelData[i] = (channelData[i] + monoData[i]) / (sqrtf(2.f));
				}

				
				//channelData[i] = debugData[i][ch];
				
				// Apply Output Gain
				channelData[i] *= outputGain->getValue();
			}
			else {

			}
		}
	}
}

//==============================================================================
bool PrototypeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PrototypeAudioProcessor::createEditor()
{
    return new PrototypeAudioProcessorEditor (*this);
}

//==============================================================================
void PrototypeAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	
	// Create an outer XML element..
	XmlElement xml("MYPLUGINSETTINGS");

	// add some attributes to it..
	xml.setAttribute("master_bypass", masterBypass->getValue());
	xml.setAttribute("solo_sub", soloSub->getValue());
	xml.setAttribute("input_gain", inputGain->getValue());
	xml.setAttribute("output_gain", outputGain->getValue());
	xml.setAttribute("sub_pre_gain", subPreGain->getValue());
	xml.setAttribute("bp_frequency", bpFreq->getValue());
	xml.setAttribute("bp_q", bpQ->getValue());
	xml.setAttribute("hysteresis", hyst->getValue());
	xml.setAttribute("colour", colour->getValue());

	// then use this helper function to stuff it into the binary blob and return it..
	copyXmlToBinary(xml, destData);
}

void PrototypeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

	// This getXmlFromBinary() helper function retrieves our XML from the binary blob..
	ScopedPointer<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState != nullptr)
	{
		// make sure that it's actually our type of XML object..
		if (xmlState->hasTagName("MYPLUGINSETTINGS"))
		{
			// ok, now pull out our parameters..
			masterBypass->setValue((float)xmlState->getDoubleAttribute("master_bypass", masterBypass->getValue()));
			soloSub->setValue((float)xmlState->getDoubleAttribute("solo_sub", soloSub->getValue()));
			inputGain->setValue((float)xmlState->getDoubleAttribute("input_gain", inputGain->getValue()));
			outputGain->setValue((float)xmlState->getDoubleAttribute("output_gain", outputGain->getValue()));
			subPreGain->setValue((float)xmlState->getDoubleAttribute("sub_pre_gain", subPreGain->getValue()));
			bpFreq->setValue((float)xmlState->getDoubleAttribute("bp_frequency", bpFreq->getValue()));
			bpQ->setValue((float)xmlState->getDoubleAttribute("bp_q", bpQ->getValue()));
			hyst->setValue((float)xmlState->getDoubleAttribute("hysteresis", hyst->getValue()));
			colour->setValue((float)xmlState->getDoubleAttribute("colour", colour->getValue()));
		}
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PrototypeAudioProcessor();
}
