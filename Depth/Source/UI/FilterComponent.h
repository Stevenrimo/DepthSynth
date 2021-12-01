/*
  ==============================================================================

    FilterComponent.h
    Created: 14 Nov 2021 10:20:03am
    Author:  steve

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& rApvts, juce::String sFilterType);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    void setSliderParams(juce::Slider&, std::string& rKnob, juce::Label& rLabel);

    // UI components 
    juce::Slider filterCutoffSlider;
    juce::Slider filterResSlider;
    juce::ComboBox filterTypeSelector;

    // Labels for knobs
    juce::Label filterCutoffLabel{ "Cutoff", "Cutoff" };
    juce::Label filterResLabel{ "Resonance", "Resonance" };

    // unique pointer that attachs the UI element to the valueTreeState
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filCutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filResAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filSelectorAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
