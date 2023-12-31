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
    phase = 0.0f;
}

void Bypass::Reset() {
    phase = 0.0f;
}

float Bypass::RenderSampleEffect(float sample, float input_phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {
    return sample;
}

float Bypass::RenderPhaseEffect(float input_phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {
    return input_phase;
}
