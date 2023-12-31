/*
  ==============================================================================

    wavefolder.cpp
    Created: 13 Nov 2023 6:31:07pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/fx/effect.h"
#include "waves/Globals.h"
#include "math.h"


void Wavefolder::Init() {
    phase = 0.0f;
}

void Wavefolder::Reset() {
    phase = 0.0f;
}

float Wavefolder::GetSample(float phase) {
    float sample = sin(2 * M_PI * phase);
    return sample;
}

float Wavefolder::RenderSampleEffect(float sample, float input_phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {
    return sample;
}

float Wavefolder::RenderPhaseEffect(float input_phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {
    float amount = effect_manager.getDepth() * (fx_amount / 4095.0f);

    float adjusted_phase = 0.0f;
    float phaseIncrement = frequency / 48000.0f;
        
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
    
    // float oscillator = 0.0f; // unused
    
    switch(effect_manager.getControlType()) {
        case EffectManager::INTERNAL_MODULATOR: {
            
            
            float oscillator_sample = context.getEngine()->GetOscillatorSample(*target_phase, phaseIncrement);

            amount = amount * oscillator_sample;
            
            adjusted_phase = 6 * context.getEngine()->GetSine(input_phase);
            
            adjusted_phase = (1 - amount) * input_phase + amount * adjusted_phase;
            
            *target_phase += phaseIncrement;
            if(*target_phase >= 1.0)
                *target_phase -= 1.0;
            
            break;
        }
        case EffectManager::EXTERNAL_MODULATOR:
        {
            amount = amount * (2.0f * fx / 4095.0f - 1.0f);
            
            adjusted_phase = 6 * context.getEngine()->GetSine(input_phase);

            adjusted_phase = (1 - amount) * input_phase + amount * adjusted_phase;

            break;
        }
        case EffectManager::MANUAL_CONTROL:
        {
            amount = amount * (2.0f * fx / 4095.0f - 1.0f);
            
            adjusted_phase = 6 * context.getEngine()->GetSine(input_phase);

            adjusted_phase = (1 - amount) * input_phase + amount * adjusted_phase;

            break;
        }
        default:
        {
            break;
        }
    }
        
    while(adjusted_phase >= 1.0)
        adjusted_phase -= 1.0;
    while(adjusted_phase < 0.0)
        adjusted_phase += 1.0;
    
    return adjusted_phase;
    
    //    gain = 4 * volume / volume_increment
}
