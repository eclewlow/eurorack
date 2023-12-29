/*
  ==============================================================================

    ControlStatusMenu.h
    Created: 14 Nov 2023 4:39:09pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "State.h"

class State;

class ControlStatusMenu: public State
{
public:
    enum ControlStatusMenuState {
        CONTROL_STATUS_STATE_NONE,
    };
    ControlStatusMenu();
    ~ControlStatusMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint(juce::Graphics& g);
    void triggerUpdate(bool back_pressed);
    void UpdateWaveform();
    void ClearWaveform();
    inline void setState(ControlStatusMenuState newState) { currentState = newState; }

private:
    ControlStatusMenuState currentState;
    uint16_t pitch_wavedata_[20];
    uint16_t fx_amount_wavedata_[20];
    uint16_t fx_wavedata_[20];
    uint16_t morph_wavedata_[20];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlStatusMenu);
};
