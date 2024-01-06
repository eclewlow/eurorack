/*
  ==============================================================================

    drive.h
    Created: 14 Nov 2023 4:36:43pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/dsp/fx/effect.h"

class Drive: public Effect {
public:

    Drive() {
        phase_ = 0.0f;
    }
    ~Drive() {}
    void Init();
    void Reset();
    float RenderSampleEffect(float sample, float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope);
    float RenderPhaseEffect(float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope);
protected:
private:
    DISALLOW_COPY_AND_ASSIGN (Drive);
};
