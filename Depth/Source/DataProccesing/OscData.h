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
    void setFMParams(const float depth, const float freq);


private:
    // Init of FM oscillator, hard coded to sine for now TODO add customizablity 
    juce::dsp::Oscillator<float> fmOsc{ [](float x) {return std::sin(x); } };
    // usied to change the frequencey via fm
    float fmMod; 
    // FM depth esstenially controls the gain, or the amount of FM that we are using
    float fmDepth;
    // last midi note, used for calculating FM
    int lastMidiNote;
};