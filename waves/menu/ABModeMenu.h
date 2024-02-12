/*
  ==============================================================================

    ABWaveSelectMenu.h
    Created: 18 Nov 2023 8:29:46pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/State.h"
#include "waves/Globals.h"

class State;

namespace waves {

class ABModeMenu: public State
{
public:
    enum ABMenuState {
        AB_NONE,
        AB_LOAD_HOVER,
        AB_EDIT_HOVER,
        AB_SELECT_WAVETABLE,
        AB_SELECT_FRAME,
        AB_EDIT,
    };
    enum ABActiveMenuSide {
        LEFT,
        RIGHT,
        NONE
    };
    ABModeMenu();
    ~ABModeMenu();
    virtual bool handleKeyPress(int key) { return false; }
    virtual bool handleKeyRelease(int key);
    virtual bool handleKeyLongPress(int key) { return false; }
    virtual void paint();
    void triggerUpdate(bool back_pressed) {};
    void DrawSide(int side);
//    inline void setLeftState(ABMenuState state) { left_state_ = state; }
//    inline void setRightState(ABMenuState state) { right_state_ = state; }
//
//    inline void SetLeftWavetable(int left_wavetable) { left_wavetable_ = CLAMP(left_wavetable, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1); }
//    inline void SetLeftFrame(int left_frame) { left_frame_ = CLAMP(left_frame, 0, 15); }
//    inline int GetLeftWavetable() { return left_wavetable_; }
//    inline int GetLeftFrame() { return left_frame_; }
//
//    inline void SetRightWavetable(int right_wavetable) { right_wavetable_ = CLAMP(right_wavetable, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1); }
//    inline void SetRightFrame(int right_frame) { right_frame_ = CLAMP(right_frame, 0, 15); }
//    inline int GetRightWavetable() { return right_wavetable_; }
//    inline int GetRightFrame() { return right_frame_; }
    void ResetTicker(int8_t side);

    static void on_load_one_ab_left_finished();
    static void on_load_one_ab_right_finished();

private:
    int16_t left_wavetable_offset_;
    int16_t left_frame_offset_;
    int16_t right_wavetable_offset_;
    int16_t right_frame_offset_;
    ABActiveMenuSide active_menu_;
    ABMenuState left_state_;
    ABMenuState right_state_;
    int left_wavetable_;
    int left_frame_;
    int right_wavetable_;
    int right_frame_;
    uint32_t left_ticker_timer_;
    uint32_t right_ticker_timer_;
    uint8_t left_ticker_;
    uint8_t right_ticker_;

    DISALLOW_COPY_AND_ASSIGN (ABModeMenu);
};

}
