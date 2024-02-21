/*
  ==============================================================================

    SaveWaveMenu.h
    Created: 26 Nov 2023 5:50:08pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

namespace waves {

class SaveWaveMenu: public State
{
public:
    enum SaveWaveMenuState {
        SAVE_WAVE_MENU_NONE,
        SAVE_WAVE_MENU_SELECT_WAVETABLE,
        SAVE_WAVE_MENU_SELECT_FRAME,
    };
    SaveWaveMenu();
    ~SaveWaveMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint();
    void triggerUpdate(bool back_pressed);
    inline void setState(int8_t state) { state_ = state; }
    static void SaveWavetable(char* param);
    static void SaveWave(char* param);

    inline void setWavedata(int16_t * data) { wavedata_ = data; }

    void ResetTicker();
    
private:
    int16_t * wavedata_;
    int8_t state_;
    int16_t wavetable_offset_;
    int16_t frame_offset_;
    int16_t wavetable_;
    int16_t frame_;
    uint32_t ticker_timer_;
    uint8_t ticker_;

    char wavetable_names_[16][9];
    char frame_names_[16][9];

    int wavetable_gui_;
    int frame_gui_;

    DISALLOW_COPY_AND_ASSIGN (SaveWaveMenu);
};

}