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
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& rApvts, juce::String sWaveSelectorId, juce::String fmFreqID, juce::String fmDepthID)
{
    // Defines the choices in the combobox
    juce::StringArray choice{ "Sine", "Saw", "Square" };
    oscWaveSelector.addItemList(choice, 1);
    oscWaveSelector.setSelectedItemIndex(0);
    // Makes combobox for oscialltor selector visable
    addAndMakeVisible(oscWaveSelector);

    // Connects the front of what the user sees to the backend Data 
    oscSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(rApvts, sWaveSelectorId, oscWaveSelector);

    // Connects knob to valueTreeState so that we can actually manipulate the data
    fmFreqSliderAttachment = std::make_unique< juce::AudioProcessorValueTreeState::SliderAttachment>(rApvts, fmFreqID, fmFreqSlider);
    fmDepthSliderAttachment = std::make_unique< juce::AudioProcessorValueTreeState::SliderAttachment>(rApvts, fmDepthID, fmDepthSlider);

    // Function to display the slider with a label 
    setSliderWithLabel(fmFreqSlider, fmFreqLabel);

    // Function to display Depth slider with label
    setSliderWithLabel(fmDepthSlider, fmDepthLabel);

}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g)
{
    // sets background color to black 
    g.fillAll(juce::Colours::black);
    g.setFont(juce::Font("High Tower Text", 20.0f, juce::Font::bold));
    // TODO place at top of the combobox
    g.setColour(juce::Colours::white);
    g.drawText("Oscillator 1",10,15,100,25, false);
}

void OscComponent::resized()
{
    // Lays out position of GUI elements
    // If these values are changed it should effect all GUI elements
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;

    //If these elements are changed it will efffect all osc Selectors 
    const auto comboBoxStartX = 0 + padding;
    const auto comboBoxStartY = 40 + padding;
    const auto comboBoxWidth = 200;//bounds.getWidth() / 4 - padding;
    const auto comboBoxHeight = 20;//bounds.getHeight() / 4 - padding;
    // sets position of osc selector 
    oscWaveSelector.setBounds(comboBoxStartX, comboBoxStartY, comboBoxWidth, comboBoxHeight);

    //Draws the FmFrequency slider
    fmFreqSlider.setBounds(0, 100, 100, 90);
    fmFreqLabel.setBounds(fmFreqSlider.getX(), fmFreqSlider.getY() - 20, fmFreqSlider.getWidth(), 20);


    //Draws the FmDepth slider
    fmDepthSlider.setBounds(fmFreqSlider.getWidth() + padding, 100, 100, 90);
    fmDepthLabel.setBounds(fmDepthSlider.getX(), fmDepthSlider.getY() - 20, fmDepthSlider.getWidth(), 20);
}

void OscComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label)
{
    // Setting the slider to be a rotary knob
    slider.setSliderStyle(juce::Slider::Rotary);
    // places the textbox below the knob
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    slider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::forestgreen);
    //rSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::forestgreen);
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::purple);
    // Makes knob visable
    addAndMakeVisible(slider);
    // Displays the label for the slider
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(juce::Font("High Tower Text", 15.0f, juce::Font::bold));
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}
