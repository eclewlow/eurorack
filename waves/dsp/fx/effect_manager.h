/*
  ==============================================================================

    effect_manager.h
    Created: 13 Nov 2023 6:40:12pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "waves/dsp/fx/effect.h"
#include "waves/Globals.h"

namespace waves {

class EffectManager {
public:
    EffectManager() {
//        depth_ = 1.0f;
//        sync_ = false;
//        oscillator_shape_ = SINE_SHAPE;
//        control_type_ = INTERNAL_MODULATOR;
    }
    ~EffectManager() {}
    void Init();
    void Reset();
    
    void setEffect(uint8_t effect_type);
    Effect* getEffect();

    int16_t RenderSampleEffect(int16_t sample, float phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscillcope=false);

    float RenderSampleEffect(float sample, float phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscillcope=false);
    float RenderPhaseEffect(float phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscillcope=false);
    float GetSample(float phase);
    
    uint8_t getEffectType();
    int8_t getControlType();
    void setControlType(int8_t control_type);
    int8_t getOscillatorShape();
    void setOscillatorShape(int8_t oscillator_shape);
    bool getSync();
    void setSync(bool sync);
    float getDepth();
    void setDepth(float depth);
    int getScale();
    void setScale(int scale);
    int getRange();
    void setRange(int range);

protected:
//    float depth_;
//    bool sync_;
//    int scale_;
//    int range_;
//    int8_t oscillator_shape_;
//    int8_t control_type_;
//    Effect* effect_;
private:
    EffectManager(const EffectManager&);
    EffectManager& operator=(EffectManager const&);
    // DISALLOW_COPY_AND_ASSIGN (EffectManager);
};


}