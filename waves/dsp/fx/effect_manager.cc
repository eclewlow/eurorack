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

int16_t EffectManager::RenderSampleEffect(int16_t sample, float phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {

    switch(settings_.fx_effect) {
        case EFFECT_TYPE_BYPASS:
            sample = bypass.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_FM:
            sample = fm.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_RING_MODULATOR:
            sample = ring_modulator.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_PHASE_DISTORTION:
            sample = phase_distortion.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_WAVEFOLDER:
            sample = wavefolder.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_WAVEWRAPPER:
            sample = wavewrapper.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_BITCRUSH:
            sample = bitcrush.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_DRIVE:
            sample = drive.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
    }
    return sample;
}

float EffectManager::RenderSampleEffect(float sample, float phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {

    switch(settings_.fx_effect) {
        case EFFECT_TYPE_BYPASS:
            sample = bypass.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_FM:
            sample = fm.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_RING_MODULATOR:
            sample = ring_modulator.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_PHASE_DISTORTION:
            sample = phase_distortion.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_WAVEFOLDER:
            sample = wavefolder.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_WAVEWRAPPER:
            sample = wavewrapper.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_BITCRUSH:
            sample = bitcrush.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_DRIVE:
            sample = drive.RenderSampleEffect(sample, phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
    }
    return sample;
}
float EffectManager::RenderPhaseEffect(float phase, float phase_increment, uint16_t fx_amount, uint16_t fx, bool isOscilloscope, bool downsampling) {

    switch(settings_.fx_effect) {
        case EFFECT_TYPE_BYPASS:
            phase = bypass.RenderPhaseEffect(phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_FM:
            phase = fm.RenderPhaseEffect(phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_RING_MODULATOR:
            phase = ring_modulator.RenderPhaseEffect(phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_PHASE_DISTORTION:
            phase = phase_distortion.RenderPhaseEffect(phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_WAVEFOLDER:
            phase = wavefolder.RenderPhaseEffect(phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_WAVEWRAPPER:
            phase = wavewrapper.RenderPhaseEffect(phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_BITCRUSH:
            phase = bitcrush.RenderPhaseEffect(phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
        case EFFECT_TYPE_DRIVE:
            phase = drive.RenderPhaseEffect(phase, phase_increment, fx_amount, fx, isOscilloscope, downsampling);
            break;
    }
    return phase;
}

void EffectManager::setEffect(uint8_t effect_type) {
    Effect* effect;
    settings_.fx_effect = effect_type;
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
    if(effect)
        effect->triggerUpdate();
}

Effect* EffectManager::getEffect() {
    Effect* effect = NULL;

    switch(settings_.fx_effect) {
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

uint8_t EffectManager::getEffectType() {
    return settings_.fx_effect;
}

int8_t EffectManager::getControlType() { return settings_.fx_control_type; }
void EffectManager::setControlType(int8_t control_type) { settings_.fx_control_type = control_type; }

int8_t EffectManager::getOscillatorShape() { return settings_.fx_oscillator_shape; }
void EffectManager::setOscillatorShape(int8_t oscillator_shape) { settings_.fx_oscillator_shape = oscillator_shape; }

bool EffectManager::getSync() { return settings_.fx_sync; }
void EffectManager::setSync(bool sync) { settings_.fx_sync = sync; }

float EffectManager::getDepth() { return settings_.fx_depth; }
void EffectManager::setDepth(float depth) { settings_.fx_depth = CLAMP(depth, 0.0f, 1.0f); }

int EffectManager::getScale() { return settings_.fx_scale; }
void EffectManager::setScale(int scale) { settings_.fx_scale = CLAMP<int>(scale, 0, 100); }

int EffectManager::getRange() { return settings_.fx_range; }
void EffectManager::setRange(int range) { settings_.fx_range = CLAMP<int>(range, 1, 10); }
