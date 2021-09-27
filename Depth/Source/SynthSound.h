/*
  ==============================================================================

    SynthSound.h
    Created: 26 Sep 2021 8:30:54pm
    Author:  steve

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:

    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};