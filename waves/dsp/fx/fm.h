/*
  ==============================================================================

    fm.h
    Created: 13 Nov 2023 6:30:54pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/dsp/fx/effect.h"

class FM: public Effect {
public:

    FM() {
        phase_ = 0.0f;
    }
    ~FM() {}
    void Init();
    void Reset();
    float RenderSampleEffect(float sample, float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope);
    float RenderPhaseEffect(float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope);
protected:
private:
    DISALLOW_COPY_AND_ASSIGN (FM);
};
