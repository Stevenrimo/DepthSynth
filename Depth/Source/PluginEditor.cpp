/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DepthAudioProcessorEditor::DepthAudioProcessorEditor (DepthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),osc(audioProcessor.apvts, "OSC1WAVETYPE","FMOSCWAVETYPE","FMFREQ","FMDEPTH"), adsr(audioProcessor.apvts), filter(audioProcessor.apvts, "FILTERTYPE")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 400);

    // Make adsr visable
    addAndMakeVisible(adsr);

    // make osc1 selector 
    addAndMakeVisible(osc);

    //make filter visable
    addAndMakeVisible(filter);
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
    adsr.setBounds(0, 0, getWidth() / 2, getHeight());
    // sets osc bounds
    osc.setBounds(0, 0, getWidth() / 2, getHeight()/2);
    // sets filters bounds
    filter.setBounds(400, 0, getWidth() / 2, getHeight());

}


