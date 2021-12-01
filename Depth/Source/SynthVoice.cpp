/*
  ==============================================================================

    SynthVoice.cpp
    Created: 26 Sep 2021 8:30:29pm
    Author:  steve

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    // Sets the frequency of the wave in the oscialltor to the midi note that is triggered
    osc1.setWaveFrequency(midiNoteNumber);
    // Lets the envelope know that the note is on 
    adsr.noteOn();
    // Lets the mod envelope know that the note is on 
    modAdsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    // Tells the envelopes to turn off the notes 
    adsr.noteOff();
    modAdsr.noteOff();

    if ((!allowTailOff) || (!adsr.isActive()))
    {
        clearCurrentNote();
    }
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    // If prepareToPlay has not been called will throw an error
    jassert(isPrepared);

    // If there are no voices active there is no need to do this processsing and we will return out of this funciton
    if (isVoiceActive())
    {
        // Setting size of synth buffer
        synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
        // Need to run this function before we use an envelope but we dont want the mod adsr to effect the data 
        // In our buffer so we put it here so that it is cleared off the buffer before we play the note
        modAdsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
        // Need to clear the buffer everytime we render a new block 
        synthBuffer.clear();

        // Creates a new audioBlock with the synth Buffer
        juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };

        // Gets the Audio block 
        osc1.getNextAudioBlock(audioBlock);

        // applies the adsr parmeters to the data in the synth buffer 
        adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
        // Procces teh Filter data 
        filter.process(synthBuffer);
        gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));


        // will pause the plugin: can uncomment for debugging 
        //if (startSample != 0)
        //{
        //    jassertfalse;
        //}

        // iterates through channels 
        for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
        {
            outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

            // If there is no more input we stop 
            if (!adsr.isActive())
            {
                clearCurrentNote();
            }
        }
    }
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    // Prepares teh Oscillator to play 
    osc1.prepareToPlay(spec);
    // Sets teh sample rate of the ADSR
    adsr.setSampleRate(sampleRate);
    // prepares the filter to play 
    filter.prepareToPlay(sampleRate, samplesPerBlock,outputChannels);
    // Sets the sample rate of the Mod ADSR
    modAdsr.setSampleRate(sampleRate);
    // Prepares the gain 
    gain.prepare(spec);

    // Set the overall gain/ volume for the synth 
    gain.setGainLinear(0.07f);

    isPrepared = true;
}

void SynthVoice::updateADSR(const float fAttack, const float fDecay, const float fSustain, const float fRelease)
{
    adsr.updateADSR(fAttack, fDecay, fSustain, fRelease);
}

void SynthVoice::updateFilter(const int filterType, const float cutoff, const float resonance)
{
    // sets the modulator value 
    float modulator = modAdsr.getNextSample();
    // updates the filter parameters
    filter.updateParameters(filterType, cutoff, resonance, modulator);
}

void SynthVoice::updateModAdsr(const float fAttack, const float fDecay, const float fSustain, const float fRelease)
{
    modAdsr.updateADSR(fAttack, fDecay, fSustain, fRelease);
}

