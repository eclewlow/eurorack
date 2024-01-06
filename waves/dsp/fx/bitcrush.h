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
        phase_ = 0.0f;
    }
    ~Bitcrush() {}
    void Init();
    void Reset();
    float RenderSampleEffect(float sample, float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling);
    float RenderPhaseEffect(float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling);
protected:
private:
    DISALLOW_COPY_AND_ASSIGN (Bitcrush);
};
