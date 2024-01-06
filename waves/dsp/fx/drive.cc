/*
  ==============================================================================

    drive.cpp
    Created: 14 Nov 2023 4:36:43pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/fx/effect.h"
#include "waves/Globals.h"
#include "math.h"
#include "waves/dsp/dsp.h"


void Drive::Init() {
    phase_ = 0.0f;
}

void Drive::Reset() {
    phase_ = 0.0f;
}

float Drive::RenderSampleEffect(float sample, float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope) {

    float amount = settings_.fx_depth * ((float)fx_amount) / 65535.0f;

    if(!settings_.fx_sync){
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
    
    
    switch(settings_.fx_control_type) {
        case INTERNAL_MODULATOR: {
            
            float modulator_sample = 0.0f;
            
            modulator_sample = GetOscillatorSample(*target_phase, phase_increment);

            *target_phase += phase_increment;
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
        case EXTERNAL_MODULATOR:
        {
            float modulator_sample = fx / 65535.0f;
            
            float calculated_sample = 0.0f;
            
            float gain = modulator_sample;
            float sample_with_gain = gain * 12.0f * sample;

            calculated_sample = tanh(sample_with_gain);

            sample = sample * (1 - amount) + amount * calculated_sample;

            break;
        }
        case MANUAL_CONTROL:
        {
            float modulator_sample = fx / 65535.0f;
            
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

float Drive::RenderPhaseEffect(float input_phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope) {
    return input_phase;
}
