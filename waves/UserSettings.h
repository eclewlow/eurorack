/*
  ==============================================================================

    UserSettings.h
    Created: 15 Nov 2023 1:42:55am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Storage.h"

class Storage;

class UserSettings {
public:
    UserSettings();
    ~UserSettings();

    enum SettingScope {
        SETTING_SCOPE_LINE = 0,
        SETTING_SCOPE_FILL = 1,
    };
    enum SettingMorph {
        SETTING_MORPH_SMOOTH    = 0,
        SETTING_MORPH_DISCRETE  = 1,
    };

    enum SubOscWave {
        SUBOSC_WAVE_SINE        = 0,
        SUBOSC_WAVE_TRIANGLE    = 1,
        SUBOSC_WAVE_SAWTOOTH    = 2,
        SUBOSC_WAVE_RAMP        = 3,
        SUBOSC_WAVE_SQUARE      = 4,
        SUBOSC_WAVE_COPY        = 5,
        SUBOSC_WAVE_LAST        = 6,
    };
    
    enum IOConfigurationMenuGain {
        IO_CONFIG_GAIN_1V   = 0,
        IO_CONFIG_GAIN_2V5  = 1,
        IO_CONFIG_GAIN_5V   = 2,
        IO_CONFIG_GAIN_10V  = 3,
        IO_CONFIG_GAIN_LAST = 4,
    };

    inline void setBrightness(int8_t brightness) { settings_.brightness = std::clamp<int8_t>(brightness, 0, 100); }
    inline int8_t getBrightness() {return settings_.brightness;}

    inline void setContrast(int8_t contrast) { settings_.contrast = std::clamp<int8_t>(contrast, 0, 100); }
    inline int8_t getContrast() {return settings_.contrast;}

    inline void setInvert(bool invert) { settings_.invert = invert; }
    inline bool getInvert() {return settings_.invert;}

    inline void setScopeSetting(int8_t scope_setting) { settings_.scope_setting = scope_setting; }
    inline int8_t getScopeSetting() {return settings_.scope_setting;}

    inline void setMorphSetting(int8_t morph_setting) { settings_.morph_setting = morph_setting; }
    inline int8_t getMorphSetting() {return settings_.morph_setting;}

    inline void setSubOscOffset(int8_t offset) { settings_.subosc_offset = std::clamp<int8_t>(offset, -24, 24); }
    inline int8_t getSubOscOffset() {return settings_.subosc_offset;}

    inline void setSubOscDetune(int8_t detune) { settings_.subosc_detune = std::clamp<int8_t>(detune, -50, 50); }
    inline int8_t getSubOscDetune() {return settings_.subosc_detune;}

    inline void setSubOscMix(uint8_t mix) { settings_.subosc_mix = std::clamp<int8_t>(mix, 0, 100); }
    inline int8_t getSubOscMix() {return settings_.subosc_mix;}

    inline void setSubOscWave(int8_t wave) { settings_.subosc_wave = wave; }
    inline int8_t getSubOscWave() {return settings_.subosc_wave;}
    
    inline int32_t getAppStartTime() { return app_start_time_; }

    inline void setIOGain(int8_t channel, float gain) { settings_.io_gain[channel] = std::clamp<float>(gain, 0.1, 10.0); }
    inline float getIOGain(int8_t channel) { return settings_.io_gain[channel]; }

    inline void setIOBias(int8_t channel, float bias) { settings_.io_bias[channel] = std::clamp<float>(bias, -1.0, 1.0); }
    inline float getIOBias(int8_t channel) { return settings_.io_bias[channel]; }

    void Calibrate(uint16_t c1, uint16_t c5);
    inline float getCalibrationX() { return settings_.calibration_x; }
    inline float getCalibrationY() { return settings_.calibration_y; }

    inline Storage::SNAPSHOT * settings_ptr() { return &settings_; }
    
    float RandomFloat(float min, float max);
    int32_t RandomInt(int32_t min, int32_t max);

    bool ResetSettings();
    bool RandomizeSettings();
    bool SaveSnapshot(const char * name, uint8_t index);
    bool LoadSnapshot(int8_t snapshot);
        
private:
    uint32_t app_start_time_;
    Storage::SNAPSHOT settings_;
       
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UserSettings);
};
