/*
  ==============================================================================

    SnapshotMenu.h
    Created: 8 Dec 2023 8:42:11pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"

class State;

namespace waves {

class SnapshotMenu: public State
{
public:
    enum SnapshotMenuState {
        SNAPSHOT_MENU_NONE      = 0,
        SNAPSHOT_MENU_OPTIONS   = 1,
        SNAPSHOT_MENU_CONFIRM   = 2,
    };
    enum SnapshotMenuOptions {
        SNAPSHOT_MENU_SAVE              = 0,
        SNAPSHOT_MENU_COPY              = 1,
        SNAPSHOT_MENU_RENAME            = 2,
        SNAPSHOT_MENU_DELETE            = 3,
        SNAPSHOT_MENU_NO                = 4,
        SNAPSHOT_MENU_YES               = 5,
    };
    
    enum SnapshotMenuFunctions {
        SNAPSHOT_MENU_DICE              = 0,
        SNAPSHOT_MENU_HOME              = 1,
    };
    
    enum SnapshotMenuCopyState {
        SNAPSHOT_MENU_COPY_STATE_NONE           = 0,
        SNAPSHOT_MENU_COPY_STATE_SNAPSHOT       = 1,
    };



    SnapshotMenu();
    ~SnapshotMenu();
    virtual bool handleKeyPress(int key);
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key);
    virtual void paint();
    void triggerUpdate(bool back_pressed);
    inline void setState(int8_t state) { state_ = state; }
    inline void setOptionSelected(int8_t option_selected) { option_selected_ = option_selected; }
    void ResetTicker();

    inline void setConfirmFunc(void (SnapshotMenu::*f)()) { confirm_func_ = f;}
    inline void setCancelFunc(void (SnapshotMenu::*f)()) { cancel_func_ = f;}

    static void Save(char* param);
    static void Rename(char* param);

    void ConfirmDelete();
    void CancelDelete();
    void ConfirmCopy();
    void CancelCopy();

    void SetLine(int line_no, char* str);

private:

    int8_t state_;

    int16_t snapshot_offset_;
    int16_t snapshot_;

    int8_t option_selected_;
    int8_t option_offset_;

    int16_t selected_snapshot_;

    uint32_t press_timer_;
    bool absorb_keypress_;

    int8_t function_selected_;

    uint32_t ticker_timer_;
    uint8_t ticker_;

    int8_t copy_state_;
    uint32_t blink_timer_;

    void (SnapshotMenu::*confirm_func_)();
    void (SnapshotMenu::*cancel_func_)();
    char confirm_lines_[3][20];

    
    DISALLOW_COPY_AND_ASSIGN (SnapshotMenu);
};

}