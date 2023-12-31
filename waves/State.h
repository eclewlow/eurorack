/*
  ==============================================================================

    State.h
    Created: 7 Nov 2023 1:06:48pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once


class State {
public:
    State() {}
    virtual ~State() {};

    virtual bool handleKeyPress(int key)        = 0;
    virtual bool handleKeyRelease(int key)      = 0;
    virtual bool handleKeyLongPress(int key)    = 0;
    virtual void paint()       = 0;
    virtual void triggerUpdate(bool back_pressed = false) = 0;
    inline void setBackMenu(State* back_menu) {back_menu_ = back_menu;}

protected:
    State* back_menu_;
    // State(const State&);
    // State& operator=(State const&);
    // DISALLOW_COPY_AND_ASSIGN (State);
};
