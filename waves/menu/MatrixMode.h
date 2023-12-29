/*
  ==============================================================================

    MatrixMode.h
    Created: 29 Nov 2023 12:27:07am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "State.h"

class State;

class MatrixMode: public State
{
public:
    enum MatrixModeEditState {
        MATRIX_MODE_EDIT_DEFAULT = 0,
        MATRIX_MODE_EDIT_TOPLEFT = 1,
        MATRIX_MODE_EDIT_BOTTOMRIGHT = 2,
        MATRIX_MODE_EDIT_LAST = 3,
        MATRIX_MODE_EDIT_OFFSET = 4,
    };
    MatrixMode();
    ~MatrixMode();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint(juce::Graphics& g);
    void triggerUpdate(bool back_pressed);

private:
    uint8_t edit_state_;
    bool editing_offset_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MatrixMode);
};
