/*
  ==============================================================================

    ManageMenu.h
    Created: 5 Dec 2023 2:46:59pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "State.h"

class State;

class ManageMenu: public State
{
public:
    enum ManageMenuState {
        MANAGE_MENU_NONE                = 0,
        MANAGE_MENU_SELECT_WAVETABLE    = 1,
        MANAGE_MENU_SELECT_FRAME        = 2,
        MANAGE_MENU_WAVETABLE_OPTIONS   = 3,
        MANAGE_MENU_CONFIRM             = 4,
        MANAGE_MENU_MOVE_WAVETABLE      = 5,
        MANAGE_MENU_MOVE_FRAME          = 6,
        MANAGE_MENU_FRAME_OPTIONS       = 7,
        MANAGE_MENU_COPY_WAVETABLE      = 8,
        MANAGE_MENU_COPY_FRAME          = 9,
    };
    enum ManageMenuOptions {
        MANAGE_MENU_EDIT                = 0,
        MANAGE_MENU_COPY                = 1,
        MANAGE_MENU_RENAME              = 2,
        MANAGE_MENU_DELETE              = 3,
        MANAGE_MENU_MOVE                = 4,
        MANAGE_MENU_NO                  = 5,
        MANAGE_MENU_YES                 = 6,
    };
    
    enum ManageMenuCopyState {
        MANAGE_MENU_COPY_STATE_NONE           = 0,
        MANAGE_MENU_COPY_STATE_WAVETABLE      = 1,
        MANAGE_MENU_COPY_STATE_FRAME          = 2,
    };

    ManageMenu();
    ~ManageMenu();
    virtual bool handleKeyPress(int key);
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key);
    virtual void paint(juce::Graphics& g);
    void triggerUpdate(bool back_pressed);
    inline void setState(int8_t state) { state_ = state; }
    inline void setTarget(int8_t target) { target_ = target; }
    inline void setOptionSelected(int8_t option_selected) { option_selected_ = option_selected; }
    static void SaveWavetable(char* param);
    static void SaveWave(char* param);
    inline void setConfirmFunc(void (ManageMenu::*f)()) { confirm_func_ = f;}
    inline void setCancelFunc(void (ManageMenu::*f)()) { cancel_func_ = f;}
    void ConfirmDeleteFrame();
    void CancelDeleteFrame();
    void ConfirmDeleteWavetable();
    void CancelDeleteWavetable();
    void ConfirmCopyWavetable();
    void CancelCopyWavetable();
    void ConfirmCopyWave();
    void CancelCopyWave();
    void SetLine(int line_no, char* str);
    void ResetTicker();

private:
    int8_t state_;
    int8_t option_selected_;
    int8_t option_offset_;

    int16_t wavetable_offset_;
    int16_t frame_offset_;
    int16_t wavetable_;
    int16_t frame_;

    void (ManageMenu::*confirm_func_)();
    void (ManageMenu::*cancel_func_)();
    char confirm_lines_[3][20];

    int16_t selected_wavetable_;
    int16_t selected_frame_;
    float morph_;
    int8_t target_;
    uint32_t ticker_timer_;
    uint8_t ticker_ = 0;
    uint32_t press_timer_;
    uint32_t blink_timer_;
    bool absorb_keypress_;
    int8_t copy_state_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ManageMenu);
};
