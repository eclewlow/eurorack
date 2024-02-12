/*
  ==============================================================================

    InfoMenu.h
    Created: 14 Nov 2023 4:39:44pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

namespace waves {


class InfoMenu: public State
{
public:
    enum InfoMenuState {
        INFO_MENU_NONE,
//        FX_MENU_LEFT_DEPTH,
    };
    InfoMenu();
    ~InfoMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint();
    void triggerUpdate(bool back_pressed) {};
    inline void setState(InfoMenuState state) { state_ = state; }

private:
    InfoMenuState state_;
    DISALLOW_COPY_AND_ASSIGN (InfoMenu);
};

}