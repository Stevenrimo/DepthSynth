/*
  ==============================================================================

    AdsrData.h
    Created: 3 Nov 2021 8:29:38pm
    Author:  steve

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AdsrData : public juce::ADSR
{
public:
    void updateADSR(const float fAttack, const float fDecay, const float fSustain, const float fRelease);

private:
    juce::ADSR::Parameters adsrParams;
};
