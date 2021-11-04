/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"

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


    // Combobox for selecting oscilattor waveform
    juce::ComboBox oscSelector;

    //unique pointer that will be used to map the GUI comboBox to the internal AudioProcessValueTreeState controls
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelectorAttachment; 

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DepthAudioProcessor& audioProcessor;

    AdsrComponent adsr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DepthAudioProcessorEditor)
};
