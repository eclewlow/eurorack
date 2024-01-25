/*
  ==============================================================================

    Engine.h
    Created: 9 Nov 2023 3:03:36pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/dsp/fx/effect.h"
#include "waves/dsp/fx/effect_manager.h"
#include "waves/defines.h"
#include "waves/drivers/audio_dac.h"
#include "stmlib/dsp/units.h"
#include "stmlib/dsp/parameter_interpolator.h"
#include "waves/resources.h"

using namespace waves;

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

protected:
    float phase_;
    float morph_;
    float fx_amount_;
    float fx_;
    float tune_;
    float phase_increment_;

    float sub_phase_increment_;
    float sub_phase_;

    bool started_;
    LoadingState loading_state_;
    // Engine(const Engine&);
    // Engine& operator=(Engine const&);
    // DISALLOW_COPY_AND_ASSIGN (Engine);
};
