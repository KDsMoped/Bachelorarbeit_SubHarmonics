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

const float defaultMasterBypass = 0;
const float defaultInputGain = 0.5f;
const float defaultOutputGain = 0.5f;

//==============================================================================
PrototypeAudioProcessor::PrototypeAudioProcessor() {
	// Set up our parameters. The base class will delete them for us.
	addParameter(masterBypass = new FloatParameter(defaultMasterBypass, 2, "Master Bypass"));
	addParameter(inputGain = new FloatParameter(defaultInputGain, 0, "Input Gain"));
	addParameter(outputGain = new FloatParameter(defaultOutputGain, 0, "Output Gain"));
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

	// Clearing buffers
}

void PrototypeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void PrototypeAudioProcessor::reset()
{
	// Use this method as the place to clear any delay lines, buffers, etc, as it
	// means there's been a break in the audio's continuity.

	// Clearing buffers
}

void PrototypeAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	const int numSamples = buffer.getNumSamples();

    // Clearing obsolete output channels
	for(int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
		buffer.clear(i, 0, numSamples);
	}

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for(int ch=0; ch < getNumInputChannels(); ++ch) {
        // Retrieve pointers to modify each buffers channel data
		float* channelData = buffer.getWritePointer(ch);

		for (int i = 0; i < getBlockSize(); i++) {
			// Check if bypassed
			if (masterBypass->getValue() == 0) {
				// Apply Input Gain
				channelData[i] *= inputGain->getValue();

				// Store current sample value in buffers for the various signal paths
				float effectBufferedSample = channelData[i];
				float rectifierBufferedSample = channelData[i];
				float triggerBufferedSample = channelData[i];
				float drySignalBufferedSample = channelData[i];

				// TODO: Full Wave Rectifier
				if (rectifierBufferedSample < 0) {
					rectifierBufferedSample *= -1;
				} 
					// TODO: Add Smoothing Filter


				// Summing Unit
				effectBufferedSample = (effectBufferedSample + rectifierBufferedSample) / 2;

				// TODO: Square Root Extractor

				// TODO: Signal Conditioner/Trigger Circuit

				// TODO: Counter

				// TODO: Variable Amplifier

				// TODO: Post Filter

				// TODO: Mixing Amplifier
				channelData[i] = (effectBufferedSample + drySignalBufferedSample) / 2;

				// Apply Output Gain
				channelData[i] *= outputGain->getValue();
			}
			else {
				// TODO: Apply Latency...
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
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PrototypeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PrototypeAudioProcessor();
}
