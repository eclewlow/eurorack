/*
  ==============================================================================

    EnterNameMenu.h
    Created: 14 Nov 2023 4:45:29pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

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
    virtual void paint(juce::Graphics& g);
    void triggerUpdate(bool back_pressed);
    inline void setState(EnterNameMenuState newState) { currentState = newState; }
    inline void setExecFunc(void (* func)(char* param)) { exec_func = func; }
    inline void setNameChars(const char* name) { strncpy(name_chars, name, 9); name_index = CLAMP<int>(strlen(name), 0, 8-1); }
    
private:
    int num_of_chars = 41;
    int num_of_name_chars = 8;
    char * char_list = (char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_- //";
    int char_index;
    int name_index;
    char name_chars[9];
    bool clear_selected = false;
    bool ok_selected = false;
    void (*exec_func)(char* param);
    EnterNameMenuState currentState;
    DISALLOW_COPY_AND_ASSIGN (EnterNameMenu);
};
