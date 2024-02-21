/*
  ==============================================================================

    Engine.h
    Created: 9 Nov 2023 3:03:36pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

// #include "waves/dsp/fx/effect.h"
// #include "waves/dsp/fx/effect_manager.h"
#include "waves/defines.h"
#include "waves/drivers/audio_dac.h"
#include "stmlib/dsp/units.h"
#include "stmlib/dsp/parameter_interpolator.h"
#include "waves/resources.h"

namespace waves {

static const float kCorrectedSampleRate = 47872.34f;
const float a0 = (440.0f / 8.0f) / kCorrectedSampleRate;

const float kSineLUTSize = 512.0f;
const size_t kSineLUTQuadrature = 128;
const size_t kSineLUTBits = 9;

class Engine
{
public:
    Engine();
    virtual ~Engine() {};
    virtual void Render(AudioDac::Frame* output, size_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph) = 0;
    virtual void FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx = true) = 0;
    virtual float GetSampleNoFX(float phase, float fx, float morph) = 0;
    virtual void triggerUpdate() = 0;
    inline void reset_phase() { phase_ = 0; }
    
    inline float NoteToFrequency(float midi_note) {
      midi_note -= 9.0f;
      CONSTRAIN(midi_note, -128.0f, 127.0f);
      return a0 * 0.25f * stmlib::SemitonesToRatio(midi_note);
    }

    // Safe for phase >= 0.0f, will wrap.
    inline float Sine(float phase) {
      return stmlib::InterpolateWrap(lut_sine, phase, kSineLUTSize);
    }

    inline void set_subosc_wave(int8_t subosc_wave) { subosc_wave_ = subosc_wave; }

    inline int8_t get_subosc_wave() { return subosc_wave_; }

    inline void set_fx_effect(int8_t fx_effect) { fx_effect_ = fx_effect; }

    inline int8_t get_fx_effect() { return fx_effect_; }

    inline void Calibrate(uint16_t c1, uint16_t c5) {
      calibration_x_ = 48.0f / (c5 - c1);
      calibration_y_ = 24.0f - c1 * calibration_x_;
    }

    inline float getCalibrationX() { return calibration_x_; }
    inline float getCalibrationY() { return calibration_y_; }

protected:
    float phase_;
    float morph_;
    float fx_amount_;
    float fx_;
    float tune_;
    float phase_increment_;

    float sub_phase_increment_;
    float sub_phase_;

    int8_t subosc_offset_;                    // (-24, 24)
    int8_t subosc_detune_;                    // (-50, 50)
    float subosc_mix_;                        // (0.0f, 1.0f)
    int8_t subosc_wave_;                      // (0, 5)
    float calibration_x_;                   // ()  // don't randomize this, but save in snapshot
    float calibration_y_;                   // ()  // don't randomize this, but save in snapshot

    float fx_depth_;                       
    bool fx_sync_;                         
    int8_t fx_scale_;                      
    int8_t fx_range_;                      
    int8_t fx_oscillator_shape_;           
    int8_t fx_control_type_;               
    int8_t fx_effect_;                     

    float carrier_fir_;
    float sub_carrier_fir_;

    bool started_;
    LoadingState loading_state_;
    // Engine(const Engine&);
    // Engine& operator=(Engine const&);
    // DISALLOW_COPY_AND_ASSIGN (Engine);
};

}