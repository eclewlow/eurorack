/*
  ==============================================================================

    bitcrusher.cpp
    Created: 14 Nov 2023 4:36:59pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/fx/effect.h"
#include "waves/Globals.h"
#include "math.h"
#include "waves/dsp/dsp.h"

void Bitcrush::Init() {
    phase_ = 0.0f;
}

void Bitcrush::Reset() {
    phase_ = 0.0f;
}

float Bitcrush::RenderSampleEffect(float sample, float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope) {

    float amount = fx_depth_ * ((float)fx_amount) / 65535.0f;
    
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
            
            float modulator_sample = 0.0f;
            
            modulator_sample = GetOscillatorSample(*target_phase, phase_increment);
            
            *target_phase += phase_increment;
            if(*target_phase >= 1.0)
                *target_phase -= 1.0;
            
            // amount of bitcrush fx
            // reduce sample
            // -1 to 1
            // round to nearest multiple of 0.25
            // 0.75  / 0.25 = 3
            //
            // // 1 means low bit rate.  0.001 means high bit rate
            float bit_rate_divisor = CLAMP((modulator_sample + 1.0f) / 2.0f, 0.001f, 1.0f);
            // bit rate = 1 / divisor
            float calculated_sample = static_cast<int>(sample / bit_rate_divisor) * bit_rate_divisor;

            sample = sample * (1 - amount) + amount * calculated_sample;

            
            break;
        }
        case EXTERNAL_MODULATOR:
        {
            float modulator_sample = fx / 65535.0f;
            
            float bit_rate_divisor = CLAMP(modulator_sample, 0.001f, 1.0f);

            float calculated_sample = static_cast<int>(sample / bit_rate_divisor) * bit_rate_divisor;

            sample = sample * (1 - amount) + amount * calculated_sample;

            break;
        }
        case MANUAL_CONTROL:
        {
            float modulator_sample = fx / 65535.0f;
            
            float bit_rate_divisor = CLAMP(modulator_sample, 0.001f, 1.0f);

            float calculated_sample = static_cast<int>(sample / bit_rate_divisor) * bit_rate_divisor;

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

float Bitcrush::RenderPhaseEffect(float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope) {
    return input_phase;
}
