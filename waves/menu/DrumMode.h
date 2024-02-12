/*
  ==============================================================================

    DrumMode.h
    Created: 30 Nov 2023 5:44:53pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

namespace waves {

class DrumMode: public State
{
public:
    enum DrumModeEditState {
        DRUM_MODE_EDIT_WAVETABLE = 0,
        DRUM_MODE_EDIT_AMP_DECAY = 1,
        DRUM_MODE_EDIT_FM_DECAY = 2,
        DRUM_MODE_EDIT_FM_SHAPE = 3,
        DRUM_MODE_EDIT_FM_DEPTH = 4,
        DRUM_MODE_EDIT_LAST = 5,
    };
    DrumMode();
    ~DrumMode();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint();
    void triggerUpdate(bool back_pressed);
    void setEditing(bool editing);

private:
    // depth -50 to 50
    // decays 0 to 100
    // shape -50 to 50
    uint32_t timer_;
    bool is_editing_;
    int8_t edit_state_;
    DISALLOW_COPY_AND_ASSIGN (DrumMode);
};

}