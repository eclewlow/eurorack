/*
 ==============================================================================
 
 phase_distortion.cpp
 Created: 13 Nov 2023 6:31:26pm
 Author:  Eugene Clewlow
 
 ==============================================================================
 */

#include "waves/dsp/fx/effect.h"
#include "waves/Globals.h"
#include "math.h"
#include "waves/dsp/dsp.h"


void PhaseDistortion::Init() {
    phase_ = 0.0f;
}

void PhaseDistortion::Reset() {
    phase_ = 0.0f;
}

float PhaseDistortion::RenderSampleEffect(float sample, float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope) {
    return sample;
}

float PhaseDistortion::RenderPhaseEffect(float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope) {

    float amount = fx_depth_ * ((float)fx_amount) / 65535.0f;

    float adjusted_phase = 0.0f;
    
    float warpedPos;
    float m1, m2, b2;
    float x1 = 0.2;
    
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
    
    
    switch(fx_control_type_) {
        case INTERNAL_MODULATOR: {
            
            float oscillator_sample = GetOscillatorSample(*target_phase, phase_increment, fx_oscillator_shape_);

            x1 = (amount * oscillator_sample + 1.0f) / 2.0f;
            
            *target_phase += phase_increment;
            if(*target_phase >= 1.0)
                *target_phase -= 1.0;
            
            break;
        }
        case EXTERNAL_MODULATOR:
        {
            x1 = 0.5f + amount * (1.0f * fx / 65535.0f - 0.5f);

            break;
        }
        case MANUAL_CONTROL:
        {
            x1 = 0.5f + amount * (1.0f * fx / 65535.0f - 0.5f);

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
