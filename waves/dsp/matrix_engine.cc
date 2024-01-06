/*
  ==============================================================================

    matrix_engine.cpp
    Created: 19 Nov 2023 3:53:03am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/Engine.h"
#include "waves/wavetables.h"
// #include "waves/dsp/ParameterInterpolator.h"
#include "waves/Globals.h"
#include "waves/dsp/downsampler/4x_downsampler.h"

MatrixEngine::MatrixEngine() {
    phase_ = 0;
}

MatrixEngine::~MatrixEngine() {
    
}

void MatrixEngine::Init() {
    phase_ = 0.0f;
}


float MatrixEngine::GetSampleBetweenFrames(float phase, float morph_x, float morph_y) {
    // if x1 = 8 and x2 = 12. and morph_x = 0.5, then x1 + morph_x * (x2 - x1)
    float frame_x = GetX1() + morph_x * ( GetX2() -  GetX1());
    uint16_t frame_x_integral = floor(frame_x);
    float frame_x_fractional = frame_x - frame_x_integral;

    uint16_t next_frame_x_integral = (frame_x_integral + 1) % 16;

    float frame_y =  GetY1() + morph_y * ( GetY2() -  GetY1());
    uint16_t frame_y_integral = floor(frame_y);
    float frame_y_fractional = frame_y - frame_y_integral;

    uint16_t next_frame_y_integral = (frame_y_integral + 1) % 16;

    // TODO: get sample across 4 frames
    float frame_x1y1_sample = GetSample(GetWavelistOffset() + frame_y_integral, frame_x_integral, phase);
    float frame_x2y1_sample = GetSample(GetWavelistOffset() + frame_y_integral, next_frame_x_integral, phase);

    float frame_x1y2_sample = GetSample(GetWavelistOffset() + next_frame_y_integral, frame_x_integral, phase);
    float frame_x2y2_sample = GetSample(GetWavelistOffset() + next_frame_y_integral, next_frame_x_integral, phase);
    
    float upper_sample = frame_x1y1_sample * (1 - frame_x_fractional) + frame_x2y1_sample * frame_x_fractional;
    float lower_sample = frame_x1y2_sample * (1 - frame_x_fractional) + frame_x2y2_sample * frame_x_fractional;

    float sample = upper_sample * (1.0f - frame_y_fractional) + lower_sample * frame_y_fractional;
    return sample;
}

void MatrixEngine::FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx) {
    float frequency = 23.4375;

    float phaseIncrement = frequency / 48000.0f;
    
    float temp_phase = 0.0f;
    
    if(withFx)
        effect_manager.getEffect()->Sync_phases();

    for(int i = 0; i < 2048; i++) {
        float morph_y = morph_;
        float morph_x = fx_;
        morph_x = CLAMP<float>(morph_x, 0.0, 1.0);
        morph_y = CLAMP<float>(morph_y, 0.0, 1.0);

        float sample = GetSampleBetweenFrames(temp_phase, morph_x, morph_y);
        
        temp_phase += phaseIncrement;
        
        if(temp_phase >= 1.0f)
            temp_phase -= 1.0f;
        
        waveform[i] = static_cast<int16_t>(sample * 32767.0f);
    }
}

float MatrixEngine::GetSampleNoFX(float phase, float fx, float morph) {
    float sample = GetSampleBetweenFrames(phase, fx, morph);
    return sample;
}

void MatrixEngine::triggerUpdate() {
    phase_ = 0.0f;
}

void MatrixEngine::Render(AudioDac::Frame* output, size_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph)
{
        loading = 57;

    float morphTarget;
    float fxTarget;

    morphTarget = morph * 1.0 / 4095.0;
    fxTarget = fx * 1.0f / 4095.0f;
    
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
    
    while (size--) {
//        float note = (120.0f * tune_interpolator.Next()) / 4095.0;
        float note = tune_interpolator.Next() * settings_.calibration_x + settings_.calibration_y;
        note = CLAMP<float>(note, 0.0f, 120.0f);

        note = quantizer.Quantize(note);

        note = note - 24.0f;
        float a = 440; //frequency of A (coomon value is 440Hz)
        float frequency = (a / 32) * pow(2, ((note - 9) / 12.0));
        float phaseIncrement = frequency / 48000.0f;
        
        float interpolated_morph = morph_interpolator.Next();
        interpolated_morph = CLAMP<float>(interpolated_morph, 0.0, 1.0);

        float interpolated_fx = fx_interpolator.Next();
        interpolated_fx = CLAMP<float>(interpolated_fx, 0.0, 1.0);

        for (size_t j = 0; j < kOversampling; ++j) {
            float sample = GetSampleBetweenFrames(phase_, interpolated_fx, interpolated_morph);
            
            phase_ += phaseIncrement;
            
            if(phase_ >= 1.0f)
                phase_ -= 1.0f;
            
            carrier_downsampler.Accumulate(j, sample);
        }
        
        float sample = carrier_downsampler.Read();
        
        output->l = static_cast<int32_t>(26000.0f * sample);
        ++output;
    }
}

void MatrixEngine::SetX1(int8_t x1) { settings_.matrix_engine_x1 = x1; }
void MatrixEngine::SetY1(int8_t y1) { settings_.matrix_engine_y1 = y1; }
void MatrixEngine::SetX2(int8_t x2) { settings_.matrix_engine_x2 = x2; }
void MatrixEngine::SetY2(int8_t y2) { settings_.matrix_engine_y2 = y2; }
void MatrixEngine::IncrementX1(int8_t dx) {
    settings_.matrix_engine_x1 = CLAMP<int8_t>(settings_.matrix_engine_x1 + dx, 0, settings_.matrix_engine_x2);
}
void MatrixEngine::IncrementY1(int8_t dy) {
    settings_.matrix_engine_y1 = CLAMP<int8_t>(settings_.matrix_engine_y1 + dy, 0, settings_.matrix_engine_y2);
}
void MatrixEngine::IncrementX2(int8_t dx) {
    settings_.matrix_engine_x2 = CLAMP<int8_t>(settings_.matrix_engine_x2 + dx, settings_.matrix_engine_x1, 15);
}
void MatrixEngine::IncrementY2(int8_t dy) {
    settings_.matrix_engine_y2 = CLAMP<int8_t>(settings_.matrix_engine_y2 + dy, settings_.matrix_engine_y1, 15);
}
int8_t MatrixEngine::GetX1() { return settings_.matrix_engine_x1; }
int8_t MatrixEngine::GetY1() { return settings_.matrix_engine_y1; }
int8_t MatrixEngine::GetX2() { return settings_.matrix_engine_x2; }
int8_t MatrixEngine::GetY2() { return settings_.matrix_engine_y2; }

void MatrixEngine::SetWavelistOffset(int8_t offset) { settings_.matrix_engine_wavelist_offset = offset; }
int8_t MatrixEngine::GetWavelistOffset() { return settings_.matrix_engine_wavelist_offset; }
