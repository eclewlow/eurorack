/*
  ==============================================================================

    QuantizerMenu.h
    Created: 14 Nov 2023 4:43:19pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "State.h"

class State;

class QuantizerMenu: public State
{
public:
    QuantizerMenu();
    ~QuantizerMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint(juce::Graphics& g);
    void triggerUpdate(bool back_pressed) {};
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QuantizerMenu);
};
