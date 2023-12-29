/*
  ==============================================================================

    bitcrusher.h
    Created: 14 Nov 2023 4:36:59pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/dsp/fx/effect.h"

class Bitcrush: public Effect {
public:

    Bitcrush() {
        phase = 0.0f;
    }
    ~Bitcrush() {}
    void Init();
    void Reset();
    float RenderSampleEffect(float sample, float input_phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling);
    float RenderPhaseEffect(float input_phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling);
    float GetSample(float phase);
protected:
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Bitcrush);
};
