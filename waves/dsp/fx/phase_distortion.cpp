/*
 ==============================================================================
 
 phase_distortion.cpp
 Created: 13 Nov 2023 6:31:26pm
 Author:  Eugene Clewlow
 
 ==============================================================================
 */

#include "waves/dsp/fx/effect.h"
#include "waves/Globals.h"

void PhaseDistortion::Init() {
    phase = 0.0f;
}

void PhaseDistortion::Reset() {
    phase = 0.0f;
}

float PhaseDistortion::GetSample(float phase) {
    float sample = sin(2 * M_PI * phase);
    return sample;
}

float PhaseDistortion::RenderSampleEffect(float sample, float input_phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {
    return sample;
}

float PhaseDistortion::RenderPhaseEffect(float input_phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {
    float amount = effect_manager.getDepth() * (fx_amount / 4095.0f);

    float adjusted_phase = 0.0f;
    float phaseIncrement = frequency / 48000.0f;
    
    float warpedPos;
    float m1, m2, b2;
    float x1 = 0.2;
    
    if(!effect_manager.getSync())
        frequency = pow(2, ((15.0 * fx) / 4095.0) - 3.0f);
    else {
        if(fx >= 2048) {
            frequency *= int(1.0f + (fx - 2048.0f) * 15.0f / 2047.0f);
        } else {
            frequency /= int(1.0f + (2047 - fx) * 15.0f / 2047.0f);
        }
    }
    
    phaseIncrement = frequency / 48000.0f;
    
    float *target_phase;
    
    if(isOscilloscope)
        target_phase = &oscilloscopePhase;
    else
        target_phase = &phase;
    
    
    switch(effect_manager.getControlType()) {
        case EffectManager::INTERNAL_MODULATOR: {
            
            float oscillator_sample = context.getEngine()->GetOscillatorSample(*target_phase, phaseIncrement);

            x1 = (amount * oscillator_sample + 1.0f) / 2.0f;
            
            *target_phase += phaseIncrement;
            if(*target_phase >= 1.0)
                *target_phase -= 1.0;
            
            break;
        }
        case EffectManager::EXTERNAL_MODULATOR:
        {
            x1 = 0.5f + amount * (1.0f * fx / 4095.0f - 0.5f);

            break;
        }
        case EffectManager::MANUAL_CONTROL:
        {
            x1 = 0.5f + amount * (1.0f * fx / 4095.0f - 0.5f);

            break;
        }
        default:
        {
            break;
        }
    }
    
    m1 = .5 / x1;
    m2 = .5 / (1.0 - x1);
    b2 = 1.0 - m2;
    
    if(input_phase < x1) {
        warpedPos = m1 * input_phase;
    } else {
        warpedPos = m2 * input_phase + b2;
    }
    adjusted_phase = warpedPos;
    
    while(adjusted_phase >= 1.0)
        adjusted_phase -= 1.0;
    while(adjusted_phase < 0.0)
        adjusted_phase += 1.0;
    
    return adjusted_phase;
    
    //    gain = 4 * volume / volume_increment
}
