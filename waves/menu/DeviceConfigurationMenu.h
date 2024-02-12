/*
  ==============================================================================

    DeviceConfigurationMenu.h
    Created: 14 Nov 2023 4:40:15pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

namespace waves {

class DeviceConfigurationMenu: public State
{
public:
    enum DeviceConfigurationMenuState {
        DEVICE_INFO,
        DEVICE_DISPLAY,
        DEVICE_OSCILLOSCOPE,
        DEVICE_IO,
        DEVICE_CALIBRATION,
        DEVICE_QUANTIZER,
        DEVICE_MEMORY,
    };
    DeviceConfigurationMenu();
    ~DeviceConfigurationMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint();
    void triggerUpdate(bool back_pressed) {};
    inline void setState(DeviceConfigurationMenuState newState) { currentState = newState; }

private:
    
    int8_t currentState;

    DISALLOW_COPY_AND_ASSIGN (DeviceConfigurationMenu);
};

}