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

// class Engine;

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
    float GetSampleBetweenFramesNoDMA(float phase, float morph_x, float morph_y);
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

    inline bool is_in_buffer(int8_t integral_x, int8_t integral_y, int8_t index_x, int8_t index_y) {
      return integral_x >= index_x && integral_x < index_x + 2 && integral_y >= index_y && integral_y < index_y + 2;            
    }

    inline void swap_buffers(int16_t ** buf_1, int16_t ** buf_2) {
      int16_t * temp_buffer = *buf_1;
      *buf_1 = *buf_2;
      *buf_2 = temp_buffer;
    }

    inline void swap_int(int8_t *int_1, int8_t *int_2) {
      int temp = *int_1;
      *int_1 = *int_2;
      *int_2 = temp;
    }

    static void on_load_1_finished();
    static void on_load_2_finished();

    static void on_triple_load_1_finished();
    static void on_triple_load_2_finished();
    static void on_triple_load_3_finished();

private:
    int8_t current_frame_x;
    int8_t current_frame_y;
    int8_t target_frame_x;
    int8_t target_frame_y;
    int8_t buffered_frame_x;
    int8_t buffered_frame_y;

    int8_t x1_;                // (0, 15)
    int8_t x2_;                // (x1, 15)
    int8_t y1_;                // (0, 15)
    int8_t y2_;                // (y1, 15)
    int8_t wavelist_offset_;   // (0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 16 - 1)

    DISALLOW_COPY_AND_ASSIGN (MatrixEngine);
};
