/*
  ==============================================================================

    Oscilloscope.h
    Created: 13 Nov 2023 6:13:45pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

class Oscilloscope: public State
{
public:
    enum OscilloscopeState {
        OSCILLOSCOPE_STATE_NONE,
    };
    Oscilloscope();
    ~Oscilloscope();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint(juce::Graphics& g);
    void triggerUpdate(bool back_pressed) {};
    inline void setState(OscilloscopeState newState) { currentState = newState; }
    
private:
    OscilloscopeState currentState;
    DISALLOW_COPY_AND_ASSIGN (Oscilloscope);
};
