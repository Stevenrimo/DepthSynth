/*
  ==============================================================================

    FilterData.cpp
    Created: 12 Nov 2021 8:54:29pm
    Author:  steve

  ==============================================================================
*/

#include "FilterData.h"

void FilterData::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
{
    // Resets the filter before we play the sound
    filter.reset();

    // A juce definied datastructure to be passed into A DSP algorithm
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numChannels; 
    
    // Initializes the Filter
    filter.prepare(spec);

    // Setting to true once filter has been initilzied 
    isPrepared = true;

}

void FilterData::process(juce::AudioBuffer<float>& buffer)
{
    // Making sure that filter is initizilied or will throw an error
    jassert(isPrepared);

    //create audio block from the buffer
    juce::dsp::AudioBlock<float> block{ buffer };

    // replacing the contents of what is the buffer with what the filter processes
    filter.process(juce::dsp::ProcessContextReplacing<float> { block });

     
}

void FilterData::reset()
{
    filter.reset();
}

void FilterData::updateParameters(const int nFilterType, const float fFrequency, const float fResonance, const float fModulator)
{
    // based on the value that is passed in it will make the filter either a lowpass, highpass or bandpass
    switch (nFilterType)
    {
    case 0:
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        break;

    case 1:
        filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        break;

    case 2:
        filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
        break;
    }

    // Takes modulator value and multiple by our frequecny to get modulated frequnecy 
    float fModFreq = fFrequency * fModulator;
    // if the modulated frequency is below 20hz we will set it to 20hz
    fModFreq = std::fmax(fModFreq, 20.0f);              // returns max of the two values
    // if the modulated frequency is above 20,000 hz we will set it to 20,000hz
    fModFreq = std::fmin(fModFreq, 20000.0f);           // returns the min of two values

    // Sets the cutoff frequency 
    filter.setCutoffFrequency(fModFreq);

    // Sets teh resonance of the filter 
    filter.setResonance(fResonance);

}
