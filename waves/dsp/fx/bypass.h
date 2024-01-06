/*
  ==============================================================================

    bypass.h
    Created: 17 Nov 2023 5:42:23pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/dsp/fx/effect.h"

class Bypass: public Effect {
public:

    Bypass() {
        phase_ = 0.0f;
    }
    ~Bypass() {}
    void Init();
    void Reset();
    float RenderSampleEffect(float sample, float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling);
    float RenderPhaseEffect(float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling);
protected:
private:
    DISALLOW_COPY_AND_ASSIGN (Bypass);
};
