/*
  ==============================================================================

    UserSettings.h
    Created: 15 Nov 2023 1:42:55am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "stmlib/system/system_clock.h"
#include "waves/defines.h"
#include "waves/Storage.h"
#include "waves/Globals.h"

class Storage;

class UserSettings {
public:
    UserSettings();
    ~UserSettings();


    // inline void setBrightness(int8_t brightness) { settings_.brightness = CLAMP<int8_t>(brightness, 0, 100); }
    // inline int8_t getBrightness() {return settings_.brightness;}

    // inline void setContrast(int8_t contrast) { settings_.contrast = CLAMP<int8_t>(contrast, 0, 100); }
    // inline int8_t getContrast() {return settings_.contrast;}

    // inline void setInvert(bool invert) { settings_.invert = invert; }
    // inline bool getInvert() {return settings_.invert;}

    // inline void setScopeSetting(int8_t scope_setting) { settings_.scope_setting = scope_setting; }
    // inline int8_t getScopeSetting() {return settings_.scope_setting;}

    // inline void setMorphSetting(int8_t morph_setting) { settings_.morph_setting = morph_setting; }
    // inline int8_t getMorphSetting() {return settings_.morph_setting;}

    // inline void setSubOscOffset(int8_t offset) { settings_.subosc_offset = CLAMP<int8_t>(offset, -24, 24); }
    // inline int8_t getSubOscOffset() {return settings_.subosc_offset;}

    // inline void setSubOscDetune(int8_t detune) { settings_.subosc_detune = CLAMP<int8_t>(detune, -50, 50); }
    // inline int8_t getSubOscDetune() {return settings_.subosc_detune;}

    // inline void setSubOscMix(uint8_t mix) { settings_.subosc_mix = CLAMP<int8_t>(mix, 0, 100); }
    // inline int8_t getSubOscMix() {return settings_.subosc_mix;}

    // inline void setSubOscWave(int8_t wave) { settings_.subosc_wave = wave; }
    // inline int8_t getSubOscWave() {return settings_.subosc_wave;}
    
    // inline int32_t getAppStartTime() { return app_start_time_; }

    // inline void setIOGain(int8_t channel, float gain) { settings_.io_gain[channel] = CLAMP<float>(gain, 0.1, 10.0); }
    // inline float getIOGain(int8_t channel) { return settings_.io_gain[channel]; }

    // inline void setIOBias(int8_t channel, float bias) { settings_.io_bias[channel] = CLAMP<float>(bias, -1.0, 1.0); }
    // inline float getIOBias(int8_t channel) { return settings_.io_bias[channel]; }


    // inline void setFXDepth(float depth) { settings_.fx_depth = CLAMP<float>(depth, 0.0f, 1.0f); }
    // inline float getFXDepth() { return settings_.fx_depth; }

    // inline void setFXSync(bool sync) { settings_.fx_sync = sync; }
    // inline bool getFXSync() { return settings_.fx_sync; }

    // inline void setFXScale(uint8_t scale) { settings_.fx_scale = CLAMP<uint8_t>(scale, 0, 100); }
    // inline uint8_t getFXScale() { return settings_.fx_scale; }

    // inline void setFXRange(uint8_t range) { settings_.fx_range = CLAMP<uint8_t>(range, 1, 10); }
    // inline uint8_t getFXRange() { return settings_.fx_range; }

    // inline void setFXOscillatorShape(uint8_t shape) { settings_.fx_oscillator_shape = CLAMP<uint8_t>(shape, 0, 5); }
    // inline uint8_t getFXOscillatorShape() { return settings_.fx_oscillator_shape; }

    // inline void setFXControlType(uint8_t type) { settings_.fx_control_type = CLAMP<uint8_t>(type, 0, 2); }
    // inline uint8_t getFXControlType() { return settings_.fx_control_type; }

    // inline void setFXEffect(uint8_t effect) { settings_.fx_effect = CLAMP<uint8_t>(effect, 0, 8); }
    // inline uint8_t getFXEffect() { return settings_.fx_effect; }

    void Calibrate(uint16_t c1, uint16_t c5);
    // inline float getCalibrationX() { return settings_.calibration_x; }
    // inline float getCalibrationY() { return settings_.calibration_y; }

    // inline SNAPSHOT * settings_ptr() { return &settings_; }
    
    float RandomFloat(float min, float max);
    int32_t RandomInt(int32_t min, int32_t max);

    bool ResetSettings();
    bool RandomizeSettings();
    bool SaveSnapshot(const char * name, uint8_t index);
    bool LoadSnapshot(int8_t snapshot);
    // SNAPSHOT settings_;
        
private:
    uint32_t app_start_time_;
       
    DISALLOW_COPY_AND_ASSIGN (UserSettings);
};
