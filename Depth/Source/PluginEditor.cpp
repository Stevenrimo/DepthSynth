/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DepthAudioProcessorEditor::DepthAudioProcessorEditor (DepthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), adsr (audioProcessor.apvts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 400);

    oscSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC", oscSelector);

    // Make adsr visable
    addAndMakeVisible(adsr);
}

DepthAudioProcessorEditor::~DepthAudioProcessorEditor()
{
}

//==============================================================================
void DepthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.fillAll(juce::Colours::black);

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Welcome To Depth!", getLocalBounds(), juce::Justification::centred, 1);
}

void DepthAudioProcessorEditor::resized()
{
    // set adsr bounds
    adsr.setBounds(0, 0, getWidth() /2, getHeight());
}


