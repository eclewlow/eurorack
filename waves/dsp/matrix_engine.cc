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
#include "waves/dsp/dsp.h"

MatrixEngine::MatrixEngine() {
    phase_ = 0;
    sub_phase_ = 0;

    current_frame_x = 0;
    target_frame_x = 0;
    current_frame_y = 0;
    target_frame_y = 0;
}

MatrixEngine::~MatrixEngine() {
    
}

void MatrixEngine::Init() {
    phase_ = 0;
    sub_phase_ = 0;

    current_frame_x = 0;
    target_frame_x = 0;
    current_frame_y = 0;
    target_frame_y = 0;
}

float MatrixEngine::GetSample(int16_t wavetable, int16_t frame, float phase) {
    float sample = 0;
    float next_sample = 0;
    float interpolated16 = 0;
    float interpolatedFloat = 0;

    float index = phase * 2048.0;
    uint16_t integral = floor(index);
    float fractional = index - integral;
    
    uint16_t nextIntegral = (integral + 1) % 2048;
    
    int16_t * buffer;

    buffer = (wavetable == 0 ? front_buffer_1 : front_buffer_2);

    sample = buffer[frame * 2048 + integral];
    next_sample = buffer[frame * 2048 + nextIntegral];
    
    interpolated16 = sample + (next_sample - sample) * fractional;
    interpolatedFloat = interpolated16 / 32768.0f;

    return interpolatedFloat;

}


void MatrixEngine::on_load_1_finished() {

    // x morphs between frames;
    // y morphs between wavetables  morph
    
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    flash.StartFrameDMARead((uint32_t*)back_buffer_2, 8192, (matrixEngine.target_frame_y + 1) * 65536 + (matrixEngine.target_frame_x) * 4096, MatrixEngine::on_load_2_finished);
}

void MatrixEngine::on_load_2_finished() {
    int16_t * temp_buffer = front_buffer_1;
    front_buffer_1 = back_buffer_1;
    back_buffer_1 = temp_buffer;

    temp_buffer = front_buffer_2;
    front_buffer_2 = back_buffer_2;
    back_buffer_2 = temp_buffer;

    matrixEngine.current_frame_x = matrixEngine.target_frame_x;
    matrixEngine.current_frame_y = matrixEngine.target_frame_y;

    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}

float MatrixEngine::GetSampleBetweenFrames(float phase, float morph_x, float morph_y) {
    // if x1 = 8 and x2 = 12. and morph_x = 0.5, then x1 + morph_x * (x2 - x1)

    // y sits between two wavetable numbers
    // x sists between two wavetable frames
    float frame_x = GetX1() + morph_x * ( GetX2() -  GetX1());
    uint16_t frame_x_integral = floor(frame_x);
    float frame_x_fractional = frame_x - frame_x_integral;

    // uint16_t next_frame_x_integral = (frame_x_integral + 1) % 16;

    float frame_y =  GetY1() + morph_y * ( GetY2() -  GetY1());
    uint16_t frame_y_integral = floor(frame_y);
    float frame_y_fractional = frame_y - frame_y_integral;

    // uint16_t next_frame_y_integral = (frame_y_integral + 1) % 16;

    if((frame_y_integral != current_frame_y || frame_x_integral != current_frame_x) && !GetFlag(&_EREG_, _BUSY_) && !GetFlag(&_EREG_, _RXNE_)) {
        target_frame_y = frame_y_integral;
        target_frame_x = frame_x_integral;
        // flash.StopDMA(true);
        flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, (target_frame_y) * 65536 + (target_frame_x) * 4096, MatrixEngine::on_load_1_finished);
    }    

    // TODO: get sample across 4 frames
    float frame_x1y1_sample = GetSample(0, 0, phase);
    float frame_x2y1_sample = GetSample(0, 1, phase);

    float frame_x1y2_sample = GetSample(1, 0, phase);
    float frame_x2y2_sample = GetSample(1, 1, phase);

    float upper_sample;
    float lower_sample;
    float sample;
    
    if(frame_x_integral > current_frame_x)
        upper_sample = frame_x2y1_sample;
    else if(frame_x_integral < current_frame_x)
        upper_sample = frame_x1y1_sample;
    else
        upper_sample = frame_x1y1_sample * (1 - frame_x_fractional) + frame_x2y1_sample * frame_x_fractional;

    if(frame_x_integral > current_frame_x)
        lower_sample = frame_x2y2_sample;
    else if(frame_x_integral < current_frame_x)
        lower_sample = frame_x1y2_sample;
    else
        lower_sample = frame_x1y2_sample * (1 - frame_x_fractional) + frame_x2y2_sample * frame_x_fractional;

    if(frame_y_integral > current_frame_y)
        sample = lower_sample;
    else if(frame_y_integral < current_frame_y)
        sample = upper_sample;
    else
        sample = upper_sample * (1.0f - frame_y_fractional) + lower_sample * frame_y_fractional;

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

    current_frame_x = 0;
    current_frame_y = 0;
    target_frame_y = 0;
    target_frame_x = 0;
    flash.StopDMA(true);
    flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, (target_frame_y) * 65536 + (target_frame_x) * 4096, MatrixEngine::on_load_1_finished);

    // current_frame_ = 0;
    // target_frame_ = 0;
    // // swap_counter_ = 0.0f;
    // // swap_ = false;
    // // target_frame_ = 0;
    // flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, target_frame_ * 4096, WavetableEngine::on_load_finished);
}

void MatrixEngine::Render(AudioDac::Frame* output, size_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph)
{
    float morphTarget = morph / 65535.0;
    float fxTarget = fx / 65535.0;

    float tuneTarget = static_cast<float>(tune);
    
    if(!started_) {
        started_ = true;
        morph_ = morphTarget;
        tune_ = tuneTarget;
        fx_ = fxTarget;
    }

    ParameterInterpolator morph_interpolator(&morph_, morphTarget, size);
    ParameterInterpolator fx_interpolator(&fx_, fxTarget, size);
    // ParameterInterpolator tune_interpolator(&tune_, tuneTarget, size);
    Downsampler carrier_downsampler(&carrier_fir_);

    // float note = (120.0f * tune_interpolator.Next()) / 4095.0;
    float note = tuneTarget * settings_.calibration_x + settings_.calibration_y;
    note = CLAMP<float>(note, 0.0f, 120.0f);

    note = quantizer.Quantize(note);

    // note = note - 24.0f;

    ParameterInterpolator phase_increment_interpolator(&phase_increment_, NoteToFrequency(note), size);
    ParameterInterpolator sub_phase_increment_interpolator(&sub_phase_increment_, NoteToFrequency((note + settings_.subosc_detune / 100.0f + settings_.subosc_offset)), size);
    
    // float phase = 0;
    float sample = 0;
    float sub_sample = 0;
    // bool isOscilloscope = false;

    while (size--) {
        
        float interpolated_morph = morph_interpolator.Next();
        interpolated_morph = CLAMP<float>(interpolated_morph, 0.0, 0.9999);

        float interpolated_fx = fx_interpolator.Next();
        interpolated_fx = CLAMP<float>(interpolated_fx, 0.0, 0.9999);

        float phase_increment = phase_increment_interpolator.Next();
        phase_increment = CLAMP<float>(phase_increment, 0.0f, 1.0f);
        float sub_phase_increment = sub_phase_increment_interpolator.Next();
        sub_phase_increment = CLAMP<float>(sub_phase_increment, 0.0f, 1.0f);


        // for (size_t j = 0; j < kOversampling; ++j) {
            // float sample = GetSampleBetweenFrames(phase_, interpolated_fx, interpolated_morph);
            sample = GetSampleBetweenFrames(phase_, interpolated_fx, interpolated_morph);

            if(settings_.subosc_wave == SUBOSC_WAVE_SINE) {
                sub_sample = GetSine(sub_phase_);
            }
            else if(settings_.subosc_wave == SUBOSC_WAVE_TRIANGLE) {
                sub_sample = GetTriangle(sub_phase_);
            }
            else if(settings_.subosc_wave == SUBOSC_WAVE_SAWTOOTH) {
                sub_sample = GetSawtooth(sub_phase_, sub_phase_increment);
            }
            else if(settings_.subosc_wave == SUBOSC_WAVE_RAMP) {
                sub_sample = GetRamp(sub_phase_, sub_phase_increment);
            }
            else if(settings_.subosc_wave == SUBOSC_WAVE_SQUARE) {
                sub_sample = GetSquare(sub_phase_, sub_phase_increment);
            }
            else if(settings_.subosc_wave == SUBOSC_WAVE_COPY) {
                sub_sample = GetSampleBetweenFrames(sub_phase_, interpolated_fx, interpolated_morph);
            }

            sub_sample = settings_.subosc_mix * sample + (1.0f - settings_.subosc_mix) * sub_sample;

            phase_ += phase_increment;
            sub_phase_ += sub_phase_increment;
            
            if(phase_ >= 1.0f)
                phase_ -= 1.0f;

            if(sub_phase_ >= 1.0f)
                sub_phase_ -= 1.0f;
            
            // carrier_downsampler.Accumulate(j, sample);
        // }
        
        // float sample = carrier_downsampler.Read();
        
        output->l = sample * 26000.0f;
        output->r = sub_sample * 26000.0f;
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
