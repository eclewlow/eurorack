/*
  ==============================================================================

    WavetableMenu.h
    Created: 18 Nov 2023 8:27:59pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

namespace waves {

class WavetableModeMenu: public State
{
public:
    WavetableModeMenu();
    ~WavetableModeMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint();
    void triggerUpdate(bool back_pressed);

private:
    int16_t wavetable_offset_;
    int16_t wavetable_;

    DISALLOW_COPY_AND_ASSIGN (WavetableModeMenu);
};

}