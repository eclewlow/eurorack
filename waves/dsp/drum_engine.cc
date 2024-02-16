/*
  ==============================================================================

    drum_engine.cpp
    Created: 30 Nov 2023 5:44:42pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/Engine.h"
#include "waves/wavetables.h"
// #include "waves/dsp/ParameterInterpolator.h"
#include "waves/Globals.h"
#include "waves/dsp/downsampler/4x_downsampler.h"
#include "waves/dsp/dsp.h"

DrumEngine::DrumEngine() {
    phase_ = 0.0f;
    amp_decay_trigger_ = 1.0f;
    fm_decay_trigger_ = 1.0f;
    current_frame_ = 0;
    target_frame_ = 0;
}

DrumEngine::~DrumEngine() {
    
}

void DrumEngine::Init() {
    phase_ = 0.0f;
    sub_phase_ = 0;

    amp_decay_trigger_ = 1.0f;
    fm_decay_trigger_ = 1.0f;

    current_frame_ = 0;
    target_frame_ = 0;

    wavetable_ = 0;

    calibration_x_ = 0.001475852597848;    // don't randomize this, but save in snapshot
    calibration_y_ = 12.0f;    // don't randomize this, but save in snapshot

    subosc_offset_ = -12;
    subosc_detune_ = 0;
    subosc_mix_ = 0.0f;

    // SUBOSC_WAVE_SINE
    // SUBOSC_WAVE_TRIANGLE
    // SUBOSC_WAVE_SAWTOOTH
    // SUBOSC_WAVE_RAMP
    // SUBOSC_WAVE_SQUARE
    // SUBOSC_WAVE_COPY
    subosc_wave_ = SUBOSC_WAVE_COPY;
    fx_depth_ = 1.0f;
    fx_sync_ = false;
    fx_scale_ = 0;
    fx_range_ = 1;
    fx_oscillator_shape_ = SINE_SHAPE;
    fx_control_type_ = INTERNAL_MODULATOR;
    fx_effect_ = EFFECT_TYPE_FM;

}

float DrumEngine::GetSampleBetweenFrames(float phase, float morph) {
    float sample = 0.0f;

    int16_t sample1 = 0;
    int16_t sample2 = 0;

    float index = phase * 2048.0;
    uint16_t integral = floor(index);
    float fractional = index - integral;
    uint16_t nextIntegral = (integral + 1) % 2048;

    float frame = morph * 15.0f;
    int frame_integral = floor(frame);
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
                flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, target_frame_ * 4096, DrumEngine::on_load_finished);
            }
        }
        /* check fractional */
        // >= 0.5 is closer to 1. < 0.5 is closer to 0
        else if(frame_fractional >= 0.5 && buffered_frame_ != current_frame_ + 1 && current_frame_ != 14) {
            // buffer frame
            target_frame_ = current_frame_ + 1;
            flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, target_frame_ * 4096, DrumEngine::on_load_finished);

        } else if (frame_fractional < 0.5 && buffered_frame_ != current_frame_ - 1 && current_frame_ != 0) {
            // buffer frame
            target_frame_ = current_frame_ - 1;
            flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, target_frame_ * 4096, DrumEngine::on_load_finished);
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

    return sample;
}

int16_t DrumEngine::GetSampleBetweenFramesNoDMA(float phase, float morph) {
    int16_t sample = 0;

    int16_t sample1 = 0;
    int16_t sample2 = 0;

    float index = phase * 2048.0;
    uint16_t integral = floor(index);
    float fractional = index - integral;
    uint16_t nextIntegral = (integral + 1) % 2048;

    float frame = morph * 15.0f;
    int frame_integral = floor(frame);
    float frame_fractional = frame - frame_integral;

    int16_t * buf = front_buffer_1;
    sample1 = buf[integral] + (buf[nextIntegral] - buf[integral]) * fractional;
    sample2 = buf[2048 + integral] + (buf[2048 + nextIntegral] - buf[2048 + integral]) * fractional;

    if(frame_integral > current_frame_)
        sample = sample2;
    else if(frame_integral < current_frame_)
        sample = sample1;
    else
        sample = sample1 * (1.0f - frame_fractional) + sample2 * frame_fractional;

    return sample;
}

void DrumEngine::on_load_finished() {

    drumEngine.buffered_frame_ = drumEngine.target_frame_;

    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}

void DrumEngine::FillWaveform(int16_t * waveform, float morph) {
    float frequency = 23.4375;

    float phaseIncrement = frequency / 48000.0f;
    
    float temp_phase = 0.0f;

    for(int i = 0; i < 2048; i++) {
        float thisX = morph;
        thisX = CLAMP<float>(thisX, 0.0, 0.9999);
        
        float calculated_phase = temp_phase;
        
        int16_t sample = GetSampleBetweenFramesNoDMA(calculated_phase, thisX);

        temp_phase += phaseIncrement;
        
        if(temp_phase >= 1.0f)
            temp_phase -= 1.0f;
        
        waveform[i] = sample;
    }
}

void DrumEngine::FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx) {
    float frequency = 23.4375;

    float phaseIncrement = frequency / 48000.0f;
    
    float temp_phase = 0.0f;
            
    // if(withFx)
    //     effect_manager.getEffect()->Sync_phases();

    for(int i = 0; i < 2048; i++) {
        float thisX = morph_;
        thisX = CLAMP<float>(thisX, 0.0, 1.0);
        
        float calculated_phase = temp_phase;
        // if(withFx)
        //     calculated_phase = effect_manager.RenderPhaseEffect(temp_phase, frequency, fx_amount, fx, true);
        
        float sample = GetSampleBetweenFrames(calculated_phase, thisX);
        
        // if(withFx)
        //     sample = effect_manager.RenderSampleEffect(sample, temp_phase, frequency, fx_amount, fx, true);
        
        sample = (1 - amp_decay_trigger_) * sample;

        temp_phase += phaseIncrement;
        
        if(temp_phase >= 1.0f)
            temp_phase -= 1.0f;
        
        waveform[i] = static_cast<int16_t>(sample * 32767.0f);
    }
}

float DrumEngine::GetSampleNoFX(float phase, float fx, float morph) {
    float sample = GetSampleBetweenFrames(phase, morph);
    return sample;
}

void DrumEngine::triggerUpdate() {
    phase_ = 0.0f;

    current_frame_ = 1;
    target_frame_ = 0;

    flash.StopDMA(true);
    flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, target_frame_ * 4096, DrumEngine::on_load_finished);
}

float DrumEngine::GetY(float x) {
    float curve = GetFMShape();
    float y;
    
    // curve is from 0.0 to 1.0
    float r, s;

    float gain = 40.0f;

//    if(abs(curve - 0.5) < 0.05) {
//        return 1 - x;
//    }

    if(curve > 0.5) {
        r = pow((1.0f - curve), 3) * gain + 1.0f;
    } else {
        r = pow(curve, 3) * gain + 1.0f;
    }

    s = sqrt(-4 + 8 * r * r);

    if(curve > 0.5) {
        s = (-2 + s) / 4;
    } else {
        s = (-2 - s) / 4;
    }
    
    y = (curve > 0.5 ? 1 : -1) * sqrt(r * r - (x + s) * (x + s)) - s;
    
//    float r;
//    if(curve > 0.5) {
//        r = (1.0f - curve) * 10.0f + 1.0f;
//    } else {
//        r = (curve) * 10.0f + 1.0f;
//    }
//    
//    float angle = acos(1.0f - 2.0f / (2.0f * r * r));
//    float new_angle = angle + (M_PI_2 - angle) / 2.0f;
//    float p = r * sin(new_angle) - 1.0f;
//    float q = r * cos(new_angle);
//
//    if(curve > 0.5)
//        y = sqrt(pow(r, 2) - pow(-x - q, 2)) - p;
//    else
//        y = 1.0f - sqrt(pow(r, 2) - pow(1 - x + q, 2)) + p;
    return y;
}

void DrumEngine::Render(AudioDac::Frame* output, size_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph)
{
    float morphTarget = morph / 65535.0;
    float tuneTarget = static_cast<float>(tune);
    
    uint16_t fx_amount_cv = adc.getChannel(Adc::ADC_CHANNEL_FX_AMOUNT_CV);
    
    if(fx_amount_cv > 2048) {
        if(amp_decay_trigger_ >= 1.0f && fm_decay_trigger_ >= 1.0f) {
            amp_decay_trigger_ = 0.0f;
            fm_decay_trigger_ = 0.0f;
        }
    }
    
    if(!started_) {
        started_ = true;
        morph_ = morphTarget;
        tune_ = tuneTarget;
    }

    ParameterInterpolator morph_interpolator(&morph_, morphTarget, size);
    // ParameterInterpolator tune_interpolator(&tune_, tuneTarget, size);
    Downsampler carrier_downsampler(&carrier_fir_);
    Downsampler sub_carrier_downsampler(&sub_carrier_fir_);

    float amp_decay_trigger_increment = (1.0f / (6.0f * (GetAmpDecay() + 0.001f))) / kCorrectedSampleRate;
    float fm_decay_trigger_increment = (1.0f / (6.0f * (GetFMDecay() + 0.001f))) / kCorrectedSampleRate;

    float note = tuneTarget * calibration_x_ + calibration_y_;
    note = CLAMP<float>(note, 0.0f, 120.0f);

    note = quantizer.Quantize(note);

    note += 12 * GetY(GetFMDecayTrigger()) * (GetFMDepth() * 2.0f - 1.0f);
    note = CLAMP<float>(note, 0.0f, 120.0f);

    // note = note - 24.0f;

    ParameterInterpolator phase_increment_interpolator(&phase_increment_, NoteToFrequency(note), size);
    ParameterInterpolator sub_phase_increment_interpolator(&sub_phase_increment_, NoteToFrequency((note + subosc_detune_ / 100.0f + subosc_offset_)), size);

    while (size--) {       
        float phase = 0;
        bool isOscilloscope = false;
        float sample = 0;
        float sub_sample = 0;

        float interpolated_morph = morph_interpolator.Next();
        interpolated_morph = CLAMP<float>(interpolated_morph, 0.0f, 0.9999f);

        float phase_increment = phase_increment_interpolator.Next();
        phase_increment = CLAMP<float>(phase_increment, 0.0f, 1.0f);
        float sub_phase_increment = sub_phase_increment_interpolator.Next();
        sub_phase_increment = CLAMP<float>(sub_phase_increment, 0.0f, 1.0f);

        
        // for (size_t j = 0; j < kOversampling; ++j) {
            // float sample = 0;//GetSampleBetweenFrames(effect_manager.RenderPhaseEffect(phase_, frequency, fx_amount, fx, false), interpolated_morph);
            
            // sample = effect_manager.RenderSampleEffect(sample, phase_, frequency, fx_amount, fx, false);
            
            switch(fx_effect_) {
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

            switch(subosc_wave_){
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


            sample = (1 - amp_decay_trigger_) * sample;

            sub_sample = subosc_mix_ * sample + (1.0f - subosc_mix_) * sub_sample;


            phase_ += phase_increment;
            sub_phase_ += sub_phase_increment;
            amp_decay_trigger_ += amp_decay_trigger_increment;
            fm_decay_trigger_ += fm_decay_trigger_increment;
            
            if(phase_ >= 1.0f)
                phase_ -= 1.0f;

            if(sub_phase_ >= 1.0f)
                sub_phase_ -= 1.0f;            
            
            if(amp_decay_trigger_ >= 1.0f)
                amp_decay_trigger_ = 1.0f;
            
            if(fm_decay_trigger_ >= 1.0f)
                fm_decay_trigger_ = 1.0f;
            
        //     carrier_downsampler.Accumulate(j, sample);
        //     sub_carrier_downsampler.Accumulate(j, sub_sample);
        // }
        
        // sample = carrier_downsampler.Read();
        // sub_sample = sub_carrier_downsampler.Read();

        output->l = static_cast<int16_t>(sample * 26000.0f);
        output->r = static_cast<int16_t>(sub_sample * 26000.0f);
        ++output;
    }
}

void DrumEngine::SetWavetable(int wavetable)
{
    wavetable_ = CLAMP<int>(wavetable, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
}
int DrumEngine::GetWavetable() { return wavetable_; }
void DrumEngine::SetAmpDecay(float value) {
    amp_decay_ = CLAMP<float>(value, 0.0f, 1.0f);
}
float DrumEngine::GetAmpDecay() { return amp_decay_; }
void DrumEngine::SetFMDecay(float value) {
    fm_decay_ = CLAMP<float>(value, 0.0f, 1.0f);
}
float DrumEngine::GetFMDecay() { return fm_decay_; }
void DrumEngine::SetFMShape(float value) {
    fm_shape_ = CLAMP<float>(value, 0.0f, 1.0f);
}
float DrumEngine::GetFMShape() { return fm_shape_; }
void DrumEngine::SetFMDepth(float value) {
    fm_depth_ = CLAMP<float>(value, 0.0f, 1.0f);
}
float DrumEngine::GetFMDepth() { return fm_depth_; }
