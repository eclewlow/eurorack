/*
  ==============================================================================

    drum_engine.h
    Created: 30 Nov 2023 5:44:42pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/dsp/fx/effect.h"
#include "waves/dsp/Engine.h"

class Engine;

class DrumEngine: public Engine
{
public:
    DrumEngine();
    ~DrumEngine();
    virtual void Render(AudioDac::Frame* output, uint32_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph);
    float Render();
    void Init();
    float GetSample(int16_t wavetable, int16_t frame, float phase);
    float GetSampleBetweenFrames(float phase, float thisX);
    void FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx);
    float GetSample(float phase);
    float GetSampleNoFX(float phase, float fx, float morph);
    void triggerUpdate();
    
    void SetWavetable(int wavetable);
    int GetWavetable();
    void SetAmpDecay(float value);
    float GetAmpDecay();
    void SetFMDecay(float value);
    float GetFMDecay();
    void SetFMShape(float value);
    float GetFMShape();
    void SetFMDepth(float value);
    float GetFMDepth();
    inline float GetAmpDecayTrigger() { return amp_decay_trigger_; }
    inline float GetFMDecayTrigger() { return fm_decay_trigger_; }
    float GetY(float x);

private:
    // depth -50 to 50
    // decays 0 to 100
    // shape -50 to 50
    float amp_decay_trigger_;
    float fm_decay_trigger_;
    DISALLOW_COPY_AND_ASSIGN (DrumEngine);
};
