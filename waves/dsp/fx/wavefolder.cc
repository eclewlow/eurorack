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
#include "waves/dsp/dsp.h"


void Wavefolder::Init() {
    phase_ = 0.0f;
}

void Wavefolder::Reset() {
    phase_ = 0.0f;
}

float Wavefolder::RenderSampleEffect(float sample, float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope) {
    return sample;
}

float Wavefolder::RenderPhaseEffect(float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope) {

    float amount = fx_depth_ * ((float)fx_amount) / 65535.0f;

    float adjusted_phase = 0.0f;
    
    if(!fx_sync_){
        float index = (fx / 65535.0f) * kSineLUTSize;
        MAKE_INTEGRAL_FRACTIONAL(index)
        float a = lut_fx_pow[index_integral];
        float b = lut_fx_pow[index_integral + 1];

        phase_increment = a + (b - a) * index_fractional;
    }
    else {
        if(fx >= 32768) {
            phase_increment *= static_cast<float>(1.0f + static_cast<uint8_t>((fx - 32768.0f) * 15.0f / 32767.0f));
        } else {
            phase_increment /= static_cast<float>(1.0f + static_cast<uint8_t>((32767 - fx) * 15.0f / 32767.0f));
        }
    }

    
    float *target_phase;
    
    if(isOscilloscope)
        target_phase = &oscilloscope_phase_;
    else
        target_phase = &phase_;
    
    // float oscillator = 0.0f; // unused
    
    switch(fx_control_type_) {
        case INTERNAL_MODULATOR: {
            
            
            float oscillator_sample = GetOscillatorSample(*target_phase, phase_increment);

            amount = amount * oscillator_sample;
            
            adjusted_phase = 6 * GetSine(input_phase);
            
            adjusted_phase = (1 - amount) * input_phase + amount * adjusted_phase;
            
            *target_phase += phase_increment;
            if(*target_phase >= 1.0)
                *target_phase -= 1.0;
            
            break;
        }
        case EXTERNAL_MODULATOR:
        {
            amount = amount * (2.0f * fx / 65535.0f - 1.0f);
            
            adjusted_phase = 6 * GetSine(input_phase);

            adjusted_phase = (1 - amount) * input_phase + amount * adjusted_phase;

            break;
        }
        case MANUAL_CONTROL:
        {
            amount = amount * (2.0f * fx / 65535.0f - 1.0f);
            
            adjusted_phase = 6 * GetSine(input_phase);

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
