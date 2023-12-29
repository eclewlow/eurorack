/*
  ==============================================================================

    DeviceConfigurationMenu.h
    Created: 14 Nov 2023 4:40:15pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "State.h"

class State;

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
    virtual void paint(juce::Graphics& g);
    void triggerUpdate(bool back_pressed) {};
    inline void setState(DeviceConfigurationMenuState newState) { currentState = newState; }

private:
    
    int8_t currentState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeviceConfigurationMenu);
};
