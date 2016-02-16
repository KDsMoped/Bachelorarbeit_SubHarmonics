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
const float defaultSwitchFilter = 0.f;
const float defaultHarmCompens = 0.f;
const float defaultMuteSub = 0.f;

const float defaultInputGain = 0.f;
const float defaultPreSubGain = 18.f;
const float defaultBpFreq = 1000.f;
const float defaultBpQ = 0.707f;
const float defaultLpfFreq = 1000.f;
const float defaultHpfFreq = 20.f;
const float defaultDecay = 30.f;
const float defaultHyst = -60.f;
const float defaultColour = 550.f;
const float defaultDirectGain = 0.f;
const float defaultPostSubGain = 0.f;
const float defaultOutputGain = 0.f;

//==============================================================================
SubHarmonicsAudioProcessor::SubHarmonicsAudioProcessor() {
	// Create biquad filter objects
	biquadPreSubBPF = new BiquadFilter(filterTypeBandPass, filterOrder8);
	biquadPreSubHPF = new BiquadFilter(filterTypeHighPass, filterOrder8);
	biquadPreSubLPF = new BiquadFilter(filterTypeLowPass, filterOrder8);
	biquadPostSubLPF = new BiquadFilter(filterTypeLowPass, filterOrder6);
	biquadStaticPostSubLPF = new BiquadFilter(filterTypeLowPass, filterOrder1);
	biquadPostSubHPF = new BiquadFilter(filterTypeHighPass, filterOrder2);
	biquadTriggerAPF = new BiquadFilter(filterTypeAllPass, filterOrder1);
	biquadPreTriggerLPF = new BiquadFilter(filterTypeLowPass, filterOrder4);
	biquadCompAPF = new BiquadFilter(filterTypeAllPass, filterOrder2);
	
	// Create peak detector object
	peakDetector = new PeakDetector();

	// Create compressor objekt
	lvlCompensationCompressor = new Compressor();

	// Set up our parameters. The base class will delete them for us.
	addParameter(paramMasterBypass = new FloatParameter(defaultMasterBypass, 2, "Master Bypass"));
	addParameter(paramSoloSub = new FloatParameter(defaultSoloSub, 2, "Solo Sub"));
	addParameter(paramSwitchFilter = new FloatParameter(defaultSwitchFilter, 2, "Switch Filter"));
	addParameter(paramHarmonicCompens = new FloatParameter(defaultHarmCompens, 2, "Harmonic Compensation"));
	addParameter(paramMuteSub = new FloatParameter(defaultMuteSub, 2, "Mute Sub"));

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

SubHarmonicsAudioProcessor::~SubHarmonicsAudioProcessor() {
}

//==============================================================================
const String SubHarmonicsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

const String SubHarmonicsAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String SubHarmonicsAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool SubHarmonicsAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool SubHarmonicsAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool SubHarmonicsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SubHarmonicsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SubHarmonicsAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double SubHarmonicsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SubHarmonicsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SubHarmonicsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SubHarmonicsAudioProcessor::setCurrentProgram (int index)
{
}

const String SubHarmonicsAudioProcessor::getProgramName (int index)
{
    return String();
}

void SubHarmonicsAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SubHarmonicsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
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
	biquadStaticPostSubLPF->flushBuffer();
	biquadPreSubHPF->flushBuffer();
	biquadPreSubLPF->flushBuffer();
	biquadPreSubBPF->flushBuffer();
	biquadTriggerAPF->flushBuffer();
	biquadPreTriggerLPF->flushBuffer();
	biquadCompAPF->flushBuffer();

	peakDetector->flushVC();

	lvlCompensationCompressor->flushDetector();
	
}

void SubHarmonicsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	delete ramper;
}

void SubHarmonicsAudioProcessor::reset()
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

void SubHarmonicsAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	const int numSamples = buffer.getNumSamples();
	const int numChannels = getMainBusNumInputChannels();
	const int sampleRate = getSampleRate();

    // Clearing obsolete output channels
	for (int i = numChannels; i < getNumOutputChannels(); i++) {
		buffer.clear(i, 0, numSamples);
	}

	// Mono Sum Buffer
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

	// ----- Preparing Effect Processing -----
	for (int ch = 0; ch < numChannels; ch++) {
		// Retrieve pointers to modify each buffers channel data
		float* channelData = buffer.getWritePointer(ch);
		
		for (int i = 0; i < numSamples; i++) {
			// Check if bypassed
			if (paramMasterBypass->getValue() == 0) {
				// Apply Input Gain
				channelData[i] *= convertDBtoFloat(paramInputGain->getValue());
				// Building Mono Sum
				monoData[i] += (channelData[i] / 2);
			}
		}
	}

	// ----- Effect Processing on Mono Sum -----
	for (int i = 0; i < numSamples; i++) {
		// Check if bypassed
		if (paramMasterBypass->getValue() == 0) {
			float effectSample = monoData[i];
			
			// ----- Pre Processing -----
			// Applay Pre Sub Gain
			effectSample *= convertDBtoFloat(paramPreSubGain->getValue());
			
			// Signal Conditioning - extracting base frequency
			if (paramSwitchFilter->getValue() == 0) {
				// Pre Sub BPF
				biquadPreSubBPF->setFilterCoeffs(sampleRate, paramBpFreq->getValue(), paramBpQ->getValue());
				biquadPreSubBPF->processFilter(&effectSample, 0);
				// Pre Calculation for Phase Correction
				biquadTriggerAPF->setFilterCoeffs(sampleRate, paramBpFreq->getValue(), 0);
			}
			else {
				// Pre Sub LPF
				biquadPreSubLPF->setFilterCoeffs(getSampleRate(), paramLpfFreq->getValue(), 0.707f);
				biquadPreSubLPF->processFilter(&effectSample, 0);
				// Pre Sub HPF
				biquadPreSubHPF->setFilterCoeffs(getSampleRate(), paramHpfFreq->getValue(), 0.707f);
				biquadPreSubHPF->processFilter(&effectSample, 0);
				// Pre Calculation for Phase Correction
				float midFreq = (paramLpfFreq->getValue() + paramHpfFreq->getValue()) / 2;
				biquadTriggerAPF->setFilterCoeffs(sampleRate, midFreq, 0);
			}

			// Saving effect sample for trigger path
			float triggerSample = effectSample;


			// ----- Signal Synthesis Path -----
			// Peak Detector to generate the Offset "1"
			float envelopeSample = peakDetector->calcEnvelope(effectSample, paramDecay->getValue(), sampleRate);

			// Summing Unit
			effectSample = (effectSample + envelopeSample);// / 2;

			// Square Root Extractor
			effectSample = fmax(effectSample, 0.f);
			effectSample = sqrtf(effectSample);

			// Harmonic Compensation Phase Correction
			if (paramHarmonicCompens->getValue() != 0.f) {
				biquadCompAPF->setFilterCoeffs(sampleRate, 40, 0.707f);
				biquadCompAPF->processFilter(&effectSample, 0);
			}
			
			debugData[i][0] = monoData[i];
			debugData[i][1] = effectSample;

			// ----- Trigger Path -----
			// First order allpass for ~90� phase shift
			//biquadTriggerAPF->processFilter(&triggerSample, 0);

			// Harmonic Compensation LPF and Compressor
			if (paramHarmonicCompens->getValue() != 0.f) {
				biquadPreTriggerLPF->setFilterCoeffs(sampleRate, 40, 0.707f);
				biquadPreTriggerLPF->processFilter(&triggerSample, 0);

				// Compressor
				float compGain = lvlCompensationCompressor->calcGain(triggerSample, -48, 10, 200, sampleRate);
				triggerSample *= compGain;

				// Make up gain
				triggerSample *= 30;
			}

			// Schmitt-Trigger
			float posHyst = convertDBtoFloat(paramHyst->getValue());
			float negHyst = posHyst * -1;

			if (triggerSample > posHyst) {
				if (schmittTriggerStatus == 0) {
					schmittTriggerStatus = 1;
					triggerChangeCount++;
				}
			}
			if (triggerSample < negHyst) {
				if (schmittTriggerStatus == 1) {
					schmittTriggerStatus = 0;
					triggerChangeCount++;
				}
			}

			// Variable Amplifier
			if (triggerChangeCount == 2) {
				sign *= -1;
				ramper->setTarget(signumGain, sign, 8);
				triggerChangeCount = 0;
			}

			ramper->ramp(signumGain);
			if (signumGain > 1.f) { signumGain = 1.f; }
			if (signumGain < -1.f) { signumGain = -1.f; }
			effectSample *= sign;// signumGain;
		

			// ----- Post Processing -----
			// Multiply with envelope
			effectSample *= envelopeSample;
			
			// Post Processing
			// Static Post
			biquadStaticPostSubLPF->setFilterCoeffs(sampleRate, 40.f, 0.707f);
			biquadStaticPostSubLPF->processFilter(&effectSample, 0);

			// Post Sub LPF
			biquadPostSubLPF->setFilterCoeffs(sampleRate, paramColour->getValue(), 0.707f);
			biquadPostSubLPF->processFilter(&effectSample, 0);

			// Post Sub HPF
			biquadPostSubHPF->setFilterCoeffs(sampleRate, 19.f, 0.707f);
			//biquadPostSubHPF->processFilter(&effectSample, 0);
			
			monoData[i] = effectSample;
		}
	}

	// ----- Calculating Output -----
	for (int ch = 0; ch < numChannels; ch++) {
		// Retrieve pointers to modify each buffers channel data
		float* channelData = buffer.getWritePointer(ch);

		for (int i = 0; i < numSamples; i++) {
			// Check if bypassed
			if (paramMasterBypass->getValue() == 0) {
				
				float directOut = channelData[i];
				float subOut = monoData[i];

				if (paramSoloSub->getValue() == 1) {
					directOut = 0.f;
				}
				if (paramMuteSub->getValue() == 1) {
					subOut = 0.f;
				}

				// Mixing Amplifier 
				channelData[i] = (directOut * convertDBtoFloat(paramDirectGain->getValue())) + (subOut * convertDBtoFloat(paramPostSubGain->getValue()));

				channelData[i] = debugData[i][ch];
				
				// Apply Output Gain
				channelData[i] *= convertDBtoFloat(paramOutputGain->getValue());
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
bool SubHarmonicsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SubHarmonicsAudioProcessor::createEditor()
{
    return new SubHarmonicsAudioProcessorEditor (*this);
}

//==============================================================================
void SubHarmonicsAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	
	// Create an outer XML element..
	XmlElement xml("PLUGINSETTINGS");

	// add some attributes to it..
	xml.setAttribute("master_bypass", paramMasterBypass->getValue());
	xml.setAttribute("solo_sub", paramSoloSub->getValue());
	xml.setAttribute("switch_filter", paramSwitchFilter->getValue());
	xml.setAttribute("harmonic_compens", paramHarmonicCompens->getValue());
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

void SubHarmonicsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
			paramHarmonicCompens->setValue((float)xmlState->getDoubleAttribute("harmonic_compens", paramHarmonicCompens->getValue()));
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
    return new SubHarmonicsAudioProcessor();
}
