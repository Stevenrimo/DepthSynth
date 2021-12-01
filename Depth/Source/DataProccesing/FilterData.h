/*
  ==============================================================================

    FilterData.h
    Created: 12 Nov 2021 8:54:28pm
    Author:  steve

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class FilterData
{
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();
    void updateParameters(const int filterType, const float frequency, const float resonance, const float modulator = 1.0f);



private:
    juce::dsp::StateVariableTPTFilter<float> filter;
    bool isPrepared{ false };
};