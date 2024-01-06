/*
  ==============================================================================

    ring_modulator.h
    Created: 13 Nov 2023 6:31:18pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/dsp/fx/effect.h"

class RingModulator: public Effect {
public:

    RingModulator() {
        phase_ = 0.0f;
    }
    ~RingModulator() {}
    void Init();
    void Reset();
    float RenderSampleEffect(float sample, float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling);
    float RenderPhaseEffect(float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling);
    float GetSample(float phase);
protected:
private:
    DISALLOW_COPY_AND_ASSIGN (RingModulator);
};
