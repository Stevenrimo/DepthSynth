/*
  ==============================================================================

    OscComponent.cpp
    Created: 8 Nov 2021 10:43:41pm
    Author:  steve

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& rApvts, juce::String sWaveSelectorId)
{
    juce::StringArray choice{ "Sine", "Saw", "Square" };
    oscWaveSelector.addItemList(choice, 1);
    oscWaveSelector.setSelectedItemIndex(0);
    addAndMakeVisible(oscWaveSelector);

    // Connects the front of what the user sees to the backend Data 
    oscSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(rApvts, sWaveSelectorId, oscWaveSelector);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g)
{
    // sets background color to black 
    g.fillAll(juce::Colours::black);
}

void OscComponent::resized()
{
    // Lays out position of GUI elements
    // If these values are changed it should effect all GUI elements
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;

    //If these elements are changed it will efffect all osc Selectors 
    const auto comboBoxStartX = 0;
    const auto comboBoxStartY = 0;
    const auto comboBoxWidth = 90;//bounds.getWidth() / 4 - padding;
    const auto comboBoxHeight = 20;//bounds.getHeight() / 4 - padding;
    // sets position of osc selector 
    oscWaveSelector.setBounds(comboBoxStartX, comboBoxStartY, comboBoxWidth, comboBoxHeight);

}
