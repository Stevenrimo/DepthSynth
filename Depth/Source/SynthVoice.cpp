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
    osc.setWaveFrequency(midiNoteNumber);
    adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();

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
        // Need to clear the buffer everytime we render a new block 
        synthBuffer.clear();

        juce::dsp::AudioBlock<float> audioBlock{ outputBuffer };
        osc.getNextAudioBlock(audioBlock);
        gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

        adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

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
    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    osc.prepareToPlay(spec);
    gain.prepare(spec);

    // Set the overall gain/ volume for the synth 
    gain.setGainLinear(0.04f);

    isPrepared = true;
}

void SynthVoice::update(const float fAttack, const float fDecay, const float fSustain, const float fRelease)
{
    adsr.updateADSR(fAttack, fDecay, fSustain, fRelease);
    //filter.updateParameters(
}
