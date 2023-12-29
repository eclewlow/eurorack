/*
  ==============================================================================

    ab_engine.h
    Created: 19 Nov 2023 3:52:35am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/dsp/fx/effect.h"
#include "waves/dsp/Engine.h"
#include "waves/Globals.h"

class Engine;

class ABEngine: public Engine
{
public:
    ABEngine();
    ~ABEngine();
    virtual void Render(float* out, float* aux, uint32_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph);
    float Render();
    void Init();
    float GetSample(int16_t wavetable, int16_t frame, float phase);
    float GetSample(int16_t wavetable, int16_t frame, float phase, bool isLeft);
    float GetSampleBetweenFrames(float phase, float morph);
//    void SetX(float newX);
    bool handleKeyPress(const juce::KeyPress &key);
    void FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx);
    void FillWaveform(int16_t * waveform, int16_t wavetable, int16_t frame);
    float GetSample(float phase);
    float GetSampleNoFX(float phase, float fx, float morph);
    
    void SetLeftWavetable(int left_wavetable);
    void SetLeftFrame(int left_frame);
    int GetLeftWavetable();
    int GetLeftFrame();
    void SetRightWavetable(int right_wavetable);
    void SetRightFrame(int right_frame);
    int GetRightWavetable();
    int GetRightFrame();
    bool IsEditingLeft();
    bool IsEditingRight();
    void SetIsEditingLeft(bool is_editing_left);
    void SetIsEditingRight(bool is_editing_right);
    bool SetLeftWave(int table, int frame);
    bool SetRightWave(int table, int frame);

    void triggerUpdate();
private:
//    int left_wavetable_;
//    int left_frame_;
//    
//    int right_wavetable_;
//    int right_frame_;
//    
//    bool is_editing_left_;
//    bool is_editing_right_;

    DISALLOW_COPY_AND_ASSIGN (ABEngine);
};
