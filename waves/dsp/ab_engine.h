/*
  ==============================================================================

    ab_engine.h
    Created: 19 Nov 2023 3:52:35am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/Globals.h"
// #include "waves/dsp/fx/effect.h"
// #include "waves/dsp/Engine.h"

// class Engine;

using namespace waves;

namespace waves {

class ABEngine: public Engine
{
public:
    ABEngine();
    ~ABEngine();
    virtual void Render(AudioDac::Frame* output, size_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph);
    float Render();
    void Init();
    float GetSample(int16_t wavetable, int16_t frame, float phase);
    float GetSample(int16_t wavetable, int16_t frame, float phase, bool isLeft);
    float GetSampleBetweenFrames(float phase, float morph);
//    void SetX(float newX);
    void FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx);
    void FillWaveform(int16_t * waveform, bool is_left);
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
    static void on_load_both_ab_left_finished();
    static void on_load_both_ab_right_finished();
    static void on_load_one_ab_left_finished();
    static void on_load_one_ab_right_finished();
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


}
