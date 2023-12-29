/*
  ==============================================================================

    IOConfiguration.h
    Created: 14 Nov 2023 4:42:37pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

class IOConfigurationMenu: public State
{
public:
    enum IOConfigurationMenuInput {
        IO_CONFIG_PITCH     = 0,
        IO_CONFIG_FX_AMOUNT = 1,
        IO_CONFIG_FX        = 2,
        IO_CONFIG_MORPH     = 3,
        IO_CONFIG_LAST      = 4,
    };
    enum IOConfigurationMenuState {
        IO_CONFIG_MENU_INPUT = 0,
        IO_CONFIG_MENU_GAIN = 1,
        IO_CONFIG_MENU_BIAS = 2,
    };

    IOConfigurationMenu();
    ~IOConfigurationMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint(juce::Graphics& g);
    void triggerUpdate(bool back_pressed);
    inline void setState(int8_t state) { state_ = state; }
    void UpdateWaveform();
    void ClearWaveform();

private:
    int8_t input_;
    int8_t state_;
    uint16_t wavedata_[50];
    DISALLOW_COPY_AND_ASSIGN (IOConfigurationMenu);
};
