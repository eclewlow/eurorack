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
        phase_ = 0.0f;
        oscilloscope_phase_ = 0.0f;
    }
    virtual ~Effect() {}
    virtual void Init() = 0;
    virtual void Reset() = 0;
    virtual float RenderSampleEffect(float sample, float phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) = 0;
    virtual float RenderPhaseEffect(float phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) = 0;
    inline void Sync_phases() { oscilloscope_phase_ = phase_; }
    
    inline void triggerUpdate() { /*phase = 0.0f; oscilloscopePhase = 0.0f;*/ }
protected:
    float phase_;
    float oscilloscope_phase_;
private:
    // Effect(const Effect&);
    // Effect& operator=(Effect const&);
    // DISALLOW_COPY_AND_ASSIGN (Effect);
};
