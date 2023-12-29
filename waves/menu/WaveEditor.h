/*
  ==============================================================================

    WaveEditor.h
    Created: 14 Nov 2023 4:45:52pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "State.h"
#include "fft.h"

class State;

class WaveEditor: public State
{
public:
    enum WaveEditorState {
        WAVE_EDITOR_STATE_MENU = 0,
        WAVE_EDITOR_STATE_EDITOR = 1,
        WAVE_EDITOR_STATE_PEN = 2,
        WAVE_EDITOR_STATE_LINE = 3,
        WAVE_EDITOR_STATE_SPECTRAL = 4,
    };

    enum WaveEditorSelection {
        WAVE_EDITOR_SELECTION_SQUARE    = 0,
        WAVE_EDITOR_SELECTION_TRIANGLE  = 1,
        WAVE_EDITOR_SELECTION_SINE      = 2,
        WAVE_EDITOR_SELECTION_RAMP      = 3,
        WAVE_EDITOR_SELECTION_SAW       = 4,
        WAVE_EDITOR_SELECTION_PEAK      = 5,
        WAVE_EDITOR_SELECTION_PEN       = 6,
        WAVE_EDITOR_SELECTION_LINE      = 7,
        WAVE_EDITOR_SELECTION_SPECTRAL  = 8,
        WAVE_EDITOR_SELECTION_CLEAR     = 9,
        WAVE_EDITOR_SELECTION_LOAD      = 10,
        WAVE_EDITOR_SELECTION_SAVE      = 11,
    };
    enum WaveEditorRightEncoderState {
        WAVE_EDITOR_RIGHT_ENCODER_DRAW,
        WAVE_EDITOR_RIGHT_ENCODER_EXPAND
    };
//    enum ABActiveMenuSide {
//        LEFT,
//        RIGHT,
//        NONE
//    };
    WaveEditor();
    ~WaveEditor();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint(juce::Graphics& g);
    virtual void triggerUpdate(bool back_pressed);
    void CalculateFFT();
    void CalculateIFFT();
    void LineToWavedata(int x0, int y0, int x1, int y1);
    void DrawMenu();
    void DrawTriangle(int x, int y, bool reversed);
    inline void setWavedata(int16_t * data) { wavedata_ = data; }
    
    inline void setFrame(uint16_t frame) { frame_ = frame; }
    inline void setWavetable(uint16_t wavetable) { wavetable_ = wavetable; }
    
private:
    int16_t * wavedata_;
    WaveEditorRightEncoderState right_state_;
    int8_t selection_;
    int8_t mode_;
    WaveEditorState state_;
    int16_t menu_selection_offset_;
    uint32_t timer_;
    int menu_target_offset_y_;
    int menu_offset_y_;
    int selection_x1_;
    int selection_x2_;
    int spectral_cursor_;
    float spectral_gain_[32];
    FFT::COMPLEX_NUMBER spectral_phasors_[2048];
    float spectral_angles_[64];
    int16_t pen_x_;
    int16_t pen_y_;
    int16_t line_x_;
    int16_t line_y_;
    bool pen_drawing_;
    int16_t wavetable_;
    int16_t frame_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveEditor);
};
