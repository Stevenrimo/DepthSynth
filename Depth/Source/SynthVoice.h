/*
  ==============================================================================

    SynthVoice.h
    Created: 26 Sep 2021 8:30:29pm
    Author:  steve

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    //Need to add these as they are pure virtual functiosn that need to be overridden
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock,int outputChannels);

private:
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;  

    juce::dsp::Oscillator<float> osc{ [](float x) {return x < 0.0f ? -1 : 1.0f;} };
    juce::dsp::Gain<float> gain;
    //check to see that our audio callback has been created 
    bool isPrepared { false };

    //return std::sin(x);       //Sine Wave
    //return x / MathConstants<float>::pi //Saw Wave
    //return x < 0.0f ? -1 : 1.0f; //Square Wave

};
