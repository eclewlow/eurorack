/*
  ==============================================================================

    suboscillator.h
    Created: 19 Nov 2023 4:08:34am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/dsp/fx/effect.h"
#include "waves/dsp/Engine.h"

class Suboscillator;

class Suboscillator: public Engine
{
public:
    Suboscillator();
    ~Suboscillator();
    virtual void Render(AudioDac::Frame* output, size_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph);
    float Render();
    void Init();
    float GetSample(int16_t wavetable, int16_t frame, float phase);
    float GetSampleBetweenFrames(float phase, float thisX);
    float GetSampleNoFX(float phase, float fx, float morph);
    void FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx);
    void triggerUpdate();
private:
    DISALLOW_COPY_AND_ASSIGN (Suboscillator);
};
