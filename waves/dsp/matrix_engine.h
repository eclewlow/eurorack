/*
  ==============================================================================

    matrix_engine.h
    Created: 19 Nov 2023 3:53:03am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/dsp/fx/effect.h"
#include "waves/dsp/Engine.h"

class Engine;

class MatrixEngine: public Engine
{
public:
    MatrixEngine();
    ~MatrixEngine();
    virtual void Render(AudioDac::Frame* output, size_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph);
    float Render();
    void Init();
    float GetSample(int16_t wavetable, int16_t frame, float phase);
    float GetSampleBetweenFrames(float phase, float morph_x, float morph_y);
    void FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx);
    float GetSample(float phase);
    float GetSampleNoFX(float phase, float fx, float morph);
    void triggerUpdate();
    
    void SetX1(int8_t x1);
    void SetY1(int8_t y1);
    void SetX2(int8_t x2);
    void SetY2(int8_t y2);

    void IncrementX1(int8_t dx);
    void IncrementY1(int8_t dy);
    void IncrementX2(int8_t dx);
    void IncrementY2(int8_t dy);

    int8_t GetX1();
    int8_t GetY1();
    int8_t GetX2();
    int8_t GetY2();
    
    void SetWavelistOffset(int8_t offset);
    int8_t GetWavelistOffset();

private:
    int current_frame_x;
    int target_frame_x;
    int current_frame_y;
    int target_frame_y;
    DISALLOW_COPY_AND_ASSIGN (MatrixEngine);
};
