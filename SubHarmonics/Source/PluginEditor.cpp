/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
PrototypeAudioProcessorEditor::PrototypeAudioProcessorEditor(PrototypeAudioProcessor& owningProcessor)
	: AudioProcessorEditor(owningProcessor) {

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (850, 310);

	int defaultTextBoxHeight = 35;	// Text box height in pixels

	// Set properties for Master Bypass Button
	masterBypassButton.addListener(this);
	masterBypassButton.setButtonText("Master Bypass");

	// Set properties for Solo Sub Button
	soloSubButton.addListener(this);
	soloSubButton.setButtonText("Solo Sub");

	// Set properties for Switch Filter Button
	switchFilterButton.addListener(this);
	switchFilterButton.setButtonText("Switch Filter");

	// Set properties for the Input Gain Slider
	inputGainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	inputGainSlider.setRange(0.0, 1.0, 0.01);
	inputGainSlider.setSliderSnapsToMousePosition(false);
	inputGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, defaultTextBoxHeight);
	inputGainSlider.setTextValueSuffix(" Volume");
	inputGainSlider.addListener(this);
	// Set properties for the Input Gain Label
	inputGainLabel.setText("Input Volume", NotificationType::dontSendNotification);

	// Set properties for the Output Gain Slider
	outputGainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	outputGainSlider.setRange(0.0, 1.0, 0.01);
	outputGainSlider.setSliderSnapsToMousePosition(false);
	outputGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, defaultTextBoxHeight);
	outputGainSlider.setTextValueSuffix(" Volume");
	outputGainSlider.addListener(this);
	// Set properties for the Output Gain Label
	outputGainLabel.setText("Output Volume", NotificationType::dontSendNotification);

	// Set properties for the Pre Sub Gain Slider
	preSubGainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	preSubGainSlider.setRange(0.0, 1.0, 0.01);
	preSubGainSlider.setSliderSnapsToMousePosition(false);
	preSubGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, defaultTextBoxHeight);
	preSubGainSlider.setTextValueSuffix(" Volume");
	preSubGainSlider.addListener(this);
	// Set properties for the Pre Sub Gain Label
	preSubGainLabel.setText("Pre Sub Gain", NotificationType::dontSendNotification);

	// Set properties for the BP Frequency Slider
	bpFreqSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	bpFreqSlider.setRange(40, 1000, 1);
	bpFreqSlider.setSliderSnapsToMousePosition(false);
	bpFreqSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, defaultTextBoxHeight);
	bpFreqSlider.setTextValueSuffix(" Hz");
	bpFreqSlider.setSkewFactor(0.5);
	bpFreqSlider.addListener(this);
	// Set properties for the HPF Frequency Label
	bpFreqLabel.setText("BP Frequency", NotificationType::dontSendNotification);

	// Set properties for the BP Q Slider
	bpQSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	bpQSlider.setRange(0.2f, 1.f, 0.001f);
	bpQSlider.setSliderSnapsToMousePosition(false);
	bpQSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, defaultTextBoxHeight);
	bpQSlider.addListener(this);
	// Set properties for the LPF Frequncy Label
	bpQLabel.setText("BP Q", NotificationType::dontSendNotification);

	// Set properties for the HPF Frequency Slider
	hpfFreqSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	hpfFreqSlider.setRange(40, 1000, 1);
	hpfFreqSlider.setSliderSnapsToMousePosition(false);
	hpfFreqSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, defaultTextBoxHeight);
	hpfFreqSlider.setTextValueSuffix(" Hz");
	hpfFreqSlider.setSkewFactor(0.5);
	hpfFreqSlider.addListener(this);
	// Set properties for the HPF Frequency Label
	hpfFreqLabel.setText("HPF Frequency", NotificationType::dontSendNotification);

	// Set properties for the LPF Frequency Slider
	lpfFreqSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	lpfFreqSlider.setRange(40, 1000, 1);
	lpfFreqSlider.setSliderSnapsToMousePosition(false);
	lpfFreqSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, defaultTextBoxHeight);
	lpfFreqSlider.setTextValueSuffix(" Hz");
	lpfFreqSlider.setSkewFactor(0.5);
	lpfFreqSlider.addListener(this);
	// Set properties for the LPF Frequency Label
	lpfFreqLabel.setText("LPF Frequency", NotificationType::dontSendNotification);

	// Set properties for the Decay Slider
	decaySlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	decaySlider.setRange(.5f, 10.f, .5f);
	decaySlider.setSliderSnapsToMousePosition(false);
	decaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, defaultTextBoxHeight);
	decaySlider.setTextValueSuffix(" ms");
	decaySlider.addListener(this);
	// Set properties for the Decay Label
	decayLabel.setText("Decay", NotificationType::dontSendNotification);

	// Set properties for the Hysteresis Slider
	hystSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	hystSlider.setRange(0, .1f, .0001f);
	hystSlider.setSliderSnapsToMousePosition(false);
	hystSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, defaultTextBoxHeight);
	hystSlider.addListener(this);
	// Set properties for the Hysteresis Label
	hystLabel.setText("Hysteresis", NotificationType::dontSendNotification);

	// Set properties for the Colour Slider
	colourSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	colourSlider.setRange(100.f, 600.f, 1.f);
	colourSlider.setSliderSnapsToMousePosition(false);
	colourSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, defaultTextBoxHeight);
	colourSlider.setTextValueSuffix(" Hz");
	colourSlider.setSkewFactor(0.5);
	colourSlider.addListener(this);
	// Set properties for the Colour Label
	colourLabel.setText("Colour", NotificationType::dontSendNotification);

	// Set properties for the Direct Gain Slider
	directGainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	directGainSlider.setRange(0.0, 1.0, 0.01);
	directGainSlider.setSliderSnapsToMousePosition(false);
	directGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, defaultTextBoxHeight);
	directGainSlider.setTextValueSuffix(" Volume");
	directGainSlider.addListener(this);
	// Set properties for the Direct Gain Label
	directGainLabel.setText("Direct Gain", NotificationType::dontSendNotification);

	// Set properties for the Post Sub Gain Slider
	postSubGainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	postSubGainSlider.setRange(0.0, 1.0, 0.01);
	postSubGainSlider.setSliderSnapsToMousePosition(false);
	postSubGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, defaultTextBoxHeight);
	postSubGainSlider.setTextValueSuffix(" Volume");
	postSubGainSlider.addListener(this);
	// Set properties for the Post Sub Gain Label
	postSubGainLabel.setText("Post Sub Gain", NotificationType::dontSendNotification);

	addAndMakeVisible(&masterBypassButton);
	addAndMakeVisible(&soloSubButton);
	addAndMakeVisible(&switchFilterButton);
	addAndMakeVisible(&inputGainSlider);
	addAndMakeVisible(&inputGainLabel);
	addAndMakeVisible(&outputGainSlider);
	addAndMakeVisible(&outputGainLabel);
	addAndMakeVisible(&preSubGainSlider);
	addAndMakeVisible(&preSubGainLabel);
	addAndMakeVisible(&bpFreqSlider);
	addAndMakeVisible(&bpFreqLabel);
	addAndMakeVisible(&bpQSlider);
	addAndMakeVisible(&bpQLabel);
	addAndMakeVisible(&hpfFreqSlider);
	addAndMakeVisible(&hpfFreqLabel);
	addAndMakeVisible(&lpfFreqSlider);
	addAndMakeVisible(&lpfFreqLabel);
	addAndMakeVisible(&decaySlider);
	addAndMakeVisible(&decayLabel);
	addAndMakeVisible(&hystSlider);
	addAndMakeVisible(&hystLabel);
	addAndMakeVisible(&colourSlider);
	addAndMakeVisible(&colourLabel);
	addAndMakeVisible(&directGainSlider);
	addAndMakeVisible(&directGainLabel);
	addAndMakeVisible(&postSubGainSlider);
	addAndMakeVisible(&postSubGainLabel);
	startTimer(50);
}

PrototypeAudioProcessorEditor::~PrototypeAudioProcessorEditor() {
}

//==============================================================================
void PrototypeAudioProcessorEditor::paint (Graphics& g)
{
	// Fill the whole window white
	g.fillAll (Colours::grey);

	// Set the current drawing colour to black
	g.setColour (Colours::black);

	// Set the font size and draw text to the screen
	g.setFont (15.0f);
}

void PrototypeAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

	int defaultKnobHeight = 80;	// knob height in pixels
	int defaultKnobWidth = 50;	// knob width in pixels

	// Setting button sizes and positions
	masterBypassButton.setBounds(50, 250, 100, 40);
	soloSubButton.setBounds(650, 250, 100, 40);
	switchFilterButton.setBounds(250, 250, 100, 40);
	
	// Setting slider sizes and positions
	inputGainSlider.setBounds(50, 110, defaultKnobWidth, defaultKnobHeight);
	preSubGainSlider.setBounds(150, 110, defaultKnobWidth, defaultKnobHeight);
	bpFreqSlider.setBounds(250, 55, defaultKnobWidth, defaultKnobHeight);
	bpQSlider.setBounds(250, 165, defaultKnobWidth, defaultKnobHeight);
	lpfFreqSlider.setBounds(350, 55, defaultKnobWidth, defaultKnobHeight);
	hpfFreqSlider.setBounds(350, 165, defaultKnobWidth, defaultKnobHeight);
	decaySlider.setBounds(450, 55, defaultKnobWidth, defaultKnobHeight);
	hystSlider.setBounds(450, 165, defaultKnobWidth, defaultKnobHeight);
	colourSlider.setBounds(550, 110, defaultKnobWidth, defaultKnobHeight);
	directGainSlider.setBounds(650, 55, defaultKnobWidth, defaultKnobHeight);
	postSubGainSlider.setBounds(650, 165, defaultKnobWidth, defaultKnobHeight);
	outputGainSlider.setBounds(750, 110, defaultKnobWidth, defaultKnobHeight);
	
	// Setting label sizes and positions
	inputGainLabel.setBounds(35, 90, 80, 15);
	preSubGainLabel.setBounds(135, 90, 80, 15);
	bpFreqLabel.setBounds(235, 35, 80, 15);
	bpQLabel.setBounds(235, 145, 80, 15);
	lpfFreqLabel.setBounds(335, 35, 80, 15);
	hpfFreqLabel.setBounds(335, 145, 80, 15);
	decayLabel.setBounds(435, 35, 80, 15);
	hystLabel.setBounds(435, 145, 80, 15);
	colourLabel.setBounds(550, 90, 80, 15);
	directGainLabel.setBounds(635, 35, 80, 15);
	postSubGainLabel.setBounds(635, 145, 80, 15);
	outputGainLabel.setBounds(735, 90, 80, 15);
}

//==============================================================================
// This timer periodically checks whether any of the filter's parameters have changed...
void PrototypeAudioProcessorEditor::timerCallback() {
	masterBypassButton.setToggleState((bool)getProcessor().paramMasterBypass->getValue(), dontSendNotification);
	soloSubButton.setToggleState((bool)getProcessor().paramSoloSub->getValue(), dontSendNotification);
	switchFilterButton.setToggleState((bool)getProcessor().paramSwitchFilter->getValue(), dontSendNotification);

	inputGainSlider.setValue(getProcessor().paramInputGain->getValue(), dontSendNotification);
	preSubGainSlider.setValue(getProcessor().paramPreSubGain->getValue(), dontSendNotification);
	bpFreqSlider.setValue(getProcessor().paramBpFreq->getValue(), dontSendNotification);
	bpQSlider.setValue(getProcessor().paramBpQ->getValue(), dontSendNotification);
	lpfFreqSlider.setValue(getProcessor().paramLpfFreq->getValue(), dontSendNotification);
	hpfFreqSlider.setValue(getProcessor().paramHpfFreq->getValue(), dontSendNotification);
	decaySlider.setValue(getProcessor().paramDecay->getValue(), dontSendNotification);
	hystSlider.setValue(getProcessor().paramHyst->getValue(), dontSendNotification);
	colourSlider.setValue(getProcessor().paramColour->getValue(), dontSendNotification);
	directGainSlider.setValue(getProcessor().paramDirectGain->getValue(), dontSendNotification);
	postSubGainSlider.setValue(getProcessor().paramPostSubGain->getValue(), dontSendNotification);
	outputGainSlider.setValue(getProcessor().paramOutputGain->getValue(), dontSendNotification);
}

//==============================================================================
// This is our Slider::Listener callback, when the user drags a slider.
void PrototypeAudioProcessorEditor::sliderValueChanged(Slider *slider) {
	//getProcessor().inputGain = inputGainSlider.getValue();
	//getProcessor().outputGain = outputGainSlider.getValue();

	if (AudioProcessorParameter* param = getParameterFromSlider(slider))
	{
		// It's vital to use setValueNotifyingHost to change any parameters that are automatable
		// by the host, rather than just modifying them directly, otherwise the host won't know
		// that they've changed.
		param->setValueNotifyingHost((float)slider->getValue());
	}
}

void PrototypeAudioProcessorEditor::sliderDragStarted(Slider* slider)
{
	if (AudioProcessorParameter* param = getParameterFromSlider(slider))
	{
		param->beginChangeGesture();
	}
}

void PrototypeAudioProcessorEditor::sliderDragEnded(Slider* slider)
{
	if (AudioProcessorParameter* param = getParameterFromSlider(slider)) {
		param->endChangeGesture();
	}
}

//==============================================================================
// TODO:: make to toggleButtonClicked?
void PrototypeAudioProcessorEditor::buttonClicked(Button* button)
{
	if (AudioProcessorParameter* param = getParameterFromButton(button)) {
		param->setValueNotifyingHost((float)button->getToggleState());
	}
	
}


//==============================================================================
AudioProcessorParameter* PrototypeAudioProcessorEditor::getParameterFromSlider(const Slider* slider) const {
	if (slider == &inputGainSlider) { return getProcessor().paramInputGain; }
	if (slider == &preSubGainSlider) { return getProcessor().paramPreSubGain; }
	if (slider == &bpFreqSlider) { return getProcessor().paramBpFreq; }
	if (slider == &bpQSlider) { return getProcessor().paramBpQ; }
	if (slider == &lpfFreqSlider) { return getProcessor().paramLpfFreq; }
	if (slider == &hpfFreqSlider) { return getProcessor().paramHpfFreq; }
	if (slider == &decaySlider) { return getProcessor().paramDecay; }
	if (slider == &hystSlider) { return getProcessor().paramHyst; }
	if (slider == &colourSlider) { return getProcessor().paramColour; }
	if (slider == &directGainSlider) { return getProcessor().paramDirectGain; }
	if (slider == &postSubGainSlider) { return getProcessor().paramPostSubGain; }
	if (slider == &outputGainSlider) { return getProcessor().paramOutputGain; }
	return nullptr;
}

AudioProcessorParameter* PrototypeAudioProcessorEditor::getParameterFromButton(const Button* button) const {
	if (button == &masterBypassButton) { return getProcessor().paramMasterBypass; }
	if (button == &soloSubButton) { return getProcessor().paramSoloSub; }
	if (button == &switchFilterButton) { return getProcessor().paramSwitchFilter; }
}