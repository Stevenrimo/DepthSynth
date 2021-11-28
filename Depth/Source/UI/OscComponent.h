/*
  ==============================================================================

    OscComponent.h
    Created: 8 Nov 2021 10:43:41pm
    Author:  steve

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OscComponent  : public juce::Component
{
public:
    OscComponent(juce::AudioProcessorValueTreeState& rApvts, juce::String sWaveSelectorId, juce::String sFMSelectorID, juce::String fmFreqID, juce::String fmDepthID);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setSliderWithLabel(juce::Slider& slider, juce::Label& label);

private:
    // Knobs for controlling FM
    juce::Slider fmFreqSlider;
    juce::Slider fmDepthSlider;

    // Attachs slider to data in the audioProcessorValueTreeState
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmFreqSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmDepthSliderAttachment;

    // Label for frequency knob
    juce::Label fmFreqLabel{ "FM Freq", "FM Freq" };

    // Label for Depth Knob
    juce::Label fmDepthLabel{ "FM Depth", "FM Depth" };

    // Combobox for selecting oscilattor waveform
    juce::ComboBox oscWaveSelector;
    juce::ComboBox fmWaveSelector; 

    //unique pointer that will be used to map the GUI comboBox to the internal AudioProcessValueTreeState controls
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelectorAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> fmOscSelectorAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
