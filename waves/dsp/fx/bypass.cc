/*
  ==============================================================================

    bypass.cpp
    Created: 17 Nov 2023 5:42:23pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/fx/effect.h"
#include "waves/Globals.h"

void Bypass::Init() {
    phase_ = 0.0f;
}

void Bypass::Reset() {
    phase_ = 0.0f;
}

float Bypass::RenderSampleEffect(float sample, float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope) {
    return sample;
}

float Bypass::RenderPhaseEffect(float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope) {
    return input_phase;
}
