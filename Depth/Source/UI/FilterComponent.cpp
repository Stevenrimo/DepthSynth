/*
  ==============================================================================

    FilterComponent.cpp
    Created: 14 Nov 2021 10:20:03am
    Author:  steve

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& rApvts, juce::String sFilterType)
{
    // Initilizes the choices for the combobox selector
    juce::StringArray choice{ "LowPass", "BandPass","HighPass" };
    filterTypeSelector.addItemList(choice, 1);
    filterTypeSelector.setSelectedItemIndex(0);
    addAndMakeVisible(filterTypeSelector);

    // connects the front end UI element to backend Data
    filSelectorAttachment = std::make_unique < juce::AudioProcessorValueTreeState::ComboBoxAttachment>(rApvts, sFilterType, filterTypeSelector);
    
    // TODO Init and connect the cutoff and res knobs 
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    filCutoffAttachment = std::make_unique<SliderAttachment>(rApvts, "FILTERCUTOFF", filterCutoffSlider);
    filResAttachment = std::make_unique<SliderAttachment>(rApvts, "FILTERRES", filterResSlider);

    std::string sFilterCutoff = "Hz";
    setSliderParams(filterCutoffSlider, sFilterCutoff, filterCutoffLabel);

    std::string sFilterRes = "";
    setSliderParams(filterResSlider, sFilterRes, filterResLabel);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    // sets background color to black 
    g.fillAll(juce::Colours::black);
    g.setFont(juce::Font("High Tower Text", 20.0f, juce::Font::bold));
    // TODO place at top of the combobox
    g.setColour(juce::Colours::white);
    g.drawText("Filter", 10, 15, 100, 25, false);
}

void FilterComponent::resized()
{
    // Lays out position of GUI elements
   // If these values are changed it should effect all GUI elements
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;

    //If these elements are changed it will efffect all slider elements 
    const auto sliderWidth = 100;//bounds.getWidth() / 4 - padding;
    const auto sliderHeight = 90;//bounds.getHeight() / 3.5 - padding;
    const auto sliderStartX = 0;
    const auto sliderStartY = 100;

    //sets postition of filter type selector 
    filterTypeSelector.setBounds(padding, 50, 150, 20);

    // Creates the Bounds of the sliders
    filterCutoffSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    filterResSlider.setBounds(filterCutoffSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);

}

void FilterComponent::setSliderParams(juce::Slider& rSlider, std::string& rKnob, juce::Label& rLabel)
{
    rSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    rSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::forestgreen);
    //rSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::forestgreen);
    rSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::purple);
    rSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 25);
    //rSlider.setHelpText(rKnob);
    rSlider.setTextValueSuffix(rKnob);
    addAndMakeVisible(rSlider);
    // Displays the label for the slider
    rLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    rLabel.setFont(juce::Font("High Tower Text", 14.0f, juce::Font::bold));
    rLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(rLabel);
}