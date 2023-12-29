/*
  ==============================================================================

    DisplayMenu.h
    Created: 14 Nov 2023 4:41:08pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

class DisplayConfigurationMenu: public State
{
public:
    enum DisplayConfigurationMenuState {
        DISPLAY_MENU_BRIGHTNESS,
        DISPLAY_MENU_CONTRAST,
        DISPLAY_MENU_INVERT,
        DISPLAY_MENU_SCOPE,

//        FX_MENU_LEFT_DEPTH,
    };
    DisplayConfigurationMenu();
    ~DisplayConfigurationMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint(juce::Graphics& g);
    void triggerUpdate(bool back_pressed) {};
    inline void setState(DisplayConfigurationMenuState state) { state_ = state; }

private:
    DisplayConfigurationMenuState state_;
    DISALLOW_COPY_AND_ASSIGN (DisplayConfigurationMenu);
};
