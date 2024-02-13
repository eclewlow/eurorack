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

namespace waves {

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
    virtual void paint();
    void triggerUpdate(bool back_pressed) {};
    inline void setState(DisplayConfigurationMenuState state) { state_ = state; }


    inline void setBrightness(int8_t brightness) { brightness = CLAMP<int8_t>(brightness, 0, 100); }
    inline int8_t getBrightness() {return brightness;}

    inline void setContrast(int8_t contrast) { contrast = CLAMP<int8_t>(contrast, 0, 100); }
    inline int8_t getContrast() {return contrast;}

    inline void setInvert(bool invert) { invert = invert; }
    inline bool getInvert() {return invert;}

    inline void setScopeSetting(int8_t scope_setting) { scope_setting = scope_setting; }
    inline int8_t getScopeSetting() {return scope_setting;}

private:
    int8_t brightness;
    int8_t contrast;
    bool invert;
    int8_t scope_setting;                  // (0, 1)
    int8_t morph_setting;                  // (0, 1)

    DisplayConfigurationMenuState state_;
    DISALLOW_COPY_AND_ASSIGN (DisplayConfigurationMenu);
};

}