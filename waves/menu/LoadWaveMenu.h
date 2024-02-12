/*
  ==============================================================================

    LoadWaveMenu.h
    Created: 26 Nov 2023 5:50:03pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

namespace waves {

class LoadWaveMenu: public State
{
public:
    enum LoadWaveMenuState {
        LOAD_WAVE_MENU_NONE,
        LOAD_WAVE_MENU_SELECT_WAVETABLE,
        LOAD_WAVE_MENU_SELECT_FRAME,
    };
    enum LoadWaveMenuTarget {
        AB_ENGINE_A     = 0,
        AB_ENGINE_B     = 1,
        WAVE_MANAGER    = 2,
    };
    LoadWaveMenu();
    ~LoadWaveMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint();
    void triggerUpdate(bool back_pressed);
    inline void setState(LoadWaveMenuState state) { state_ = state; }
    inline void setTarget(LoadWaveMenuTarget target) { target_ = target; }
    void ResetTicker();
    
private:
    int8_t state_;
    int16_t wavetable_offset_;
    int16_t frame_offset_;
    int16_t wavetable_;
    int16_t frame_;
    float morph_;
    int8_t target_;
    uint32_t ticker_timer_;
    uint8_t ticker_;
    
    DISALLOW_COPY_AND_ASSIGN (LoadWaveMenu);
};

}