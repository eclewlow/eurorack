/*
  ==============================================================================

    MemoryClearMenu.h
    Created: 14 Nov 2023 4:43:31pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "State.h"

class State;

class MemoryClearMenu: public State
{
public:
    enum SnapshotMenuState {
        MEMORY_MENU_NONE        = 0,
        MEMORY_MENU_WAVETABLES  = 1,
        MEMORY_MENU_SNAPSHOTS   = 2,
        MEMORY_MENU_CONFIRM     = 3,
    };
    enum MemoryMenuOptions {
        MEMORY_MENU_NO      = 0,
        MEMORY_MENU_YES     = 1,
    };

    MemoryClearMenu();
    ~MemoryClearMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint(juce::Graphics& g);
    void triggerUpdate(bool back_pressed) {};
private:
    int8_t state_;
    int8_t option_selected_;
    int8_t referer_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MemoryClearMenu);
};
