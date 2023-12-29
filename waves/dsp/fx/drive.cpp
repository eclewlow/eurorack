/*
  ==============================================================================

    drive.cpp
    Created: 14 Nov 2023 4:36:43pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/fx/effect.h"
#include "waves/Globals.h"


void Drive::Init() {
    phase = 0.0f;
}

void Drive::Reset() {
    phase = 0.0f;
}

float Drive::GetSample(float phase) {
    float sample = sin(2 * M_PI * phase);
    return sample;
}

float Drive::RenderSampleEffect(float sample, float input_phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {
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
    
    
    switch(effect_manager.getControlType()) {
        case EffectManager::INTERNAL_MODULATOR: {
            
            float modulator_sample = 0.0f;
            
            modulator_sample = context.getEngine()->GetOscillatorSample(*target_phase, phaseIncrement);

            *target_phase += phaseIncrement;
            if(*target_phase >= 1.0)
                *target_phase -= 1.0;
            
            float calculated_sample = 0.0f;
            
            float gain = (modulator_sample + 1.0f) / 2.0f;
            float sample_with_gain = gain * 12.0f * sample;
            
//            if(sample_with_gain == 0.0f)
//                calculated_sample = 0.0f;
//            else if(sample_with_gain < 0.0f)
//                calculated_sample = -1 + exp(sample_with_gain);
//            else if(sample_with_gain > 0.0f)
//                calculated_sample = 1 - exp(-sample_with_gain);
            calculated_sample = tanh(sample_with_gain);

            sample = sample * (1 - amount) + amount * calculated_sample;

            
            break;
        }
        case EffectManager::EXTERNAL_MODULATOR:
        {
            float modulator_sample = fx / 4095.0;
            
            float calculated_sample = 0.0f;
            
            float gain = modulator_sample;
            float sample_with_gain = gain * 12.0f * sample;

            calculated_sample = tanh(sample_with_gain);

            sample = sample * (1 - amount) + amount * calculated_sample;

            break;
        }
        case EffectManager::MANUAL_CONTROL:
        {
            float modulator_sample = fx / 4095.0;
            
            float calculated_sample = 0.0f;
            
            float gain = modulator_sample;
            float sample_with_gain = gain * 12.0f * sample;
            
//            if(sample_with_gain == 0.0f)
//                calculated_sample = 0.0f;
//            else if(sample_with_gain < 0.0f)
//                calculated_sample = -1 + exp(sample_with_gain);
//            else if(sample_with_gain > 0.0f)
//                calculated_sample = 1 - exp(-sample_with_gain);

            calculated_sample = tanh(sample_with_gain);

            sample = sample * (1 - amount) + amount * calculated_sample;

            break;
        }
        default:
        {
            break;
        }
    }
    
    return sample;
}

float Drive::RenderPhaseEffect(float input_phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {
    return input_phase;
}
