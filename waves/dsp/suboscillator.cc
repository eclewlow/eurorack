/*
  ==============================================================================

    suboscillator.cpp
    Created: 19 Nov 2023 4:08:34am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/Engine.h"
#include "waves/wavetables.h"
// #include "waves/dsp/ParameterInterpolator.h"
#include "waves/Globals.h"
#include "waves/dsp/downsampler/4x_downsampler.h"
#include "waves/dsp/dsp.h"

Suboscillator::Suboscillator() {
    phase_ = 0.0f;
}

Suboscillator::~Suboscillator() {
    
}

void Suboscillator::Init() {
    phase_ = 0.0f;
}

float Suboscillator::GetSampleNoFX(float phase, float fx, float morph) {
    return 0.0f;
}

void Suboscillator::FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx)
{
}

void Suboscillator::triggerUpdate() {
    phase_ = 0.0f;
}

void Suboscillator::Render(AudioDac::Frame* output, size_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph)
{
    float morphTarget;
    float fxTarget;
    
    morphTarget = morph / 65535.0;
    fxTarget = fx / 65535.0;

    float tuneTarget = static_cast<float>(tune);
    
    if(!started_) {
        started_ = true;
        morph_ = morphTarget;
        tune_ = tuneTarget;
        fx_ = fxTarget;
    }

    ParameterInterpolator morph_interpolator(&morph_, morphTarget, size);
    ParameterInterpolator fx_interpolator(&fx_, fxTarget, size);
    ParameterInterpolator tune_interpolator(&tune_, tuneTarget, size);
    Downsampler carrier_downsampler(&carrier_fir_);

   // float note = (120.0f * tune_interpolator.Next()) / 4095.0;
    float note = tuneTarget * calibration_x_ + calibration_y_;
    note = CLAMP<float>(note, 0.0f, 120.0f);

    note = quantizer.Quantize(note);

    if(engine_ == ENGINE_TYPE_DRUM) {
        note += 12 * drumEngine.GetY(drumEngine.GetFMDecayTrigger()) * (drumEngine.GetFMDepth() * 2.0f - 1.0f);
        note = CLAMP<float>(note, 0.0f, 120.0f);
    }

    note = note - 24.0f;
    float a = 440; //frequency of A (coomon value is 440Hz)
    float frequency; // = (a / 32) * pow(2, ((note - 9) / 12.0));
    float phaseIncrement; // = frequency / 48000.0f;
    
    frequency = (a / 32) * pow(2, (((note + subosc_detune_ / 100.0f + subosc_offset_) - 9) / 12.0));
    phaseIncrement = frequency / kCorrectedSampleRate;

    ParameterInterpolator phase_increment_interpolator(&phase_increment_, phaseIncrement, size);
    
    while (size--) {

        float interpolated_morph = morph_interpolator.Next();
        interpolated_morph = CLAMP<float>(interpolated_morph, 0.0, 1.0);

        float interpolated_fx = fx_interpolator.Next();
        interpolated_fx = CLAMP<float>(interpolated_fx, 0.0, 1.0);

        float phase_increment = phase_increment_interpolator.Next();

        for (size_t j = 0; j < kOversampling; ++j) {
            float sample = 0.0f;
            if(subosc_wave_ == SUBOSC_WAVE_SINE) {
                sample = GetSine(phase_);
            }
            else if(subosc_wave_ == SUBOSC_WAVE_TRIANGLE) {
                sample = GetTriangle(phase_);
            }
            else if(subosc_wave_ == SUBOSC_WAVE_SAWTOOTH) {
                sample = GetSawtooth(phase_, phase_increment);
            }
            else if(subosc_wave_ == SUBOSC_WAVE_RAMP) {
                sample = GetRamp(phase_, phase_increment);
            }
            else if(subosc_wave_ == SUBOSC_WAVE_SQUARE) {
                sample = GetSquare(phase_, phase_increment);
            }
            else if(subosc_wave_ == SUBOSC_WAVE_COPY) {
                sample = context.getEngine()->GetSampleNoFX(phase_, interpolated_fx, interpolated_morph);
            }

            if(engine_ == ENGINE_TYPE_DRUM) {
                sample = (1 - drumEngine.GetAmpDecayTrigger()) * sample;
            }

            phase_ += phase_increment;
            
            if(phase_ >= 1.0f)
                phase_ -= 1.0f;
            
            carrier_downsampler.Accumulate(j, sample);
        }
        
        float sample = carrier_downsampler.Read();
        
        output->r = static_cast<int32_t>(26000.0f * sample);
        ++output;
    }
}

