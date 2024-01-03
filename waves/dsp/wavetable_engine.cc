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
    current_frame_ = 0;
    target_frame_ = 0;
    swap_counter_ = 0.0f;
    swap_ = false;
}

WavetableEngine::~WavetableEngine() {
    
}

void WavetableEngine::Init() {
    phase_ = 0.0f;
    current_frame_ = 0;
    target_frame_ = 0;
    swap_counter_ = 0.0f;
    swap_ = false;
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
        thisX = CLAMP<float>(thisX, 0.0, 1.0);
        
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
    float index = phase * 2048.0;
    uint16_t integral = floor(index);
    float fractional = index - integral;
    uint16_t nextIntegral = (integral + 1) % 2048;

    float frame = morph * 15.0f;
    uint16_t frame_integral = floor(frame);
    float frame_fractional = frame - frame_integral;

    int16_t sample1 = 0;
    int16_t sample2 = 0;
    float sample = 0.0f;

    int16_t * buf = front_buffer_1;
    sample1 = buf[integral] + (buf[nextIntegral] - buf[integral]) * fractional;
    sample2 = buf[2048 + integral] + (buf[2048 + nextIntegral] - buf[2048 + integral]) * fractional;

    if(frame_integral > current_frame_)
        sample = sample2;
    else if(frame_integral < current_frame_)
        sample = sample1;
    else
        sample = sample1 * (1.0f - frame_fractional) + sample2 * frame_fractional;

    // float sample = GetSampleBetweenFrames(phase, morph);
    return sample;
}

void WavetableEngine::triggerUpdate() {
    phase_ = 0.0f;

    float morph = adc.getChannelProcessed(3);

    morph = morph * 1.0 / 4095.0;

    morph = CLAMP<float>(morph, 0.0, 0.9999);

    float frame = morph * 15.0f;
    uint16_t frame_integral = floor(frame);
    float frame_fractional = frame - frame_integral;

    current_frame_ = frame_fractional;
    target_frame_ = frame_fractional;
    swap_counter_ = 0.0f;
    swap_ = false;
    target_frame_ = 0;
    flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, target_frame_ * 4096, WavetableEngine::on_load_finished);
}

void WavetableEngine::on_load_finished() {
    wavetableEngine.swap_ = true;
}


void WavetableEngine::Render(AudioDac::Frame* output, uint32_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph)
{
        loading = 58;

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
    
    float swap_increment = 1.0f / 10000.0f;

    // float note = (120.0f * tune_interpolator.Next()) / 4095.0;
    float note = tune_interpolator.Next() * user_settings.getCalibrationX() + user_settings.getCalibrationY();
    note = CLAMP<float>(note, 0.0f, 120.0f);

    note = quantizer.Quantize(note);

    note = note - 24.0f;
    float a = 440; //frequency of A (coomon value is 440Hz)
    float frequency = (a / 32) * pow(2, ((note - 9) / 12.0));
    float phaseIncrement = frequency / 48000.0f;

    ParameterInterpolator phase_increment_interpolator(&phase_increment_, phaseIncrement, size);

    while (size--) {
        
        float interpolated_morph = morph_interpolator.Next();
        interpolated_morph = CLAMP<float>(interpolated_morph, 0.0, 0.9999);

        float phase_increment = phase_increment_interpolator.Next();

        float frame = interpolated_morph * 15.0f;
        uint16_t frame_integral = floor(frame);
        float frame_fractional = frame - frame_integral;
        
        if(frame_integral != current_frame_) {
            target_frame_ = frame_integral;
            flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, target_frame_ * 4096, WavetableEngine::on_load_finished);
        }

        for (size_t j = 0; j < kOversampling; ++j) {
            // float sample = GetSampleBetweenFrames(effect_manager.RenderPhaseEffect(phase_, frequency, fx_amount, fx, false, true), interpolated_morph);
            
            float sample = 0.0f;

            float phase = effect_manager.RenderPhaseEffect(phase_, 1 / phase_increment, fx_amount, fx, false, true);

            float index = phase * 2048.0;
            uint16_t integral = floor(index);
            float fractional = index - integral;
            uint16_t nextIntegral = (integral + 1) % 2048;

            if(!swap_) {
              int16_t sample1 = 0;
              int16_t sample2 = 0;

              int16_t * buf = front_buffer_1;
              sample1 = buf[integral] + (buf[nextIntegral] - buf[integral]) * fractional;
              sample2 = buf[2048 + integral] + (buf[2048 + nextIntegral] - buf[2048 + integral]) * fractional;

              if(frame_integral > current_frame_)
                sample = sample2;
              else if(frame_integral < current_frame_)
                sample = sample1;
              else
                sample = sample1 * (1.0f - frame_fractional) + sample2 * frame_fractional;
            }
            else {
              int16_t sample1 = 0;
              int16_t sample2 = 0;

              int16_t * buf = front_buffer_1;
              sample1 = buf[integral] + (buf[nextIntegral] - buf[integral]) * fractional;
              sample2 = buf[2048 + integral] + (buf[2048 + nextIntegral] - buf[2048 + integral]) * fractional;

              if(target_frame_ > current_frame_)
                sample1 = sample2;
              else if(current_frame_ > target_frame_)
                sample1 = sample1;
              else
                sample1 = sample1 * (1.0f - frame_fractional) + sample2 * frame_fractional;

              int16_t sample3 = 0;
              int16_t sample4 = 0;

              buf = back_buffer_1;
              sample3 = buf[integral] + (buf[nextIntegral] - buf[integral]) * fractional;
              sample4 = buf[2048 + integral] + (buf[2048 + nextIntegral] - buf[2048 + integral]) * fractional;

              sample3 = sample3 * (1.0f - frame_fractional) + sample4 * frame_fractional;

              sample = sample1 * (1.0f - swap_counter_) + sample3 * swap_counter_;
            }

            sample = effect_manager.RenderSampleEffect(sample, phase_, 1 / phase_increment, fx_amount, fx, false, true);
            
            phase_ += phase_increment;
            
            if(phase_ >= 1.0f)
                phase_ -= 1.0f;
            

            if(swap_) {
              swap_counter_ += swap_increment;
              if(swap_counter_ >= 1.0f) {
                swap_ = false;
                int16_t * temp_buffer = front_buffer_1;
                front_buffer_1 = back_buffer_1;
                back_buffer_1 = temp_buffer;
                current_frame_ = target_frame_;
                swap_counter_ = 0.0f;

                SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
                SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
              }
            }
            carrier_downsampler.Accumulate(j, sample);
        }
        
        float sample = carrier_downsampler.Read();
        
        output->l = static_cast<int32_t>(26000.0f * sample);
        ++output;
    }
}

void WavetableEngine::SetWavetable(int wavetable) {
    user_settings.settings_ptr()->wavetable_engine_wavetable = CLAMP(wavetable, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
}
int WavetableEngine::GetWavetable() { return user_settings.settings_ptr()->wavetable_engine_wavetable; }
