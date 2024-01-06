/*
  ==============================================================================

    Effect.cpp
    Created: 12 Nov 2023 4:26:44pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/fx/effect.h"
#include "waves/Globals.h"

void Effect::Init() {
    phase_ = 0.0f;
    oscilloscope_phase_ = 0.0f;
}

void Effect::Reset() {
    phase_ = 0.0f;
    oscilloscope_phase_ = 0.0f;
}
