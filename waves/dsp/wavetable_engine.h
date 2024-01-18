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
    float swap_counter_;
    bool swap_;
    DISALLOW_COPY_AND_ASSIGN (WavetableEngine);
};
