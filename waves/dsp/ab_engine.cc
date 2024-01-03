/*
  ==============================================================================

    ab_engine.cpp
    Created: 19 Nov 2023 3:52:35am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/Engine.h"
#include "waves/wavetables.h"
#include "waves/dsp/ParameterInterpolator.h"
#include "waves/Globals.h"
#include "waves/dsp/downsampler/4x_downsampler.h"
#include "math.h"

namespace waves {

ABEngine::ABEngine() {
    phase_ = 0.0f;
}

ABEngine::~ABEngine() {
    
}

void ABEngine::Init() {
    phase_ = 0.0f;
}

float ABEngine::GetSample(int16_t wavetable, int16_t frame, float phase, bool isLeft) {
    float sample = 0;
    float next_sample = 0;
    float interpolated16 = 0;
    // float interpolatedFloat = 0;

    float index = phase * 2048.0;
    uint16_t integral = floor(index);
    float fractional = index - integral;
    
    uint16_t nextIntegral = (integral + 1) % 2048;
    
    // if(IsEditingLeft() && isLeft) {
    //     sample = BUF3[integral];
    //     next_sample = BUF3[nextIntegral];
    // } else if (IsEditingRight() && !isLeft) {
    //     sample = BUF4[integral];
    //     next_sample = BUF4[nextIntegral];
    // } else {
        if(isLeft) {
            // sample += 0;
            // next_sample += 0;
            // nextIntegral += 0;
            sample = front_buffer_1[integral];
            next_sample = front_buffer_1[nextIntegral];
        } else {
            // sample += 0;
            // next_sample += 0;
            // nextIntegral += 0;
            sample = front_buffer_1[2048 + integral];
            next_sample = front_buffer_1[2048 + nextIntegral];
        }
    // }
    
    interpolated16 = sample + (next_sample - sample) * fractional;
    // interpolatedFloat = interpolated16 / 32768.0f;
    

    // interpolated16 = sample;
    // interpolatedFloat = interpolated16 / 32768.0f;
    // interpolatedFloat += 0;
    // sample += 0;
    // interpolatedFloat = sample;
    // return interpolatedFloat;
    return interpolated16;
}

float ABEngine::GetSampleBetweenFrames(float phase, float morph) {
    float frame1sample = GetSample(GetLeftWavetable(), GetLeftFrame(), phase, true);
    float frame2sample = GetSample(GetRightWavetable(), GetRightFrame(), phase, false);
    
    float sample = frame1sample * (1.0f - morph) + frame2sample * morph;
    return sample;
}

void ABEngine::FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx) {
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

void ABEngine::FillWaveform(int16_t * waveform, bool is_left) {
    // storage.LoadWaveSample(waveform, wavetable, frame);
    if(is_left)
        memcpy(waveform, front_buffer_1, 4096);
    else
        memcpy(waveform, front_buffer_2, 4096);
}

float ABEngine::GetSampleNoFX(float phase, float fx, float morph) {
    float sample = GetSampleBetweenFrames(phase, morph);
    return sample;
}

void ABEngine::on_load_both_ab_left_finished() {
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    flash.StartFrameDMARead((uint32_t*)&back_buffer_1[2048], 4096, abEngine.GetRightFrame() * 4096, ABEngine::on_load_both_ab_right_finished);
}
void ABEngine::on_load_both_ab_right_finished() {
    int16_t * temp_buffer = front_buffer_1;
    front_buffer_1 = back_buffer_1;
    back_buffer_1 = temp_buffer;
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}
void ABEngine::on_load_one_ab_left_finished() {
    // TODO: maybe always load both.  loading only one makes it difficult, unless we copy already loaded front buffer to back buffer
}
void ABEngine::on_load_one_ab_right_finished() {
    // TODO: maybe always load both.  loading only one makes it difficult, unless we copy already loaded front buffer to back buffer
}

void ABEngine::triggerUpdate() {
    phase_ = 0.0f;
    // TODO: Load current left frame into left side of buffer, and right frame into right side of buffer
    flash.StopDMA(true);
    flash.StartFrameDMARead((uint32_t*)back_buffer_1, 4096, GetLeftFrame() * 4096, ABEngine::on_load_both_ab_left_finished);
 }

void ABEngine::Render(AudioDac::Frame* output, uint32_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph)
{
    // if(GetFlag(&_EREG_, _BUSY_)) {
    //     return;
    // }
    // convert 12 bit uint 0-4095 to 0...15 float
    float morphTarget = morph * 1.0 / 65535.0f;
    //    float interpolatedFloat = interpolated16 / 32768.0f;
    float tuneTarget = static_cast<float>(tune);
    
    if(!started_) {
        started_ = true;
        morph_ = morphTarget;
        tune_ = tuneTarget;
    }

    ParameterInterpolator morph_interpolator(&morph_, morphTarget, size);
    // ParameterInterpolator tune_interpolator(&tune_, tuneTarget, size);
    Downsampler carrier_downsampler(&carrier_fir_);


    // TODO:  interpolate phase_increment instead of tune.  pass in phase increment to effects-functions instead of frequency.
    float note = (120.0f * tuneTarget) / 65535.0;
    // float note = 48;//tuneTarget * user_settings.getCalibrationX() + user_settings.getCalibrationY();
    note = CLAMP<float>(note, 0.0f, 120.0f);

    note = quantizer.Quantize(note);

    // note = note - 24.0f;

    ParameterInterpolator phase_increment_interpolator(&phase_increment_, NoteToFrequency(note), size);

    while (size--) {
        
        float interpolated_morph = morph_interpolator.Next();
        interpolated_morph = CLAMP<float>(interpolated_morph, 0.0, 0.9999);
// //        printf("%f\n", interpolated_morph);

        float phase_increment = phase_increment_interpolator.Next();
        // phase_increment = CLAMP<float>(phase_increment, 0.0f, 1.0f);
        // phase_increment += 0;
        // for (size_t j = 0; j < kOversampling; ++j) {

            // float phase = effect_manager.RenderPhaseEffect(phase_, 1  / phase_increment, fx_amount, fx, false, true);

            // int16_t sample = GetSampleBetweenFrames(phase, interpolated_morph);

            // sample = effect_manager.RenderSampleEffect(sample, phase_, 1  / phase_increment, fx_amount, fx, false, true);
            

            float index = phase_ * 2048.0;
            uint16_t integral = floor(index);
            // float fractional = index - integral;
            
            // uint16_t nextIntegral = (integral + 1) % 2048;
            
            int16_t sample = front_buffer_1[integral];
            // int16_t next_sample = front_buffer_1[nextIntegral];

            phase_ += phase_increment;
            
            if(phase_ >= 1.0f)
                phase_ -= 1.0f;
            
            // carrier_downsampler.Accumulate(j, sample);
        // }
        
        // float sample = carrier_downsampler.Read();

            loading = front_buffer_1[3];
        output->l = sample;
        ++output;
    }
}

bool ABEngine::SetLeftWave(int table, int frame) {
    SetLeftWavetable(table);
    SetLeftFrame(frame);
    return true;
}

bool ABEngine::SetRightWave(int table, int frame) {
    SetRightWavetable(table);
    SetRightFrame(frame);
    return true;
}

void ABEngine::SetLeftWavetable(int left_wavetable)
{ 
    user_settings.settings_ptr()->ab_engine_left_wavetable = CLAMP(left_wavetable, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
}
void ABEngine::SetLeftFrame(int left_frame) {
    user_settings.settings_ptr()->ab_engine_left_frame = CLAMP(left_frame, 0, 15);
}
int ABEngine::GetLeftWavetable() { return user_settings.settings_ptr()->ab_engine_left_wavetable; }
int ABEngine::GetLeftFrame() { return user_settings.settings_ptr()->ab_engine_left_frame; }
void ABEngine::SetRightWavetable(int right_wavetable)
{
    user_settings.settings_ptr()->ab_engine_right_wavetable = CLAMP(right_wavetable, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
}
void ABEngine::SetRightFrame(int right_frame) { user_settings.settings_ptr()->ab_engine_right_frame = CLAMP(right_frame, 0, 15); }
int ABEngine::GetRightWavetable() { return user_settings.settings_ptr()->ab_engine_right_wavetable; }
int ABEngine::GetRightFrame() { return user_settings.settings_ptr()->ab_engine_right_frame; }
bool ABEngine::IsEditingLeft() { return user_settings.settings_ptr()->ab_engine_is_editing_left; }
bool ABEngine::IsEditingRight() { return user_settings.settings_ptr()->ab_engine_is_editing_right; }
void ABEngine::SetIsEditingLeft(bool is_editing_left) { user_settings.settings_ptr()->ab_engine_is_editing_left = is_editing_left; }
void ABEngine::SetIsEditingRight(bool is_editing_right) { user_settings.settings_ptr()->ab_engine_is_editing_right = is_editing_right; }

}