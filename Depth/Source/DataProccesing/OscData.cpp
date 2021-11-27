/*
  ==============================================================================

    OscData.cpp
    Created: 8 Nov 2021 10:15:19pm
    Author:  steve

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
    fmOsc.prepare(spec);
    prepare(spec);
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    // Does our FM Proccesing bu going throught the number of channels and samples 
    for (int ch = 0; ch < block.getNumChannels(); ch++)
    {
        for (int s = 0; s < block.getNumSamples(); s++)
        {
            // sets fmModulation value to be the current sample 
            fmMod = fmOsc.processSample(block.getSample(ch, s)); 
        }
    }
    process(juce::dsp::ProcessContextReplacing<float>(block));
}

void OscData::setWaveFrequency(const int midiNoteNumber)
{
    // converts midi note number to hertz
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    lastMidiNote = midiNoteNumber;
}

void OscData::setWaveType(const int choice)
{
    //return std::sin(x);       //sine wave
    //return x / juce::mathconstants<float>::pi; //saw wave
    //return x < 0.0f ? -1 : 1.0f; //square wave

    switch (choice)
    {
        //sine wave
        case 0:
            initialise([](float x) {return std::sin(x); });
            break;

        //Saw wave
        case 1:
            initialise([](float x) {return x / juce::MathConstants<float>::pi; });
            break;

        //square wave
        case 2:
            initialise([](float x) {return x < 0.0f ? -1 : 1.0f; });
            break;

        default:
            jassertfalse;
            break;
    }
}

void OscData::setFMParams(const float depth, const float freq)
{
    fmOsc.setFrequency(freq);
    fmDepth = depth;
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod);
}
