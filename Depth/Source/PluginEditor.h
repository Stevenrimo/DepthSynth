/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DepthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DepthAudioProcessorEditor (DepthAudioProcessor&);
    ~DepthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Sliders that will be styled as rotary knobs to control the ADSR, THESE ARE GUI ELEMENTS
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    // Combobox for selecting oscilattor waveform
    juce::ComboBox oscSelector;

    // Using Unique pointer here so that when the plugin is closed the memory will free up
    // Unique pointers that will be used to map the GUI elements to the internal AudioProcessValueTreeState Controls 
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;

    //unique pointer that will be used to map the GUI comboBox to the internal AudioProcessValueTreeState controls
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelectorAttachment; 

    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DepthAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DepthAudioProcessorEditor)
};
