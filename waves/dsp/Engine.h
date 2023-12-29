/*
  ==============================================================================

    Engine.h
    Created: 9 Nov 2023 3:03:36pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "effect.h"

class Engine
{
public:
    Engine();
    virtual ~Engine() {};
    virtual void Render(float* out, float* aux, uint32_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph) = 0;
    virtual void FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx = true) = 0;
    virtual float GetSample(int16_t wavetable, int16_t frame, float phase) = 0;
    virtual float GetSampleNoFX(float phase, float fx, float morph) = 0;
    virtual void triggerUpdate() = 0;
    float GetSine(float phase);
    float GetRamp(float phase, float phase_increment);
    float GetSawtooth(float phase, float phase_increment);
    float GetSquare(float phase, float phase_increment);
    float GetTriangle(float phase);
    float GetOscillatorSample(float phase, float phase_increment);
    double poly_blep(double t);
    float poly_blep(float t, float dt);
    inline float ThisBlepSample(float t) { return 0.5f * t * t; }
    inline float NextBlepSample(float t) { t = 1.0f - t; return -0.5f * t * t; }
    inline void reset_phase() { phase_ = 0; }
    
protected:
    float phase_;
    float morph_;
    float fx_amount_;
    float fx_;
    float tune_;
    float carrier_fir_;
    bool started_ = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Engine);
};
