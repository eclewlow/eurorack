/*
  ==============================================================================

    effect_manager.cpp
    Created: 13 Nov 2023 6:40:12pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/fx/effect_manager.h"
#include "waves/Globals.h"

void EffectManager::Init() {
}

void EffectManager::Reset() {
}

float EffectManager::RenderSampleEffect(float sample, float phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {
    if(getEffect()) {
        return getEffect()->RenderSampleEffect(sample, phase, frequency, fx_amount, fx, isOscilloscope, downsampling);
    }
    else {
        return sample;
    }
}
float EffectManager::RenderPhaseEffect(float phase, float frequency, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {
    if(getEffect()) {
        return getEffect()->RenderPhaseEffect(phase, frequency, fx_amount, fx, isOscilloscope, downsampling);
    }
    else {
        return phase;
    }
}

void EffectManager::setEffect(uint8_t effect_type) {
    Effect* effect;
    user_settings.settings_ptr()->fx_effect = effect_type;
    switch(effect_type) {
        case EFFECT_TYPE_BYPASS:
            effect = &bypass;
            break;
        case EFFECT_TYPE_FM:
            effect = &fm;
            break;
        case EFFECT_TYPE_RING_MODULATOR:
            effect = &ring_modulator;
            break;
        case EFFECT_TYPE_PHASE_DISTORTION:
            effect = &phase_distortion;
            break;
        case EFFECT_TYPE_WAVEFOLDER:
            effect = &wavefolder;
            break;
        case EFFECT_TYPE_WAVEWRAPPER:
            effect = &wavewrapper;
            break;
        case EFFECT_TYPE_BITCRUSH:
            effect = &bitcrush;
            break;
        case EFFECT_TYPE_DRIVE:
            effect = &drive;
            break;
    }
    effect->triggerUpdate();
    
}

Effect* EffectManager::getEffect() {
    Effect* effect = NULL;

    switch(user_settings.settings_ptr()->fx_effect) {
        case EFFECT_TYPE_BYPASS:
            effect = &bypass;
            break;
        case EFFECT_TYPE_FM:
            effect = &fm;
            break;
        case EFFECT_TYPE_RING_MODULATOR:
            effect = &ring_modulator;
            break;
        case EFFECT_TYPE_PHASE_DISTORTION:
            effect = &phase_distortion;
            break;
        case EFFECT_TYPE_WAVEFOLDER:
            effect = &wavefolder;
            break;
        case EFFECT_TYPE_WAVEWRAPPER:
            effect = &wavewrapper;
            break;
        case EFFECT_TYPE_BITCRUSH:
            effect = &bitcrush;
            break;
        case EFFECT_TYPE_DRIVE:
            effect = &drive;
            break;
    }
    return effect;
}


int8_t EffectManager::getControlType() { return user_settings.settings_ptr()->fx_control_type; }
void EffectManager::setControlType(int8_t control_type) { user_settings.settings_ptr()->fx_control_type = control_type; }

int8_t EffectManager::getOscillatorShape() { return user_settings.settings_ptr()->fx_oscillator_shape; }
void EffectManager::setOscillatorShape(int8_t oscillator_shape) { user_settings.settings_ptr()->fx_oscillator_shape = oscillator_shape; }

bool EffectManager::getSync() { return user_settings.settings_ptr()->fx_sync; }
void EffectManager::setSync(bool sync) { user_settings.settings_ptr()->fx_sync = sync; }

float EffectManager::getDepth() { return user_settings.settings_ptr()->fx_depth; }
void EffectManager::setDepth(float depth) { user_settings.settings_ptr()->fx_depth = CLAMP(depth, 0.0f, 1.0f); }

int EffectManager::getScale() { return user_settings.settings_ptr()->fx_scale; }
void EffectManager::setScale(int scale) { user_settings.settings_ptr()->fx_scale = CLAMP<int>(scale, 0, 100); }

int EffectManager::getRange() { return user_settings.settings_ptr()->fx_range; }
void EffectManager::setRange(int range) { user_settings.settings_ptr()->fx_range = CLAMP<int>(range, 1, 10); }
