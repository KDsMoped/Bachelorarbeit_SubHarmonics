/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class PrototypeAudioProcessorEditor : public AudioProcessorEditor,
									  private Slider::Listener,
									  private Button::Listener,
									  private Timer
{
public:
    PrototypeAudioProcessorEditor (PrototypeAudioProcessor&);
    ~PrototypeAudioProcessorEditor();

    //==============================================================================
	void timerCallback() override;
	void paint(Graphics&) override;
	void resized() override;
	void sliderValueChanged(Slider *slider) override;
	void sliderDragStarted(Slider*) override;
	void sliderDragEnded(Slider*) override;
	void buttonClicked(Button* button) override;

private:
	/*
	// This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PrototypeAudioProcessor& processor;
	*/
	ToggleButton masterBypassButton;
	ToggleButton soloSubButton;

	Slider inputGainSlider;
	Label inputGainLabel;

	Slider outputGainSlider;
	Label outputGainLabel;

	Slider subPreGainSlider;
	Label subPreGainLabel;

	Slider hpfFreqSlider;
	Label hpfFreqLabel;

	Slider lpfFreqSlider;
	Label lpfFreqLabel;

	//==============================================================================
	PrototypeAudioProcessor& getProcessor() const
	{
		return static_cast<PrototypeAudioProcessor&> (processor);
	}

	AudioProcessorParameter* getParameterFromSlider(const Slider*) const;
	AudioProcessorParameter* getParameterFromButton(const Button*) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PrototypeAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
