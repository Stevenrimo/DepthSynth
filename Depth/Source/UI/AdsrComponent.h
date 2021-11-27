/*
  ==============================================================================

    AdsrComponent.h
    Created: 3 Nov 2021 8:28:32pm
    Author:  steve

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AdsrComponent  : public juce::Component
{
public:
    AdsrComponent(juce::AudioProcessorValueTreeState& rApvts);
    ~AdsrComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    void setSliderParams(juce::Slider& rSlider, juce::String& rKnob);

    // Sliders that will be styled as rotary knobs to control the ADSR, THESE ARE GUI ELEMENTS
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    juce::Label label;

    // Using Unique pointer here so that when the plugin is closed the memory will free up
// Unique pointers that will be used to map the GUI elements to the internal AudioProcessValueTreeState Controls 
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdsrComponent)
};
