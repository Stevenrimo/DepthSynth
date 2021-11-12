/*
  ==============================================================================

    OscData.h
    Created: 8 Nov 2021 10:15:19pm
    Author:  steve

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator<float>
{
public:
    void prepareToPlay(juce::dsp::ProcessSpec& spec);
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);
    void setWaveFrequency(const int midiNoteNumber);
    void setWaveType(const int choice);


private:


};