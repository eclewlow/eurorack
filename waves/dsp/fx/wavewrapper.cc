/*
  ==============================================================================

    wavewrapper.cpp
    Created: 14 Nov 2023 4:36:35pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/fx/effect.h"
#include "waves/Globals.h"
#include "math.h"


void Wavewrapper::Init() {
    phase_ = 0.0f;
}

void Wavewrapper::Reset() {
    phase_ = 0.0f;
}

float Wavewrapper::GetSample(float phase) {
    float sample = sin(2 * M_PI * phase);
    return sample;
}

float Wavewrapper::RenderSampleEffect(float sample, float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {
    float amount = effect_manager.getDepth() * (fx_amount / 4095.0f);
    
    // float adjusted_phase = 0.0f;  // unused
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
            
            // add gain.. fx_amount * sample
            modulator_sample = 6 * sample * modulator_sample;
            //  if sample > 1.0  wrap around
            while(modulator_sample > 1.0) modulator_sample -= 2.0f;
            while(modulator_sample < -1.0) modulator_sample += 2.0f;

            sample = sample * (1 - amount) + amount * modulator_sample;

            
            break;
        }
        case EXTERNAL_MODULATOR:
        {
            float modulator_sample = 2.0 * fx / 4095.0 - 1.0;
            
            modulator_sample = 6 * sample * modulator_sample;
            
            while(modulator_sample > 1.0) modulator_sample -= 2.0f;
            while(modulator_sample < -1.0) modulator_sample += 2.0f;
            
            sample = sample * (1 - amount) + amount * modulator_sample;
            
            break;
        }
        case MANUAL_CONTROL:
        {
            float modulator_sample = 2.0 * fx / 4095.0 - 1.0;
            
            modulator_sample = 6 * sample * modulator_sample;
            
            while(modulator_sample > 1.0) modulator_sample -= 2.0f;
            while(modulator_sample < -1.0) modulator_sample += 2.0f;
            
            sample = sample * (1 - amount) + amount * modulator_sample;
            
            break;
        }
        default:
        {
            break;
        }
    }
    
    return sample;
}

float Wavewrapper::RenderPhaseEffect(float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {
    return input_phase;
}
