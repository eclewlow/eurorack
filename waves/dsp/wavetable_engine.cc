/*
  ==============================================================================

    wavetable_engine.cpp
    Created: 19 Nov 2023 3:52:47am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/Engine.h"
#include "waves/wavetables.h"
// #include "waves/dsp/ParameterInterpolator.h"
#include "waves/Globals.h"
#include "waves/dsp/downsampler/4x_downsampler.h"
#include "waves/dsp/dsp.h"

WavetableEngine::WavetableEngine() {
    phase_ = 0.0f;
    sub_phase_ = 0;

    current_frame_ = 0;
    target_frame_ = 0;
    swap_counter_ = 0.0f;
    swap_ = false;
}

WavetableEngine::~WavetableEngine() {
    
}

void WavetableEngine::Init() {
    phase_ = 0.0f;
    sub_phase_ = 0;

    current_frame_ = 0;
    target_frame_ = 0;
    swap_counter_ = 0.0f;
    swap_ = false;
}

inline float WavetableEngine::GetSampleBetweenFrames(float phase, float morph, bool swap) {

    float sample = 0.0f;

    int16_t sample1 = 0;
    int16_t sample2 = 0;

    float index = phase * 2048.0;
    uint16_t integral = floor(index);
    float fractional = index - integral;
    uint16_t nextIntegral = (integral + 1) % 2048;

    float frame = morph * 15.0f;
    uint16_t frame_integral = floor(frame);
    float frame_fractional = frame - frame_integral;
    
    if(!GetFlag(&_EREG_, _BUSY_) && !GetFlag(&_EREG_, _RXNE_)) {

        if(frame_integral != current_frame_) {

            // target_frame_ = frame_integral;

            if(frame_integral == buffered_frame_) {
                int16_t * temp_buffer = front_buffer_1;
                front_buffer_1 = back_buffer_1;
                back_buffer_1 = temp_buffer;

                int temp_frame = current_frame_;
                current_frame_ = buffered_frame_;
                buffered_frame_= temp_frame;
            } else {
                target_frame_ = frame_integral;
                // flash.StopDMA(true);
                flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, target_frame_ * 4096, WavetableEngine::on_load_finished);
            }
        }
        /* check fractional */
        // >= 0.5 is closer to 1. < 0.5 is closer to 0
        if(frame_fractional >= 0.5 && buffered_frame_ != current_frame_ + 1 && current_frame_ != 14) {
            // buffer frame
            target_frame_ = current_frame_ + 1;
            flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, target_frame_ * 4096, WavetableEngine::on_load_finished);

        } else if (frame_fractional < 0.5 && buffered_frame_ != current_frame_ - 1 && current_frame_ != 0) {
            // buffer frame
            target_frame_ = current_frame_ - 1;
            flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, target_frame_ * 4096, WavetableEngine::on_load_finished);

        }
    }

    int16_t * buf = front_buffer_1;
    sample1 = buf[integral] + (buf[nextIntegral] - buf[integral]) * fractional;
    sample2 = buf[2048 + integral] + (buf[2048 + nextIntegral] - buf[2048 + integral]) * fractional;

    if(frame_integral > current_frame_)
        sample = sample2;
    else if(frame_integral < current_frame_)
        sample = sample1;
    else
        sample = sample1 * (1.0f - frame_fractional) + sample2 * frame_fractional;

    sample = sample / 32768.0f;
    // float frame = morph * 15.0f;
    // uint16_t frame_integral = floor(frame);
    // float frame_fractional = frame - frame_integral;
    
    // uint16_t next_frame_integral = (frame_integral + 1) % 16;

    // float frame1sample = GetSample(GetWavetable(), frame_integral, phase);
    // float frame2sample = GetSample(GetWavetable(), next_frame_integral, phase);
    
    // float sample = frame1sample * (1.0f - frame_fractional) + frame2sample * frame_fractional;
    return sample;
}

void WavetableEngine::FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx) {
    float frequency = 23.4375;

    float phaseIncrement = frequency / 48000.0f;
    
    float temp_phase = 0.0f;
    
    if(withFx)
        effect_manager.getEffect()->Sync_phases();

    float morph_float = morph / 65535.0f;
    morph_float = CLAMP<float>(morph_float, 0.0, 0.9999);

    // float frame = morph * 15.0f;
    // uint16_t frame_integral = floor(frame);
    // float frame_fractional = frame - frame_integral;

    for(int i = 0; i < 2048; i++) {
        
        float calculated_phase = temp_phase;
        if(withFx)
            calculated_phase = effect_manager.RenderPhaseEffect(temp_phase, frequency, fx_amount, fx, true);
        
        float sample = GetSampleBetweenFrames(calculated_phase, morph_float);
        
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

    current_frame_ = 1;
    target_frame_ = 0;
    // swap_counter_ = 0.0f;
    // swap_ = false;
    // target_frame_ = 0;
    // buffered_frame_ = 0;
    flash.StopDMA(true);
    flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, target_frame_ * 4096, WavetableEngine::on_load_finished);
}

void WavetableEngine::on_load_finished() {
    // loading = 12;
    // wavetableEngine.swap_ = true;
    // int16_t * temp_buffer = front_buffer_1;
    // front_buffer_1 = back_buffer_1;
    // back_buffer_1 = temp_buffer;

    wavetableEngine.buffered_frame_ = wavetableEngine.target_frame_;

    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}


void WavetableEngine::Render(AudioDac::Frame* output, size_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph)
{
    float morphTarget = morph / 65535.0;
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

    // float swap_increment = 1.0f / 10000.0f;

    // float note = (120.0f * tuneTarget) / 65535.0;
    float note = tuneTarget * settings_.calibration_x + settings_.calibration_y;
    note = CLAMP<float>(note, 0.0f, 120.0f);

    note = quantizer.Quantize(note);

    // note = note - 24.0f;
    ParameterInterpolator phase_increment_interpolator(&phase_increment_, NoteToFrequency(note), size);
    ParameterInterpolator sub_phase_increment_interpolator(&sub_phase_increment_, NoteToFrequency((note + settings_.subosc_detune / 100.0f + settings_.subosc_offset)), size);

    float phase = 0;
    float sample = 0;
    // float swap_sample = 0;
    float sub_sample = 0;
    bool isOscilloscope = false;
    // bool downsampling = true;

    while (size--) {
        
        float interpolated_morph = morph_interpolator.Next();
        interpolated_morph = CLAMP<float>(interpolated_morph, 0.0, 0.9999);

        float phase_increment = phase_increment_interpolator.Next();
        phase_increment = CLAMP<float>(phase_increment, 0.0f, 1.0f);
        float sub_phase_increment = sub_phase_increment_interpolator.Next();
        sub_phase_increment = CLAMP<float>(sub_phase_increment, 0.0f, 1.0f);

        // for (size_t j = 0; j < kOversampling; ++j) {
            // float sample = GetSampleBetweenFrames(effect_manager.RenderPhaseEffect(phase_, frequency, fx_amount, fx, false, true), interpolated_morph);
            
            // float sample = 0.0f;

            // float phase = effect_manager.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false, true);
            // phase = phase_;

            // float index = phase * 2048.0;
            // uint16_t integral = floor(index);
            // float fractional = index - integral;
            // uint16_t nextIntegral = (integral + 1) % 2048;

            // if(!swap_) {
              // int16_t sample1 = 0;
              // int16_t sample2 = 0;

              // int16_t * buf = front_buffer_1;
              // sample1 = buf[integral] + (buf[nextIntegral] - buf[integral]) * fractional;
              // sample2 = buf[2048 + integral] + (buf[2048 + nextIntegral] - buf[2048 + integral]) * fractional;

              // if(frame_integral > current_frame_)
              //   sample = sample2;
              // else if(frame_integral < current_frame_)
              //   sample = sample1;
              // else
              //   sample = sample1 * (1.0f - frame_fractional) + sample2 * frame_fractional;
            
            // }
            // else {
            //   int16_t sample1 = 0;
            //   int16_t sample2 = 0;

            //   int16_t * buf = front_buffer_1;
            //   sample1 = buf[integral] + (buf[nextIntegral] - buf[integral]) * fractional;
            //   sample2 = buf[2048 + integral] + (buf[2048 + nextIntegral] - buf[2048 + integral]) * fractional;

            //   if(target_frame_ > current_frame_)
            //     sample1 = sample2;
            //   else if(current_frame_ > target_frame_)
            //     sample1 = sample1;
            //   else
            //     sample1 = sample1 * (1.0f - frame_fractional) + sample2 * frame_fractional;

            //   int16_t sample3 = 0;
            //   int16_t sample4 = 0;

            //   buf = back_buffer_1;
            //   sample3 = buf[integral] + (buf[nextIntegral] - buf[integral]) * fractional;
            //   sample4 = buf[2048 + integral] + (buf[2048 + nextIntegral] - buf[2048 + integral]) * fractional;

            //   sample3 = sample3 * (1.0f - frame_fractional) + sample4 * frame_fractional;

            //   sample = sample1 * (1.0f - swap_counter_) + sample3 * swap_counter_;
            // }

            // sample = effect_manager.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, false, true);
            
            switch(settings_.fx_effect) {
                case EFFECT_TYPE_BYPASS:
                    phase = bypass.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);
                    // swap_sample = GetSampleBetweenFrames(phase, interpolated_morph, true);

                    sample = bypass.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    // swap_sample = bypass.RenderSampleEffect(swap_sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
                case EFFECT_TYPE_FM:
                    phase = fm.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);
                    // swap_sample = GetSampleBetweenFrames(phase, interpolated_morph, true);

                    sample = fm.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    // swap_sample = fm.RenderSampleEffect(swap_sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
                case EFFECT_TYPE_RING_MODULATOR:
                    phase = ring_modulator.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);
                    // swap_sample = GetSampleBetweenFrames(phase, interpolated_morph, true);

                    sample = ring_modulator.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    // swap_sample = ring_modulator.RenderSampleEffect(swap_sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
                case EFFECT_TYPE_PHASE_DISTORTION:
                    phase = phase_distortion.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);
                    // swap_sample = GetSampleBetweenFrames(phase, interpolated_morph, true);

                    sample = phase_distortion.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    // swap_sample = phase_distortion.RenderSampleEffect(swap_sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
                case EFFECT_TYPE_WAVEFOLDER:
                    phase = wavefolder.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);
                    // swap_sample = GetSampleBetweenFrames(phase, interpolated_morph, true);

                    sample = wavefolder.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    // swap_sample = wavefolder.RenderSampleEffect(swap_sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
                case EFFECT_TYPE_WAVEWRAPPER:
                    phase = wavewrapper.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);
                    // swap_sample = GetSampleBetweenFrames(phase, interpolated_morph, true);

                    sample = wavewrapper.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    // swap_sample = wavewrapper.RenderSampleEffect(swap_sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
                case EFFECT_TYPE_BITCRUSH:
                    phase = bitcrush.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);
                    // swap_sample = GetSampleBetweenFrames(phase, interpolated_morph, true);

                    sample = bitcrush.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    // swap_sample = bitcrush.RenderSampleEffect(swap_sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
                case EFFECT_TYPE_DRIVE:
                    phase = drive.RenderPhaseEffect(phase_, phase_increment, fx_amount, fx, false);

                    sample = GetSampleBetweenFrames(phase, interpolated_morph);
                    // swap_sample = GetSampleBetweenFrames(phase, interpolated_morph, true);

                    sample = drive.RenderSampleEffect(sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    // swap_sample = drive.RenderSampleEffect(swap_sample, phase_, phase_increment, fx_amount, fx, isOscilloscope);
                    break;
            }

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
                sub_sample = GetSampleBetweenFrames(sub_phase_, interpolated_morph);
            }

            sub_sample = settings_.subosc_mix * sample + (1.0f - settings_.subosc_mix) * sub_sample;


    // int16_t * temp_buffer = front_buffer_1;
    // front_buffer_1 = back_buffer_1;
    // back_buffer_1 = temp_buffer;

    // wavetableEngine.current_frame_ = wavetableEngine.target_frame_;

    // SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    // SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);

            // loading = front_buffer_1[0];

            phase_ += phase_increment;
            sub_phase_ += sub_phase_increment;
            
            if(phase_ >= 1.0f)
                phase_ -= 1.0f;

            if(sub_phase_ >= 1.0f)
                sub_phase_ -= 1.0f;
            

            // if(swap_) {
            //   swap_counter_ += swap_increment;

            //   if(swap_counter_ >= 1.0f) {
            //     swap_ = false;
            //     int16_t * temp_buffer = front_buffer_1;
            //     front_buffer_1 = back_buffer_1;
            //     back_buffer_1 = temp_buffer;
            //     current_frame_ = target_frame_;
            //     swap_counter_ = 0.0f;

            //     SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
            //     SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
            //   }

            //   sample = sample * (1.0f - swap_counter_) + swap_sample * swap_counter_;
            // }
            // carrier_downsampler.Accumulate(j, sample);
        // }
        
        // float sample = carrier_downsampler.Read();
        
        output->l = sample * 26000.0f;
        output->r = sub_sample * 26000.0f;
        ++output;
    }
}

void WavetableEngine::SetWavetable(int wavetable) {
    settings_.wavetable_engine_wavetable = CLAMP(wavetable, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
}
int WavetableEngine::GetWavetable() { return settings_.wavetable_engine_wavetable; }
