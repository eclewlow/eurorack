/*
  ==============================================================================

    ab_engine.cpp
    Created: 19 Nov 2023 3:52:35am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/Engine.h"
#include "waves/wavetables.h"
// #include "waves/dsp/ParameterInterpolator.h"
#include "waves/Globals.h"
#include "math.h"
#include "waves/dsp/dsp.h"

namespace waves {

ABEngine::ABEngine() {
    phase_ = 0.0f;
    sub_phase_ = 0;
}

ABEngine::~ABEngine() {
    
}

void ABEngine::Init() {
    phase_ = 0.0f;
    sub_phase_ = 0;
    calibration_x_ = 0.001475852597848;    // don't randomize this, but save in snapshot
    calibration_y_ = 12.0f;    // don't randomize this, but save in snapshot
    left_wavetable_ = 0;
    left_frame_ = 0;
    right_wavetable_ = 0;
    right_frame_ = 0;
    is_editing_left_ = false;
    is_editing_right_ = false;
    subosc_offset_ = -24;
    subosc_detune_ = 0;
    subosc_mix_ = 0.0f;

    // SUBOSC_WAVE_SINE
    // SUBOSC_WAVE_TRIANGLE
    // SUBOSC_WAVE_SAWTOOTH
    // SUBOSC_WAVE_RAMP
    // SUBOSC_WAVE_SQUARE
    // SUBOSC_WAVE_COPY
    subosc_wave_ = SUBOSC_WAVE_TRIANGLE;
    fx_depth_ = 1.0f;
    fx_sync_ = false;
    fx_scale_ = 0;
    fx_range_ = 1;
    fx_oscillator_shape_ = SINE_SHAPE;
    fx_control_type_ = INTERNAL_MODULATOR;
    fx_effect_ = EFFECT_TYPE_FM;
}

float ABEngine::GetSample(int16_t wavetable, int16_t frame, float phase, bool isLeft) {
    float sample = 0;
    float next_sample = 0;
    float interpolated16 = 0;
    float interpolatedFloat = 0;

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
    interpolatedFloat = interpolated16 / 32768.0f;
    

    // interpolated16 = sample;
    // interpolatedFloat = interpolated16 / 32768.0f;
    // interpolatedFloat += 0;
    // sample += 0;
    // interpolatedFloat = sample;
    return interpolatedFloat;
    // return interpolated16;
}

float ABEngine::GetSampleBetweenFrames(float phase, float morph) {
    float frame1sample = GetSample(GetLeftWavetable(), GetLeftFrame(), phase, true);
    float frame2sample = GetSample(GetRightWavetable(), GetRightFrame(), phase, false);
    
    float sample = frame1sample * (1.0f - morph) + frame2sample * morph;
    return sample;
}

void ABEngine::FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx) {
    // float frequency = 23.4375;

    // float phaseIncrement = frequency / 48000.0f;
    
    // float temp_phase = 0.0f;
    
    // if(withFx)
    //     effect_manager.getEffect()->Sync_phases();

    // for(int i = 0; i < 2048; i++) {
    //     float thisX = morph_;
    //     thisX = CLAMP<float>(thisX, 0.0, 1.0);
        
    //     float calculated_phase = temp_phase;
    //     if(withFx)
    //         calculated_phase = effect_manager.RenderPhaseEffect(temp_phase, frequency, fx_amount, fx, true);
        
    //     float sample = GetSampleBetweenFrames(calculated_phase, thisX);
        
    //     if(withFx)
    //         sample = effect_manager.RenderSampleEffect(sample, temp_phase, frequency, fx_amount, fx, true);
        
    //     temp_phase += phaseIncrement;
        
    //     if(temp_phase >= 1.0f)
    //         temp_phase -= 1.0f;
        
    //     waveform[i] = static_cast<int16_t>(sample * 32767.0f);
    // }
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
    flash.StartFrameDMARead((uint32_t*)&back_buffer_1[2048], 4096, 15 * 4096, ABEngine::on_load_both_ab_right_finished);
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
    flash.StartFrameDMARead((uint32_t*)back_buffer_1, 4096, 0 * 4096, ABEngine::on_load_both_ab_left_finished);
 }

void ABEngine::Render(AudioDac::Frame* output, size_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph)
{
    // if(GetFlag(&_EREG_, _BUSY_)) {
    //     return;
    // }
    // convert 12 bit uint 0-4095 to 0...15 float
    float morphTarget = morph / 65535.0f;
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
    Downsampler sub_carrier_downsampler(&sub_carrier_fir_);


    // TODO:  interpolate phase_increment instead of tune.  pass in phase increment to effects-functions instead of frequency.
    // float note = (120.0f * tuneTarget) / 65535.0;
    float note = tuneTarget * calibration_x_ + calibration_y_;
    note = CLAMP<float>(note, 0.0f, 120.0f);

    note = quantizer.Quantize(note);

    note = note - 24.0f;
    // Effect * effect;
    // switch(settings_.fx_effect) {
    // case 0:
    //     effect = &bypass;
    //     break;
    // case 1:
    //     effect = &fm;
    //     break;
    // case 2:
    //     effect = &ring_modulator;
    //     break;
    // case 3:
    //     effect = &phase_distortion;
    //     break;
    // case 4:
    //     effect = &wavefolder;
    //     break;
    // case 5:
    //     effect = &wavewrapper;
    //     break;
    // case 6:
    //     effect = &bitcrush;
    //     break;
    // case 7:
    //     effect = &drive;
    //     break;
    // }
    float head = head_;
    float tail = 0.0f;
    float sub_head = sub_head_;
    float sub_tail = 0.0f;

    ParameterInterpolator phase_increment_interpolator(&phase_increment_, NoteToFrequency(note), size);
    ParameterInterpolator sub_phase_increment_interpolator(&sub_phase_increment_, NoteToFrequency((note + subosc_detune_ / 100.0f + subosc_offset_)), size);

    float sub_sample = 0;
    bool isOscilloscope = false;

    while (size--) {

        float phase = 0;
        float sample = 0;
        
        float interpolated_morph = morph_interpolator.Next();
        interpolated_morph = CLAMP<float>(interpolated_morph, 0.0, 0.9999);

        float phase_increment = phase_increment_interpolator.Next();
        phase_increment = CLAMP<float>(phase_increment, 0.0f, 1.0f);
        float sub_phase_increment = sub_phase_increment_interpolator.Next();
        sub_phase_increment = CLAMP<float>(sub_phase_increment, 0.0f, 1.0f);

        for (uint8_t j = 0; j < kOversampling; ++j) {

            switch(fx_effect_) {//settings_.fx_effect) {
                case EFFECT_TYPE_BYPASS:
                    phase = bypass.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);

                    sample = bypass.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
                case EFFECT_TYPE_FM:
                    phase = fm.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);

                    sample = fm.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
                case EFFECT_TYPE_RING_MODULATOR:
                    phase = ring_modulator.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);

                    sample = ring_modulator.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
                case EFFECT_TYPE_PHASE_DISTORTION:
                    phase = phase_distortion.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);

                    sample = phase_distortion.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
                case EFFECT_TYPE_WAVEFOLDER:
                    phase = wavefolder.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);

                    sample = wavefolder.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
                case EFFECT_TYPE_WAVEWRAPPER:
                    phase = wavewrapper.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);

                    sample = wavewrapper.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
                case EFFECT_TYPE_BITCRUSH:
                    phase = bitcrush.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);

                    sample = bitcrush.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
                case EFFECT_TYPE_DRIVE:
                    phase = drive.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);

                    sample = drive.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
            }

            switch(subosc_wave_){//settings_.subosc_wave) {
                case SUBOSC_WAVE_SINE:
                    sub_sample = GetSine(sub_phase_);
                    break;
                case SUBOSC_WAVE_TRIANGLE:
                    sub_sample = GetTriangle(sub_phase_);
                    break;
                case SUBOSC_WAVE_SAWTOOTH:
                    sub_sample = GetSawtooth(sub_phase_, sub_phase_increment);
                    break;
                case SUBOSC_WAVE_RAMP:
                    sub_sample = GetRamp(sub_phase_, sub_phase_increment);
                    break;
                case SUBOSC_WAVE_SQUARE:
                    sub_sample = GetSquare(sub_phase_, sub_phase_increment);
                    break;
                case SUBOSC_WAVE_COPY:
                    sub_sample = GetSampleBetweenFrames(sub_phase_, interpolated_morph);
                    break;
            }

            // if(settings_.subosc_wave == SUBOSC_WAVE_SINE) {
            //     sub_sample = GetSine(sub_phase_);
            // }
            // else if(settings_.subosc_wave == SUBOSC_WAVE_TRIANGLE) {
            //     sub_sample = GetTriangle(sub_phase_);
            // }
            // else if(settings_.subosc_wave == SUBOSC_WAVE_SAWTOOTH) {
            //     sub_sample = GetSawtooth(sub_phase_, sub_phase_increment);
            // }
            // else if(settings_.subosc_wave == SUBOSC_WAVE_RAMP) {
            //     sub_sample = GetRamp(sub_phase_, sub_phase_increment);
            // }
            // else if(settings_.subosc_wave == SUBOSC_WAVE_SQUARE) {
            //     sub_sample = GetSquare(sub_phase_, sub_phase_increment);
            // }
            // else if(settings_.subosc_wave == SUBOSC_WAVE_COPY) {
            //     sub_sample = GetSampleBetweenFrames(sub_phase_, interpolated_morph);
            // }            

            // sub_sample = settings_.subosc_mix * sample + (1.0f - settings_.subosc_mix) * sub_sample;

            phase_ += phase_increment;
            sub_phase_ += sub_phase_increment;
            
            if(phase_ >= 1.0f)
                phase_ -= 1.0f;

            if(sub_phase_ >= 1.0f)
                sub_phase_ -= 1.0f;
            
            // Accumulate(j, sample);
            // float test[4] = {
            //   0.02442415000,  0.09297315000,  0.1671293800,  0.2154733200,
            // };
            // const float lut_4x_downsampler_fir[] = {
            //    2.442415000e-02,  9.297315000e-02,  1.671293800e-01,  2.154733200e-01,
            // };

            head += sample * lut_4x_downsampler_fir[3 - (j & 3)];
            tail += sample * lut_4x_downsampler_fir[j & 3];
            // head += 0.0f;
            // tail += 0.0f;
            sub_head += sub_sample * lut_4x_downsampler_fir[3 - (j & 3)];
            sub_tail += sub_sample * lut_4x_downsampler_fir[j & 3];

            // carrier_downsampler.Accumulate(j, sample);
            // sub_carrier_downsampler.Accumulate(j, sub_sample);
            // sub_sample += 0;
        }
        
        // float test = head;
        // sample = head;
        // float test = sub_carrier_downsampler.Read();
        // test += 0.0f;
        // loading = front_buffer_1[3];
        float carrier_output = head; //carrier_downsampler.Read();
        float sub_output = sub_head; //sub_carrier_downsampler.Read();
        sub_output += 0;
        carrier_output += 0;
        // loading = carrier_output;
        output->l = static_cast<int16_t>(carrier_output * 26000.0f);
        output->r = static_cast<int16_t>(sub_output * 26000.0f);
        // output->r = static_cast<int16_t>(sub_carrier_downsampler.Read() * 26000.0f);
        // sub_sample * 26000.0f
        ++output;

        head = tail;
        tail = 0.0f;
        sub_head = sub_tail;
        sub_tail = 0.0f;
    }
    head_ = head;
    sub_head_ = sub_head;
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
    left_wavetable_ = CLAMP(left_wavetable, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
}
void ABEngine::SetLeftFrame(int left_frame) {
    left_frame_ = CLAMP(left_frame, 0, 15);
}
int ABEngine::GetLeftWavetable() { return left_wavetable_; }
int ABEngine::GetLeftFrame() { return left_frame_; }
void ABEngine::SetRightWavetable(int right_wavetable)
{
    right_wavetable_ = CLAMP(right_wavetable, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
}
void ABEngine::SetRightFrame(int right_frame) { right_frame_ = CLAMP(right_frame, 0, 15); }
int ABEngine::GetRightWavetable() { return right_wavetable_; }
int ABEngine::GetRightFrame() { return right_frame_; }
bool ABEngine::IsEditingLeft() { return is_editing_left_; }
bool ABEngine::IsEditingRight() { return is_editing_right_; }
void ABEngine::SetIsEditingLeft(bool is_editing_left) { is_editing_left_ = is_editing_left; }
void ABEngine::SetIsEditingRight(bool is_editing_right) { is_editing_right_ = is_editing_right; }

}