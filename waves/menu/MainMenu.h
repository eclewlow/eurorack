/*
  ==============================================================================

    MainMenu.h
    Created: 7 Nov 2023 1:30:39pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "State.h"

class State;

class MainMenu: public State
{
public:
    enum MainMenuState {
        MAIN_WAVE_DESIGN        = 0,
        MODE_SELECT             = 1,
        FX_MANAGEMENT           = 2,
        SUBOSCILLATOR_CONFIG    = 3,
        PLAYBACK_MONITORING     = 4,
        SNAPSHOTS_LIST          = 5,
        WAVETABLE_MANAGEMENT    = 6,
        MODULE_SETUP_CONFIG     = 7,
    };
    MainMenu();
    ~MainMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint(juce::Graphics& g);
    void triggerUpdate(bool back_pressed) {};
    inline void setState(MainMenuState newState) { currentState = newState; }

private:
    
    int8_t currentState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainMenu);
};
