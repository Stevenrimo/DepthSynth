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
    processFMOsc(block);
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

void OscData::setFMWaveType(const int choice)
{
    //return std::sin(x);       //sine wave
//return x / juce::mathconstants<float>::pi; //saw wave
//return x < 0.0f ? -1 : 1.0f; //square wave

    switch (choice)
    {
        //sine wave
    case 0:
        fmOsc.initialise([](float x) {return std::sin(x); });
        break;

        //Saw wave
    case 1:
        fmOsc.initialise([](float x) {return x / juce::MathConstants<float>::pi; });
        break;

        //square wave
    case 2:
        fmOsc.initialise([](float x) {return x < 0.0f ? -1 : 1.0f; });
        break;

    default:
        jassertfalse;
        break;
    }
}

void OscData::setFMParams(const float depth, const float freq)
{
    // Sets the FM paramters
    fmOsc.setFrequency(freq);
    fmDepth = depth;

    // Gets the current freq with fmModulation applied to  it
    auto currentFreq = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod;

    // If the current Freq after applying modulation is negative we are multiplying it by -1 to make it positive 
    // If we dont do this we will get an error 
    setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
}

void OscData::processFMOsc(juce::dsp::AudioBlock<float>& block)
{
    // Applies our FM  synthesis by going through each channel and sample 
    for (int ch = 0; ch < block.getNumChannels(); ch++)
    {
        for (int s = 0; s < block.getNumSamples(); s++)
        {
            fmMod = fmOsc.processSample(block.getSample(ch, s)) * fmDepth;
        }
    }
}

//juce::dsp::Oscillator<float> OscData::getFmOsc()
//{
//    return fmOsc;
//}
