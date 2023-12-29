/*
  ==============================================================================

    ModeMenu.h
    Created: 7 Nov 2023 4:54:56pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "State.h"

class State;

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
    virtual void paint(juce::Graphics& g);
    void triggerUpdate(bool back_pressed) {};
    inline void setState(ModeMenuState newState) { currentState = newState; }

private:
    
    int8_t currentState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModeMenu);
};
