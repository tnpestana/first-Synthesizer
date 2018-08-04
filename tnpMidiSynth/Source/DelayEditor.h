/*
  ==============================================================================

    DelayEditor.h
    Created: 4 Aug 2018 6:29:27pm
    Author:  tnpes

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class DelayEditor : public Component
{
public:
	DelayEditor(TnpMidiSynthAudioProcessor&, AudioProcessorValueTreeState&);
	~DelayEditor();

private:
	void paint(Graphics&) override;
	void resized() override;
	
	Label titleLabel;
	Label delayTimeLabel;
	Label delayFeedbackLabel;
	Label delayMixLabel;

	Slider delayTimeSlider;
	Slider delayFeedbackSlider; 
	Slider delayMixSlider;

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> delayTimeAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> delayFeedbackAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> delayMixAttachment;
};