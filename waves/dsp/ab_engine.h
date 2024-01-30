/*
  ==============================================================================

    ab_engine.h
    Created: 19 Nov 2023 3:52:35am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/Globals.h"
#include "waves/dsp/downsampler/4x_downsampler.h"

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
    void Render(AudioDac::Frame* output, size_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph);
    // void Render(AudioDac::Frame* output, size_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, uint8_t effect, uint8_t sub_osc_shape);
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
    // inline void Accumulate(int i, float sample) {
    //     head_ += sample * lut_4x_downsampler_fir[3 - (i & 3)];
    //     tail_ += sample * lut_4x_downsampler_fir[i & 3];
    // }
    
    // inline float Read() {
    //     float value = head_;
    //     head_ = tail_;
    //     tail_ = 0.0f;
    //     return value;
    // }

private:
    int left_wavetable_;
    int left_frame_;
    //    
    int right_wavetable_;
    int right_frame_;
    //    
    bool is_editing_left_;
    bool is_editing_right_;

    float carrier_fir_;
    float sub_carrier_fir_;

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

    DISALLOW_COPY_AND_ASSIGN (ABEngine);
};


}
