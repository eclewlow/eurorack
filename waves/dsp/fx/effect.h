/*
  ==============================================================================

    Effect.h
    Created: 12 Nov 2023 4:26:44pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "stdint.h"

class Effect {
public:
    Effect() {
        phase = 0.0f;
        oscilloscopePhase = 0.0f;
    }
    virtual ~Effect() {}
    virtual void Init() = 0;
    virtual void Reset() = 0;
    virtual float RenderSampleEffect(float sample, float phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) = 0;
    virtual float RenderPhaseEffect(float phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) = 0;
    inline void Sync_phases() { oscilloscopePhase = phase; }
    
    inline void triggerUpdate() { /*phase = 0.0f; oscilloscopePhase = 0.0f;*/ }
protected:
    float phase;
    float oscilloscopePhase;
private:
    // Effect(const Effect&);
    // Effect& operator=(Effect const&);
    // DISALLOW_COPY_AND_ASSIGN (Effect);
};
