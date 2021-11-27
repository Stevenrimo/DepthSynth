/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 3 Nov 2021 8:28:32pm
    Author:  steve

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrComponent.h"

//==============================================================================
AdsrComponent::AdsrComponent(juce::AudioProcessorValueTreeState& rApvts)
{
    // Only needs access to apvts instead of whole audio proccessor for encapsulation reasons (should give more data than is needed)
    // Instanciates the GUI attachment elements for the ADSR and oscilator selector
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    attackAttachment = std::make_unique<SliderAttachment>(rApvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(rApvts, "DECAY", decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(rApvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(rApvts, "RELEASE", releaseSlider);

    // Creates the sliders on the GUI Interface
    juce::String sAttack = "Attack";
    setSliderParams(attackSlider, sAttack);
    juce::String sDecay = "Decay";
    setSliderParams(decaySlider, sDecay);
    juce::String sSustain = "Sustain";
    setSliderParams(sustainSlider, sSustain);
    juce::String sRelease = "Release";
    setSliderParams(releaseSlider, sRelease);

}

AdsrComponent::~AdsrComponent()
{
}

void AdsrComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::green);
    //g.setColour(juce::Colours::sandybrown);
    //g.drawRect(0, 0, getLocalBounds().getWidth(), getLocalBounds,getHeight(), 3);
    g.setFont(juce::Font("High Tower Text", 20.0f, juce::Font::bold));
    // TODO place at top of the combobox
    g.drawText("Envelope", 10, 215, 100, 25, false);
    // setting the font to be smaller for slider labels
    g.setFont(juce::Font("High Tower Text", 15.0f, juce::Font::bold));
    g.drawText("Attack", 25, 240, 50, 50, false);
    g.drawText("Decay", 120, 240, 50, 50, false);
    g.drawText("Sustain", 215, 240, 50, 50, false);
    g.drawText("Release", 310, 240, 50, 50, false);
}

void AdsrComponent::resized()
{
    // Lays out position of GUI elements
   // If these values are changed it should effect all GUI elements
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;

    //If these elements are changed it will efffect all slider elements 
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getHeight() / 3.5 - padding;
    const auto sliderStartX = 0;
    const auto sliderStartY = bounds.getHeight() - sliderHeight - padding; //- (sliderHeight / 4);

    // Creates the bounds of the sliders
    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);

}

void AdsrComponent::setSliderParams(juce::Slider& rSlider, juce::String& rKnob)
{
    rSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    rSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::forestgreen);
    //rSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::forestgreen);
    rSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::purple);
    rSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 25);
    //rSlider.setHelpText(rKnob);
    //rSlider.setHelpText(rKnob)
    label.setFont(juce::Font("High Tower Text", 20.0f, juce::Font::bold));
    label.setText(rKnob, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
    addAndMakeVisible(rSlider);
}