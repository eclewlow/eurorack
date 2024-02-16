/*
  ==============================================================================

    EnterNameMenu.h
    Created: 14 Nov 2023 4:45:29pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"
#include "waves/Globals.h"
#include <string.h>
#include <cstring>

class State;

namespace waves {


class EnterNameMenu: public State
{
public:
    enum EnterNameMenuState {
    };
    EnterNameMenu();
    ~EnterNameMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint();
    void triggerUpdate(bool back_pressed);
    inline void setState(EnterNameMenuState newState) { currentState = newState; }
    inline void setExecFunc(void (* func)(char* param)) { exec_func = func; }
    inline void setNameChars(const char* name) { strncpy(name_chars, name, 9); name_index = CLAMP<int>(strlen(name), 0, 8-1); }
    
private:
    static const int num_of_chars = 41;
    static const int num_of_name_chars = 8;
    static const char * char_list;
    int char_index;
    int name_index;
    char name_chars[9];
    bool clear_selected;
    bool ok_selected;
    void (*exec_func)(char* param);
    EnterNameMenuState currentState;
    DISALLOW_COPY_AND_ASSIGN (EnterNameMenu);
};


}