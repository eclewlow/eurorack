/*
  ==============================================================================

    ModeMenu.h
    Created: 7 Nov 2023 4:54:56pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

namespace waves {

class ModeMenu: public State
{
public:
    enum ModeMenuState {
        MODE_AB         = 0,
        MODE_WAVETABLE  = 1,
        MODE_MATRIX     = 2,
        MODE_DRUM       = 3,
    };
    ModeMenu();
    ~ModeMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint();
    void triggerUpdate(bool back_pressed) {};
    inline void setState(ModeMenuState newState) { currentState = newState; }

private:
    
    int8_t currentState;

    DISALLOW_COPY_AND_ASSIGN (ModeMenu);
};

}