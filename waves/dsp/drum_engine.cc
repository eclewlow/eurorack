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

DrumEngine::DrumEngine() {
    phase_ = 0.0f;
    amp_decay_trigger_ = 1.0f;
    fm_decay_trigger_ = 1.0f;
}

DrumEngine::~DrumEngine() {
    
}

void DrumEngine::Init() {
    phase_ = 0.0f;
    amp_decay_trigger_ = 1.0f;
    fm_decay_trigger_ = 1.0f;
}

float DrumEngine::GetSample(int16_t wavetable, int16_t frame, float phase) {
    return 0.0f;
}

float DrumEngine::GetSampleBetweenFrames(float phase, float morph) {
    float frame = morph * 15.0f;
    uint16_t frame_integral = floor(frame);
    float frame_fractional = frame - frame_integral;
    
    uint16_t next_frame_integral = (frame_integral + 1) % 16;

    float frame1sample = GetSample(GetWavetable(), frame_integral, phase);
    float frame2sample = GetSample(GetWavetable(), next_frame_integral, phase);
    
    float sample = frame1sample * (1.0f - frame_fractional) + frame2sample * frame_fractional;
    return sample;
}

void DrumEngine::FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx) {
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
        loading = 56;

    //    float target = morph;
    // convert 12 bit uint 0-4095 to 0...15 float
    float morphTarget = morph * 1.0 / 4095.0;
    //    float interpolatedFloat = interpolated16 / 32768.0f;
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
    ParameterInterpolator tune_interpolator(&tune_, tuneTarget, size);
    Downsampler carrier_downsampler(&carrier_fir_);
    
    while (size--) {
        float amp_decay_trigger_increment = (1.0f / (6.0f * (GetAmpDecay() + 0.001f))) / 48000.0f;
        float fm_decay_trigger_increment = (1.0f / (6.0f * (GetFMDecay() + 0.001f))) / 48000.0f;

        float note = tune_interpolator.Next() * settings_.calibration_x + settings_.calibration_y;
        note = CLAMP<float>(note, 0.0f, 120.0f);

        note = quantizer.Quantize(note);

        note += 12 * GetY(GetFMDecayTrigger()) * (GetFMDepth() * 2.0f - 1.0f);
        note = CLAMP<float>(note, 0.0f, 120.0f);


        note = note - 24.0f;
        float a = 440; //frequency of A (coomon value is 440Hz)
        float frequency = (a / 32) * pow(2, ((note - 9) / 12.0));
        
        float phaseIncrement = frequency / 48000.0f;
        

        float interpolated_morph = morph_interpolator.Next();
        interpolated_morph = CLAMP<float>(interpolated_morph, 0.0, 1.0);
        
        for (size_t j = 0; j < kOversampling; ++j) {
            float sample = GetSampleBetweenFrames(effect_manager.RenderPhaseEffect(phase_, frequency, fx_amount, fx, false), interpolated_morph);
            
            sample = effect_manager.RenderSampleEffect(sample, phase_, frequency, fx_amount, fx, false);
            
            sample = (1 - amp_decay_trigger_) * sample;
            
            amp_decay_trigger_ += amp_decay_trigger_increment;
            
            if(amp_decay_trigger_ >= 1.0f)
                amp_decay_trigger_ = 1.0f;

            fm_decay_trigger_ += fm_decay_trigger_increment;
            
            if(fm_decay_trigger_ >= 1.0f)
                fm_decay_trigger_ = 1.0f;

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

void DrumEngine::SetWavetable(int wavetable)
{
    settings_.drum_engine_wavetable = CLAMP<int>(wavetable, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
}
int DrumEngine::GetWavetable() { return settings_.drum_engine_wavetable; }
void DrumEngine::SetAmpDecay(float value) {
    settings_.drum_engine_amp_decay = CLAMP<float>(value, 0.0f, 1.0f);
}
float DrumEngine::GetAmpDecay() { return settings_.drum_engine_amp_decay; }
void DrumEngine::SetFMDecay(float value) {
    settings_.drum_engine_fm_decay = CLAMP<float>(value, 0.0f, 1.0f);
}
float DrumEngine::GetFMDecay() { return settings_.drum_engine_fm_decay; }
void DrumEngine::SetFMShape(float value) {
    settings_.drum_engine_fm_shape = CLAMP<float>(value, 0.0f, 1.0f);
}
float DrumEngine::GetFMShape() { return settings_.drum_engine_fm_shape; }
void DrumEngine::SetFMDepth(float value) {
    settings_.drum_engine_fm_depth = CLAMP<float>(value, 0.0f, 1.0f);
}
float DrumEngine::GetFMDepth() { return settings_.drum_engine_fm_depth; }
