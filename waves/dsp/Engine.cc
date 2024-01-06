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

float Engine::GetSine(float phase) {
    float sample = Sine(2 * M_PI * phase);
    return sample;
    // return 0;
}

// This function calculates the PolyBLEPs
float Engine::poly_blep(float t, float dt)
{
//    double dt = phase;
    
    // t-t^2/2 +1/2
    // 0 < t <= 1
    // discontinuities between 0 & 1
    if (t < dt)
    {
        t /= dt;
        return t + t - t * t - 1.0;
    }
    
    // t^2/2 +t +1/2
    // -1 <= t <= 0
    // discontinuities between -1 & 0
    else if (t > 1 - dt)
    {
        t = (t - 1.0) / dt;
        return t * t + t + t + 1.0;
    }
    
    // no discontinuities
    // 0 otherwise
    else return 0.0;
}

float Engine::GetRamp(float phase, float phase_increment) {
    float value = 2.0f * phase - 1.0f;
    
    if(phase + phase_increment >= 1.0f) {
        phase = phase + phase_increment - 1.0f;
        float t = phase / phase_increment;
        value -= ThisBlepSample(t);
    } else if(phase < phase_increment) {
        float t = phase / phase_increment;
        value -= NextBlepSample(t);
    }
    
    return value;
}

float Engine::GetSawtooth(float phase, float phase_increment) {
    float value = 2.0f * (1.0-phase) - 1.0f;
    
    if(phase + phase_increment >= 1.0f) {
        phase = phase + phase_increment - 1.0f;
        float t = phase / phase_increment;
        value += ThisBlepSample(t);
    } else if(phase < phase_increment) {
        float t = phase / phase_increment;
        value += NextBlepSample(t);
    }
    
    return value;
}

float Engine::GetSquare(float phase, float phase_increment) {
    float value = phase < 0.5 ? 0.0 : 1.0;
    //(2.0f * phase - 1.0f) * 1.0;
    
    if(phase + phase_increment >= 0.5f && phase < 0.5f) {
        phase = phase + phase_increment - 0.5f;
        float t = phase / phase_increment;
        value += ThisBlepSample(t);
    } else if(phase >= 0.5f && phase - phase_increment < 0.5f) {
        float t = (phase - 0.5f) / phase_increment;
        value += NextBlepSample(t);
    }

    if(phase + phase_increment >= 1.0f) {
        phase = phase + phase_increment - 1.0f;
        float t = phase / phase_increment;
        value -= ThisBlepSample(t);
    } else if(phase < phase_increment) {
        float t = phase / phase_increment;
        value -= NextBlepSample(t);
    }

    return value;
}

float Engine::GetTriangle(float phase) {
    float value = -1.0 + (2.0 * phase);
    value = 2.0 * (fabs(value) - 0.5);
    return value;
}

float Engine::GetOscillatorSample(float phase, float phase_increment) {
    if(effect_manager.getOscillatorShape() == SINE_SHAPE)
        return GetSine(phase);
    else if(effect_manager.getOscillatorShape() == TRIANGLE_SHAPE)
        return GetTriangle(phase);
    else if(effect_manager.getOscillatorShape() == SAWTOOTH_SHAPE)
        return GetSawtooth(phase, phase_increment);
    else if(effect_manager.getOscillatorShape() == RAMP_SHAPE)
        return GetRamp(phase, phase_increment);
    else if(effect_manager.getOscillatorShape() == SQUARE_SHAPE)
        return GetSquare(phase, phase_increment);
    else return 0.0f;
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

