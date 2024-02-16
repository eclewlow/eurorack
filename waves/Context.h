/*
  ==============================================================================

    Context.h
    Created: 7 Nov 2023 1:06:53pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/Globals.h"

namespace waves {

class Context {
public:
    Context();
    ~Context();
        
    inline void setState(State* state, bool back_pressed = false) { if(_state == state) return; _state = state; _state->triggerUpdate(back_pressed); }
    inline State* getState() { return _state; }
    
    void setEngine(int8_t engine);
    Engine* getEngine();
    Engine* getLastEngine();
    inline void setLastEngine(int8_t engine) { last_engine_ = engine; }

    bool handleKeyPress(int key);
    bool handleKeyLongPress(int key);
    bool handleKeyRelease(int key);
    void paint();

private:
    State* _state;
    int8_t last_engine_;

    Context(const Context&);
    Context& operator=(Context const&);
    // DISALLOW_COPY_AND_ASSIGN (Context);
};

}