/*
  ==============================================================================

    AdsrData.cpp
    Created: 3 Nov 2021 8:29:38pm
    Author:  steve

  ==============================================================================
*/

#include "AdsrData.h"
void AdsrData::updateADSR(const float fAttack, const float fDecay, const float fSustain, const float fRelease)
{
    adsrParams.attack = fAttack;
    adsrParams.decay = fDecay;
    adsrParams.sustain = fSustain;
    adsrParams.release = fRelease;

    setParameters(adsrParams);

}