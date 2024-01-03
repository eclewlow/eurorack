/*
  ==============================================================================

    Context.cpp
    Created: 7 Nov 2023 1:06:53pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Context.h"
#include "waves/Globals.h"

Context::Context() {_state = NULL;}
Context::~Context() {}

bool Context::handleKeyPress(int key) {
    if(_state)
        return _state->handleKeyPress(key);
    else
        return false;
}

bool Context::handleKeyLongPress(int key) {
    if(_state)
        return _state->handleKeyLongPress(key);
    else
        return false;
}

bool Context::handleKeyRelease(int key) {
    if(_state)
        return _state->handleKeyRelease(key);
    else
        return false;
}


void Context::paint() {
    if(_state)
        _state->paint();
}

void Context::setEngine(int8_t engine) { 
    // loading=41;
    if(engine == user_settings.settings_ptr()->engine)
        return;
    
    last_engine_ = user_settings.settings_ptr()->engine;
    // loading=42;

    user_settings.settings_ptr()->engine = engine;

    switch(user_settings.settings_ptr()->engine) {
        case Context::ENGINE_TYPE_AB:
            abEngine.triggerUpdate();
            break;
        case Context::ENGINE_TYPE_WAVETABLE:
            wavetableEngine.triggerUpdate();
            break;
        case Context::ENGINE_TYPE_MATRIX:
            matrixEngine.triggerUpdate();
            break;
        case Context::ENGINE_TYPE_DRUM:
            drumEngine.triggerUpdate();
            break;
    }
}

Engine* Context::getEngine() {
    Engine * engine = NULL;
    switch(user_settings.settings_ptr()->engine) {
        case Context::ENGINE_TYPE_AB:
            engine = &abEngine;
            break;
        case Context::ENGINE_TYPE_WAVETABLE:
            engine = &wavetableEngine;
            break;
        case Context::ENGINE_TYPE_MATRIX:
            engine = &matrixEngine;
            break;
        case Context::ENGINE_TYPE_DRUM:
            engine = &drumEngine;
            break;
    }
    return engine;
}
Engine* Context::getLastEngine() { 
    Engine * engine;
    switch(last_engine_) {
        case Context::ENGINE_TYPE_AB:
            engine = &abEngine;
            break;
        case Context::ENGINE_TYPE_WAVETABLE:
            engine = &wavetableEngine;
            break;
        case Context::ENGINE_TYPE_MATRIX:
            engine = &matrixEngine;
            break;
        case Context::ENGINE_TYPE_DRUM:
            engine = &drumEngine;
            break;
        default:
            engine = NULL;
            break;
    }
    return engine;
}
