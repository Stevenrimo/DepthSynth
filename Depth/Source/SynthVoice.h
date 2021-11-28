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
#include "DataProccesing/AdsrData.h"
#include "DataProccesing/OscData.h"
#include "DataProccesing/FilterData.h"

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
    void update(const float attack, const float decay, const float sustain, const float release);
    OscData& getOscillator() { return osc1; }
    AdsrData& getAdsr() { return adsr; }

private:
    // Instance of user made classes 
    AdsrData adsr;
    FilterData filter;
    OscData osc1;
    OscData FMosc;
    // Need to make a second audio buffer to load the synth data into instead of using the output buffer directly
    // By using this synth buffer we are able to remove an issue that cause a clicking sound when a MIDI was pressed 
    juce::AudioBuffer<float> synthBuffer;

    juce::dsp::Gain<float> gain;

    //check to see that our audio callback has been created 
    bool isPrepared { false };
};
