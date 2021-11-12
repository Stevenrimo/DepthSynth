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
    OscComponent(juce::AudioProcessorValueTreeState& rApvts, juce::String sWaveSelectorId);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Combobox for selecting oscilattor waveform
    juce::ComboBox oscWaveSelector;
    //unique pointer that will be used to map the GUI comboBox to the internal AudioProcessValueTreeState controls
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelectorAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
