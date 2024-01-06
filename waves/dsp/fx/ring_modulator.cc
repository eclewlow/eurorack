/*
 ==============================================================================
 
 ring_modulator.cpp
 Created: 13 Nov 2023 6:31:18pm
 Author:  Eugene Clewlow
 
 ==============================================================================
 */

#include "waves/dsp/fx/effect.h"
#include "waves/Globals.h"
#include "math.h"


void RingModulator::Init() {
    phase_ = 0.0f;
}

void RingModulator::Reset() {
    phase_ = 0.0f;
}

float RingModulator::GetSample(float phase) {
    float sample = sin(2 * M_PI * phase);
    return sample;
}

float RingModulator::RenderSampleEffect(float sample, float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {
    float amount = effect_manager.getDepth() * (fx_amount / 4095.0f);
    
    // float adjusted_phase = 0.0f; // unused
    float frequency = 48000.0f * phase_increment;
    
    if(!effect_manager.getSync())
        frequency = pow(2, ((15.0 * fx) / 4095.0) - 3.0f);
    else {
        if(fx >= 2048) {
            frequency *= int(1.0f + (fx - 2048.0f) * 15.0f / 2047.0f);
        } else {
            frequency /= int(1.0f + (2047 - fx) * 15.0f / 2047.0f);
        }
    }
    
    phase_increment = frequency / 48000.0f;
    
    float *target_phase;
    
    if(isOscilloscope)
        target_phase = &oscilloscope_phase_;
    else
        target_phase = &phase_;
    
    
    switch(effect_manager.getControlType()) {
        case INTERNAL_MODULATOR: {
            
            float modulator_sample = 0.0f;
            
            modulator_sample = context.getEngine()->GetOscillatorSample(*target_phase, phase_increment);

            *target_phase += phase_increment;
            if(*target_phase >= 1.0)
                *target_phase -= 1.0;
            sample = sample * (1 - amount) + amount * sample * modulator_sample;
            
            break;
        }
        case EXTERNAL_MODULATOR:
        {
            float modulator_sample = 2.0 * fx / 4095.0 - 1.0;
            
            sample = sample * (1 - amount) + amount * sample * modulator_sample;
            break;
        }
        case MANUAL_CONTROL:
        {
            float modulator_sample = 2.0 * fx / 4095.0 - 1.0;
            
            sample = sample * (1 - amount) + amount * sample * modulator_sample;
            break;
        }
        default:
        {
            break;
        }
    }
    
    return sample;
}

float RingModulator::RenderPhaseEffect(float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {
    return input_phase;
}
