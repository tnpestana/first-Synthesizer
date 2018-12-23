/*
  ==============================================================================

    OscillatorEditor.cpp
    Created: 17 Apr 2018 1:57:27pm
    Author:  Tiago Pestana

	Main application is suposed to run more than one oscillator, so here is a
	template for its GUI added to the main editor as many times as needed.

  ==============================================================================
*/

#include "OscillatorEditor.h"

OscillatorEditor::OscillatorEditor(TnpMidiSynthAudioProcessor& p, AudioProcessorValueTreeState& apvts)
{
	// Title label.
	addAndMakeVisible(titleLabel);
	titleLabel.setText("OSCILLATOR", dontSendNotification);
	titleLabel.setJustificationType(Justification::centred);

	// Number of voices components. 
	addAndMakeVisible(numVoicesLabel);
	numVoicesLabel.setText("voices: ", dontSendNotification);
	numVoicesLabel.setJustificationType(Justification::bottomLeft);
	addAndMakeVisible(numVoicesInput);
	for (int i = 1; i < 11; i++)							//	Even though the numVoices parameter's range is defined we
		numVoicesInput.addItem((String)i, i);				// seem to need to populate the combo box anyway.
	numVoicesInput.addItem("16", 11);
	numVoicesInput.addItem("32", 12);
	numVoicesAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, "oscNumVoices", numVoicesInput);

	// Transpose
	addAndMakeVisible(transposeSlider);
	transposeSlider.setSliderStyle(Slider::LinearHorizontal);
	transposeSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 40, 15);
	addAndMakeVisible(transposeLabel);
	transposeLabel.setText("transpose: ", dontSendNotification);
	transposeLabel.setJustificationType(Justification::bottomLeft);
	transposeAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (apvts, "oscTranspose", transposeSlider);

	// Oscillator type.
	addAndMakeVisible(oscTypeLabel);
	oscTypeLabel.setText("wave type:", dontSendNotification);
	oscTypeLabel.setJustificationType(Justification::bottomLeft);
	addAndMakeVisible(oscTypeInput);
	oscTypeInput.addItem("sine", 1);
	oscTypeInput.addItem("harmonic", 2);			//	Even though the oscType parameter's range is defined we
	oscTypeInput.addItem("square", 3);					// seem to need to populate the combo box anyway.
	oscTypeInput.addItem("triangle", 4);
	oscTypeInput.addItem("sawtooth", 5);
	oscTypeAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, "oscType", oscTypeInput);

	// Attack.
	addAndMakeVisible(attackSlider);
	attackSlider.setSliderStyle(Slider::RotaryVerticalDrag);
	attackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 15);
	addAndMakeVisible(attackLabel);
	attackLabel.setText("attack", dontSendNotification);
	attackLabel.setJustificationType(Justification::centredBottom);
	attackAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (apvts, "volEnvAttack", attackSlider);

	// Decay.
	addAndMakeVisible(decaySlider);
	decaySlider.setSliderStyle(Slider::RotaryVerticalDrag);
	decaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 15);
	addAndMakeVisible(decayLabel);
	decayLabel.setText("decay", dontSendNotification);
	decayLabel.setJustificationType(Justification::centredBottom);
	decayAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, "volEnvDecay", decaySlider);

	// Sustain.
	addAndMakeVisible(sustainSlider);
	sustainSlider.setSliderStyle(Slider::RotaryVerticalDrag);
	sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 15);
	addAndMakeVisible(sustainLabel);
	sustainLabel.setText("sustain", dontSendNotification);
	sustainLabel.setJustificationType(Justification::centredBottom);
	sustainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, "volEnvSustain", sustainSlider);

	// Release. 
	addAndMakeVisible(releaseSlider);
	releaseSlider.setSliderStyle(Slider::RotaryVerticalDrag);
	releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 15);
	addAndMakeVisible(releaseLabel);
	releaseLabel.setText("release", dontSendNotification);
	releaseLabel.setJustificationType(Justification::centredBottom);
	releaseAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, "volEnvRelease", releaseSlider);
}

OscillatorEditor::~OscillatorEditor()
{
}

void OscillatorEditor::paint(Graphics& g)
{
	titleLabel.setColour(Label::backgroundColourId, Colours::lightgrey);
	transposeSlider.setColour(Slider::textBoxTextColourId, Colours::black);
	attackSlider.setColour(Slider::textBoxTextColourId, Colours::black);
	decaySlider.setColour(Slider::textBoxTextColourId, Colours::black);
	sustainSlider.setColour(Slider::textBoxTextColourId, Colours::black);
	releaseSlider.setColour(Slider::textBoxTextColourId, Colours::black);
	numVoicesInput.setColour(ComboBox::textColourId, Colours::black);
	oscTypeInput.setColour(ComboBox::textColourId, Colours::black);
}

void OscillatorEditor::resized()
{
	// Total oscillator area.
	juce::Rectangle<int> oscArea(getLocalBounds());
	titleLabel.setBounds(oscArea.removeFromTop(20).reduced(2));

	// Controls area.
	juce::Rectangle<int> controls(oscArea.removeFromTop(oscArea.getHeight() * 0.30).reduced(2));

	// Number of voices selection area.   
	juce::Rectangle<int> numVoicesArea(controls.removeFromLeft(60));
	numVoicesLabel.setBounds(numVoicesArea.removeFromTop(20));
	numVoicesInput.setBounds(numVoicesArea.reduced(2));
	
	// Oscillator Type.
	juce::Rectangle<int> oscTypeArea(controls.removeFromLeft(100));
	oscTypeLabel.setBounds(oscTypeArea.removeFromTop(20));
	oscTypeInput.setBounds(oscTypeArea.reduced(2));

	// Transpose selection area.
	juce::Rectangle<int> transposeArea(controls);
	transposeLabel.setBounds(transposeArea.removeFromTop(20));
	transposeSlider.setBounds(transposeArea);

	// Envelope sliders and labels area - needs reviewing.
	juce::Rectangle<int> oscEnvelope(oscArea.reduced(10));
	const double sliderWidth = oscEnvelope.getWidth() / 4;

	juce::Rectangle<int> oscEnvelopeLabel(oscEnvelope.removeFromTop(20));
	attackLabel.setBounds(oscEnvelopeLabel.removeFromLeft(sliderWidth));
	decayLabel.setBounds(oscEnvelopeLabel.removeFromLeft(sliderWidth));
	sustainLabel.setBounds(oscEnvelopeLabel.removeFromLeft(sliderWidth));
	releaseLabel.setBounds(oscEnvelopeLabel.removeFromLeft(sliderWidth));

	juce::Rectangle<int> oscEnvelopeSlider(oscEnvelope);
	attackSlider.setBounds(oscEnvelopeSlider.removeFromLeft(sliderWidth));
	decaySlider.setBounds(oscEnvelopeSlider.removeFromLeft(sliderWidth));
	sustainSlider.setBounds(oscEnvelopeSlider.removeFromLeft(sliderWidth));
	releaseSlider.setBounds(oscEnvelopeSlider.removeFromLeft(sliderWidth));
}