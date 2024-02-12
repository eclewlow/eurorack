/*
  ==============================================================================

    SubOscillatorMenu.h
    Created: 14 Nov 2023 4:38:34pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

namespace waves {

class SubOscillatorMenu: public State
{
public:
    enum SubOscillatorMenuState {
        SUB_OSC_MENU_OFFSET     = 0,
        SUB_OSC_MENU_DETUNE     = 1,
        SUB_OSC_MENU_MIX        = 2,
        SUB_OSC_MENU_WAVE       = 3,

    };
    SubOscillatorMenu();
    ~SubOscillatorMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint();
    void triggerUpdate(bool back_pressed) {};
    inline void setState(SubOscillatorMenuState state) { state_ = state; }

private:
    int8_t state_;
    DISALLOW_COPY_AND_ASSIGN (SubOscillatorMenu);
};

}