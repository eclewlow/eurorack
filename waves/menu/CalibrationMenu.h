/*
  ==============================================================================

    CalibrationMenu.h
    Created: 14 Nov 2023 4:42:55pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

class CalibrationMenu: public State
{
public:
//    enum IOConfigurationMenuInput {
//        IO_CONFIG_PITCH     = 0,
//        IO_CONFIG_FX_AMOUNT = 1,
//        IO_CONFIG_FX        = 2,
//        IO_CONFIG_MORPH     = 3,
//        IO_CONFIG_LAST      = 4,
//    };
    enum CalibrationMenuState {
        CALIBRATION_MENU_STATE_1V = 0,
        CALIBRATION_MENU_STATE_5V = 1,
        CALIBRATION_MENU_STATE_DONE = 2,
    };

    CalibrationMenu();
    ~CalibrationMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint(juce::Graphics& g);
    void triggerUpdate(bool back_pressed);
    inline void setState(int8_t state) { state_ = state; }

private:
    int8_t state_;
    uint32_t timer_;
    bool message_displayed_;
    int16_t c1_value_;
    int16_t c5_value_;
    DISALLOW_COPY_AND_ASSIGN (CalibrationMenu);
};
