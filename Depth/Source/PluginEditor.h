/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"
#include "UI/OscComponent.h"
#include "UI/FilterComponent.h"

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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DepthAudioProcessor& audioProcessor;
    // Instance of our internal OscComponent UI class
    OscComponent osc;
    // Instance of our internal adsrComponent UI class
    AdsrComponent adsr;
    // Instance of our internal filterComponent UI class
    FilterComponent filter;
    // Instance of ADSR for modulation fucnitons
    AdsrComponent modAdsr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DepthAudioProcessorEditor)
};
