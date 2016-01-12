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
const float defailtSwitchFilter = 0.f;

const float defaultInputGain = 0.5f;
const float defaultPreSubGain = 0.5f;
const float defaultBpFreq = 1000.f;
const float defaultBpQ = 0.707f;
const float defaultLpfFreq = 900.f;
const float defaultHpfFreq = 1000.f;
const float defaultDecay = 5.f;
const float defaultHyst = 0.025f;
const float defaultColour = 550.f;
const float defaultDirectGain = 0.5f;
const float defaultPostSubGain = 0.5f;
const float defaultOutputGain = 0.5f;

//==============================================================================
PrototypeAudioProcessor::PrototypeAudioProcessor() : biquadPreSubHPF(new BiquadFilter(filterTypeHighPass, filterOrder8)),
													 biquadPreSubLPF(new BiquadFilter(filterTypeLowPass, filterOrder8)),
													 biquadPostSubLPF(new BiquadFilter(filterTypeLowPass, filterOrder6)),
													 biquadPostSubHPF(new BiquadFilter(filterTypeHighPass, filterOrder2)),
													 biquadPreSubBPF(new BiquadFilter(filterTypeBandPass, filterOrder8)),
													 biquadTriggerAPF(new BiquadFilter(filterTypeAllPass, filterOrder1)),
													 biquadPreTriggerLPF(new BiquadFilter(filterTypeLowPass, filterOrder4)),
													 biquadCompAPF(new BiquadFilter(filterTypeAllPass, filterOrder2)),
													 peakDetector(new PeakDetector()),
													 lvlCompensationCompressor(new Compressor())
													 {
	// Set up our parameters. The base class will delete them for us.
	addParameter(paramMasterBypass = new FloatParameter(defaultMasterBypass, 2, "Master Bypass"));
	addParameter(paramSoloSub = new FloatParameter(defaultSoloSub, 2, "Solo Sub"));
	addParameter(paramSwitchFilter = new FloatParameter(defailtSwitchFilter, 2, "Switch Filter"));

	addParameter(paramInputGain = new FloatParameter(defaultInputGain, 0, "Input Gain"));
	addParameter(paramPreSubGain = new FloatParameter(defaultPreSubGain, 0, "Pre Sub Gain"));
	addParameter(paramBpFreq = new FloatParameter(defaultBpFreq, 0, "BP Frequency"));
	addParameter(paramBpQ = new FloatParameter(defaultBpQ, 0, "BP Q"));
	addParameter(paramLpfFreq = new FloatParameter(defaultLpfFreq, 0, "LPF Frequency"));
	addParameter(paramHpfFreq = new FloatParameter(defaultHpfFreq, 0, "HPF Frequency"));
	addParameter(paramDecay = new FloatParameter(defaultDecay, 0, "Decay"));
	addParameter(paramHyst = new FloatParameter(defaultHyst, 0, "Hysteresis"));
	addParameter(paramColour = new FloatParameter(defaultColour, 0, "Colour"));
	addParameter(paramDirectGain = new FloatParameter(defaultDirectGain, 0, "Direct Gain"));
	addParameter(paramPostSubGain = new FloatParameter(defaultPostSubGain, 0, "Post Sub Gain"));
	addParameter(paramOutputGain = new FloatParameter(defaultOutputGain, 0, "Output Gain"));
}

PrototypeAudioProcessor::~PrototypeAudioProcessor() {
}

//==============================================================================
const String PrototypeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

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
	biquadTriggerAPF->flushBuffer();
	biquadPreTriggerLPF->flushBuffer();
	biquadCompAPF->flushBuffer();

	peakDetector->flushVC();

	lvlCompensationCompressor->flushDetector();
	
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
	peakDetector->flushVC();

	lvlCompensationCompressor->flushDetector();

	triggerChangeCount = 0;
	sign = 1;
	schmittTriggerStatus = 0;
	yk1 = 0.f;
	
	// Clearing buffers

}

void PrototypeAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	const int numSamples = buffer.getNumSamples();
	const int numChannels = getMainBusNumInputChannels();
	const int sampleRate = getSampleRate();


    // Clearing obsolete output channels
	for (int i = numChannels; i < getNumOutputChannels(); i++) {
		buffer.clear(i, 0, numSamples);
	}


	float monoData[4096];
	for (int i = 0; i < numSamples; i++) {
		monoData[i] = 0;
	}


	// Debug Signal Buffer
	float debugData[4096][2];
	for (int i = 0; i < numSamples; i++) {
		for (int j = 0; j < 2; j++) {
			monoData[i] = 0;
		}
	}

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
	
	for (int ch = 0; ch < numChannels; ch++) {
		// Retrieve pointers to modify each buffers channel data
		float* channelData = buffer.getWritePointer(ch);
		
		for (int i = 0; i < numSamples; i++) {
			// Check if bypassed
			if (paramMasterBypass->getValue() == 0) {
				// Apply Input Gain
				channelData[i] *= (paramInputGain->getValue() * 2);
				// Mono Sum
				monoData[i] += (channelData[i] / 2);
			}
		}
	}
				

	for (int i = 0; i < numSamples; i++) {
		// Check if bypassed
		if (paramMasterBypass->getValue() == 0) {
			// Store current sample value in buffers for the various signal paths
			float effectBufferedSample = monoData[i];

			// Applay Sub Pre Gain
			effectBufferedSample *= (paramPreSubGain->getValue() * 4);

			// Signal Conditioning
			if (paramSwitchFilter->getValue() == 0) {
				// Pre Sub BPF
				biquadPreSubBPF->setFilterCoeffs(sampleRate, paramBpFreq->getValue(), paramBpQ->getValue());
				biquadPreSubBPF->processFilter(&effectBufferedSample, 0);
			}
			else {
				// Pre Sub LPF
				biquadPreSubLPF->setFilterCoeffs(getSampleRate(), paramLpfFreq->getValue(), 0.707f);
				biquadPreSubLPF->processFilter(&effectBufferedSample, 0);
				// Pre Sub HPF
				biquadPreSubHPF->setFilterCoeffs(getSampleRate(), paramHpfFreq->getValue(), 0.707f);
				biquadPreSubHPF->processFilter(&effectBufferedSample, 0);
			}

			
			// Forking effect signal paths
			float triggerBufferedSample = effectBufferedSample;


			// Peak Detector to generate the Offset "1"
			float rectifierBufferedSample = peakDetector->calcEnvelope(effectBufferedSample, paramDecay->getValue(), sampleRate);


			// Summing Unit
			effectBufferedSample = (effectBufferedSample + rectifierBufferedSample) / 2;

			// Square Root Extractor
			if (effectBufferedSample < 0.f) {
				effectBufferedSample = 0.f;
			}
			effectBufferedSample = sqrtf(effectBufferedSample);

			debugData[i][1] = effectBufferedSample;

			biquadCompAPF->setFilterCoeffs(sampleRate, 40, 0.707f);
			biquadCompAPF->processFilter(&effectBufferedSample, 0);
		
			

			// Trigger Circuit

			// Optional first order allpass
			biquadTriggerAPF->setFilterCoeffs(sampleRate, paramBpFreq->getValue(), 0);
			biquadTriggerAPF->processFilter(&triggerBufferedSample, 0);

			// 
			
			biquadPreTriggerLPF->setFilterCoeffs(sampleRate, 40, 0.707f);
			biquadPreTriggerLPF->processFilter(&triggerBufferedSample, 0);

			
			// Compressor
			debugData[i][0] = triggerBufferedSample;
			float compGain = lvlCompensationCompressor->calcGain(triggerBufferedSample, -48, 10, 200, sampleRate);
			triggerBufferedSample *= compGain;
			
			// Make up gain
			triggerBufferedSample *= 30;
			
			debugData[i][1] = triggerBufferedSample;

			// Schmitt-Trigger
			float posHyst = paramHyst->getValue();
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
			biquadPostSubLPF->setFilterCoeffs(sampleRate, paramColour->getValue(), 0.707f);
			biquadPostSubHPF->setFilterCoeffs(sampleRate, 19.f, 0.707f);

			biquadPostSubLPF->processFilter(&effectBufferedSample, 0);
			//biquadPostSubHPF->processFilter(&effectBufferedSample, 0);
			
			monoData[i] = effectBufferedSample;
		}
	}

	for (int ch = 0; ch < numChannels; ch++) {
		// Retrieve pointers to modify each buffers channel data
		float* channelData = buffer.getWritePointer(ch);

		for (int i = 0; i < numSamples; i++) {
			// Check if bypassed
			if (paramMasterBypass->getValue() == 0) {
				
				if (paramSoloSub->getValue() == 1) {
					channelData[i] = monoData[i] * (2 * paramPostSubGain->getValue());
				}
				else {
					// Mixing Amplifier 
					channelData[i] = ((channelData[i] * (2 * paramDirectGain->getValue())) + (monoData[i] * (2 * paramPostSubGain->getValue()))) / (sqrtf(2.f));
				}

				
				//channelData[i] = debugData[i][ch];
				
				// Apply Output Gain
				channelData[i] *= paramOutputGain->getValue();
			}
			else {
				/*
				if (ch == 0) {
				biquadCompAPF->setFilterCoeffs(getSampleRate(), 40, 0.717f);
				biquadCompAPF->processFilter(&channelData[i], ch);
				}
				else {
				biquadPreTriggerLPF->setFilterCoeffs(getSampleRate(), 40, 0.717f);
				biquadPreTriggerLPF->processFilter(&channelData[i], ch);
				}
				*/
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
	XmlElement xml("PLUGINSETTINGS");

	// add some attributes to it..
	xml.setAttribute("master_bypass", paramMasterBypass->getValue());
	xml.setAttribute("solo_sub", paramSoloSub->getValue());
	xml.setAttribute("switch_filter", paramSwitchFilter->getValue());
	xml.setAttribute("input_gain", paramInputGain->getValue());
	xml.setAttribute("pre_sub_gain", paramPreSubGain->getValue());
	xml.setAttribute("bp_frequency", paramBpFreq->getValue());
	xml.setAttribute("bp_q", paramBpQ->getValue());
	xml.setAttribute("lpf_frequency", paramLpfFreq->getValue());
	xml.setAttribute("hpf_frequency", paramHpfFreq->getValue());
	xml.setAttribute("decay", paramDecay->getValue());
	xml.setAttribute("hysteresis", paramHyst->getValue());
	xml.setAttribute("colour", paramColour->getValue());
	xml.setAttribute("direct_gain", paramDirectGain->getValue());
	xml.setAttribute("post_sub_gain", paramPostSubGain->getValue());
	xml.setAttribute("output_gain", paramOutputGain->getValue());

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
		if (xmlState->hasTagName("PLUGINSETTINGS"))
		{
			// ok, now pull out our parameters..
			paramMasterBypass->setValue((float)xmlState->getDoubleAttribute("master_bypass", paramMasterBypass->getValue()));
			paramSoloSub->setValue((float)xmlState->getDoubleAttribute("solo_sub", paramSoloSub->getValue()));
			paramSwitchFilter->setValue((float)xmlState->getDoubleAttribute("switch_filter", paramSwitchFilter->getValue()));
			paramInputGain->setValue((float)xmlState->getDoubleAttribute("input_gain", paramInputGain->getValue()));
			paramPreSubGain->setValue((float)xmlState->getDoubleAttribute("pre_sub_gain", paramPreSubGain->getValue()));
			paramBpFreq->setValue((float)xmlState->getDoubleAttribute("bp_frequency", paramBpFreq->getValue()));
			paramBpQ->setValue((float)xmlState->getDoubleAttribute("bp_q", paramBpQ->getValue()));
			paramLpfFreq->setValue((float)xmlState->getDoubleAttribute("lpf_frequency", paramLpfFreq->getValue()));
			paramHpfFreq->setValue((float)xmlState->getDoubleAttribute("hpf_frequency", paramHpfFreq->getValue()));
			paramDecay->setValue((float)xmlState->getDoubleAttribute("decay", paramDecay->getValue()));
			paramHyst->setValue((float)xmlState->getDoubleAttribute("hysteresis", paramHyst->getValue()));
			paramColour->setValue((float)xmlState->getDoubleAttribute("colour", paramColour->getValue()));
			paramDirectGain->setValue((float)xmlState->getDoubleAttribute("direct_gain", paramDirectGain->getValue()));
			paramPostSubGain->setValue((float)xmlState->getDoubleAttribute("post_sub_gain", paramPostSubGain->getValue()));
			paramOutputGain->setValue((float)xmlState->getDoubleAttribute("output_gain", paramOutputGain->getValue()));
		}
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PrototypeAudioProcessor();
}
