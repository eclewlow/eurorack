/*
 ==============================================================================
 
 Engine.cpp
 Created: 9 Nov 2023 3:03:36pm
 Author:  Eugene Clewlow
 
 ==============================================================================
 */

#include "waves/dsp/Engine.h"
// #include "waves/wavetables.h"
// #include "waves/dsp/ParameterInterpolator.h"
#include "waves/Globals.h"
// #include "waves/dsp/downsampler/4x_downsampler.h"
#include "math.h"

Engine::Engine() {
    phase_ = 0.0f;
    started_ = false;
}


// float Engine::GetSample(float phase) {
//     float index = phase * 2048.0;
//     uint16_t integral = floor(index);
//     float fractional = index - integral;
    
//     uint16_t nextIntegral = (integral + 1) % 2048;
    
//     float sample = storage.LoadWaveSample(wavetable, frame, integral);
//     float next_sample = storage.LoadWaveSample(wavetable, frame, nextIntegral);
//     float interpolated16 = sample + (next_sample - sample) * fractional;
    
//     float interpolatedFloat = interpolated16 / 32768.0f;
    
//     return interpolatedFloat;
// }

