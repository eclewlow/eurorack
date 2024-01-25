/*
  ==============================================================================

    wavetable_engine.h
    Created: 19 Nov 2023 3:52:47am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/dsp/fx/effect.h"
#include "waves/dsp/Engine.h"

class Engine;

class WavetableEngine: public Engine
{
public:
    WavetableEngine();
    ~WavetableEngine();
    virtual void Render(AudioDac::Frame* output, size_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph);
    float Render();
    void Init();
    float GetSample(int16_t wavetable, int16_t frame, float phase);
    float GetSampleBetweenFrames(float phase, float morph, bool swap = false);
    void FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx);
    float GetSample(float phase);
    float GetSampleNoFX(float phase, float fx, float morph);
    void triggerUpdate();
    
    void SetWavetable(int wavetable);
    int GetWavetable();

    static void on_load_finished();
private:
    int current_frame_;
    int target_frame_;
    int buffered_frame_;
    float swap_counter_;
    bool swap_;
    float carrier_fir_;
    float sub_carrier_fir_;

    int8_t subosc_offset_;                    // (-24, 24)
    int8_t subosc_detune_;                    // (-50, 50)
    float subosc_mix_;                        // (0.0f, 1.0f)
    int8_t subosc_wave_;                      // (0, 5)
    float calibration_x_;                   // ()  // don't randomize this, but save in snapshot
    float calibration_y_;                   // ()  // don't randomize this, but save in snapshot

    int8_t wavetable_;      // (0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1)
    int8_t fx_effect_;                       // (0, 8)

    DISALLOW_COPY_AND_ASSIGN (WavetableEngine);
};
