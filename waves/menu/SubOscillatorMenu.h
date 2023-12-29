/*
  ==============================================================================

    SubOscillatorMenu.h
    Created: 14 Nov 2023 4:38:34pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "State.h"

class State;

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
    virtual void paint(juce::Graphics& g);
    void triggerUpdate(bool back_pressed) {};
    inline void setState(SubOscillatorMenuState state) { state_ = state; }

private:
    int8_t state_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubOscillatorMenu);
};
