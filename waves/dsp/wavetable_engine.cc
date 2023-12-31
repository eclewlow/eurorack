/*
  ==============================================================================

    wavetable_engine.cpp
    Created: 19 Nov 2023 3:52:47am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/Engine.h"
#include "waves/wavetables.h"
#include "waves/dsp/ParameterInterpolator.h"
#include "waves/Globals.h"
#include "waves/dsp/downsampler/4x_downsampler.h"

WavetableEngine::WavetableEngine() {
    phase_ = 0.0f;
}

WavetableEngine::~WavetableEngine() {
    
}

void WavetableEngine::Init() {
    phase_ = 0.0f;
}

float WavetableEngine::GetSampleBetweenFrames(float phase, float morph) {
    float frame = morph * 15.0f;
    uint16_t frame_integral = floor(frame);
    float frame_fractional = frame - frame_integral;
    
    uint16_t next_frame_integral = (frame_integral + 1) % 16;

    float frame1sample = GetSample(GetWavetable(), frame_integral, phase);
    float frame2sample = GetSample(GetWavetable(), next_frame_integral, phase);
    
    float sample = frame1sample * (1.0f - frame_fractional) + frame2sample * frame_fractional;
    return sample;
}

void WavetableEngine::FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx) {
    float frequency = 23.4375;

    float phaseIncrement = frequency / 48000.0f;
    
    float temp_phase = 0.0f;
    
    if(withFx)
        effect_manager.getEffect()->Sync_phases();

    for(int i = 0; i < 2048; i++) {
        float thisX = morph_;
        thisX = clamp(thisX, 0.0, 1.0);
        
        float calculated_phase = temp_phase;
        if(withFx)
            calculated_phase = effect_manager.RenderPhaseEffect(temp_phase, frequency, fx_amount, fx, true);
        
        float sample = GetSampleBetweenFrames(calculated_phase, thisX);
        
        if(withFx)
            sample = effect_manager.RenderSampleEffect(sample, temp_phase, frequency, fx_amount, fx, true);
        
        temp_phase += phaseIncrement;
        
        if(temp_phase >= 1.0f)
            temp_phase -= 1.0f;
        
        waveform[i] = static_cast<int16_t>(sample * 32767.0f);
    }
}

float WavetableEngine::GetSampleNoFX(float phase, float fx, float morph) {
    float sample = GetSampleBetweenFrames(phase, morph);
    return sample;
}

void WavetableEngine::triggerUpdate() {
    phase_ = 0.0f;
}

void WavetableEngine::Render(float* out, float* aux, uint32_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph)
{
    //    float target = morph;
    // convert 12 bit uint 0-4095 to 0...15 float
    float morphTarget = morph * 1.0 / 4095.0;
    //    float interpolatedFloat = interpolated16 / 32768.0f;
    float tuneTarget = static_cast<float>(tune);
    
    if(!started_) {
        started_ = true;
        morph_ = morphTarget;
        tune_ = tuneTarget;
    }
    
    ParameterInterpolator morph_interpolator(&morph_, morphTarget, size);
    ParameterInterpolator tune_interpolator(&tune_, tuneTarget, size);
    Downsampler carrier_downsampler(&carrier_fir_);
    
    while (size--) {
//        float note = (120.0f * tune_interpolator.Next()) / 4095.0;
        float note = tune_interpolator.Next() * user_settings.getCalibrationX() + user_settings.getCalibrationY();
        note = clamp(note, 0.0f, 120.0f);

        note = quantizer.Quantize(note);

        note = note - 24.0f;
        float a = 440; //frequency of A (coomon value is 440Hz)
        float frequency = (a / 32) * pow(2, ((note - 9) / 12.0));
        float phaseIncrement = frequency / 48000.0f;
        
        float interpolated_morph = morph_interpolator.Next();
        interpolated_morph = clamp(interpolated_morph, 0.0, 1.0);
        
        for (size_t j = 0; j < kOversampling; ++j) {
            float sample = GetSampleBetweenFrames(effect_manager.RenderPhaseEffect(phase_, frequency, fx_amount, fx, false, true), interpolated_morph);
            
            sample = effect_manager.RenderSampleEffect(sample, phase_, frequency, fx_amount, fx, false, true);
            
            phase_ += phaseIncrement;
            
            if(phase_ >= 1.0f)
                phase_ -= 1.0f;
            
            carrier_downsampler.Accumulate(j, sample);
        }
        
        float sample = carrier_downsampler.Read();
        
        *out++ = sample;
        *aux++ = sample;
    }
}

void WavetableEngine::SetWavetable(int wavetable) {
    user_settings.settings_ptr()->wavetable_engine_wavetable = CLAMP(wavetable, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
}
int WavetableEngine::GetWavetable() { return user_settings.settings_ptr()->wavetable_engine_wavetable; }
