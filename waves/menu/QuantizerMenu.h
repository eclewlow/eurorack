/*
  ==============================================================================

    QuantizerMenu.h
    Created: 14 Nov 2023 4:43:19pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

namespace waves {

class QuantizerMenu: public State
{
public:
    QuantizerMenu();
    ~QuantizerMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint();
    void triggerUpdate(bool back_pressed) {};
private:
    DISALLOW_COPY_AND_ASSIGN (QuantizerMenu);
};

}