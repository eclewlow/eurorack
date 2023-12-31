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

class Storage;

class UserSettings {
public:
    UserSettings();
    ~UserSettings();


typedef struct {
    char name[9];
    uint32_t memory_location;
    bool factory_preset;
    bool is_empty;
} WAVE;

typedef struct {
    char name[9];
    WAVE waves[16];
    bool factory_preset;
    bool is_empty;
} WAVETABLE;

typedef struct {
    char name[9];
    bool factory_preset;
    bool is_empty;

    // misc settings
    int8_t brightness;
    int8_t contrast;
    bool invert;
    int8_t scope_setting;                  // (0, 1)
    int8_t morph_setting;                  // (0, 1)

    // sub osc parameters
    int8_t subosc_offset;                    // (-24, 24)
    int8_t subosc_detune;                    // (-50, 50)
    int8_t subosc_mix;                       // (0, 100)
    int8_t subosc_wave;                      // (0, 5)
    
    // fx parameters
    float fx_depth;                        // (0.0f, 1.0f)
    bool fx_sync;                           // (false, true)
    int8_t fx_scale;                        // (0, 100)
    int8_t fx_range;                        // (1, 10)
    int8_t fx_oscillator_shape;             // (0, 5)
    int8_t fx_control_type;                 // (0, 2)
    int8_t fx_effect;                       // (0, 8)

    int8_t engine;                          // (0, 3)
    // ab engine parameters
    int8_t ab_engine_left_wavetable;        // (0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1)
    int8_t ab_engine_left_frame;            // (0, 15)
    int8_t ab_engine_right_wavetable;       // (0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1)
    int8_t ab_engine_right_frame;           // (0, 15)
    bool ab_engine_is_editing_left;
    bool ab_engine_is_editing_right;
//        bool ab_engine_is_editing; set to false for both left and right

    // wavetable engine parameters
    int8_t wavetable_engine_wavetable;      // (0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1)

    // matrix engine parameters
    int8_t matrix_engine_x1;                // (0, 15)
    int8_t matrix_engine_x2;                // (x1, 15)
    int8_t matrix_engine_y1;                // (0, 15)
    int8_t matrix_engine_y2;                // (y1, 15)
    int8_t matrix_engine_wavelist_offset;   // (0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 16 - 1)

    // drum engine parameters
    float drum_engine_amp_decay;            // (0.0, 1.0)
    float drum_engine_fm_decay;             // (0.0, 1.0)
    float drum_engine_fm_shape;             // (0.0, 1.0)
    float drum_engine_fm_depth;             // (0.0, 1.0)
    int8_t drum_engine_wavetable;           // (0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1)

    // pot positions
    uint16_t pot_fx_amount;                 // (0, 4095)
    uint16_t pot_fx;                        // (0, 4095)
    uint16_t pot_morph;                     // (0, 4095)
    
    // calibration data
    float io_gain[4];                      // (1, 10.0)    // don't randomize this, but save in snapshot
    float io_bias[4];                      // (-1.0, 1.0)  // don't randomize this, but save in snapshot

    float calibration_x;                   // ()  // don't randomize this, but save in snapshot
    float calibration_y;                   // ()  // don't randomize this, but save in snapshot
} SNAPSHOT;

typedef struct {
    WAVETABLE wavetables[USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT];
    SNAPSHOT snapshots[USER_SNAPSHOT_COUNT + FACTORY_SNAPSHOT_COUNT];
} PERSISTENT_STORAGE;

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

    inline void setBrightness(int8_t brightness) { settings_.brightness = CLAMP<int8_t>(brightness, 0, 100); }
    inline int8_t getBrightness() {return settings_.brightness;}

    inline void setContrast(int8_t contrast) { settings_.contrast = CLAMP<int8_t>(contrast, 0, 100); }
    inline int8_t getContrast() {return settings_.contrast;}

    inline void setInvert(bool invert) { settings_.invert = invert; }
    inline bool getInvert() {return settings_.invert;}

    inline void setScopeSetting(int8_t scope_setting) { settings_.scope_setting = scope_setting; }
    inline int8_t getScopeSetting() {return settings_.scope_setting;}

    inline void setMorphSetting(int8_t morph_setting) { settings_.morph_setting = morph_setting; }
    inline int8_t getMorphSetting() {return settings_.morph_setting;}

    inline void setSubOscOffset(int8_t offset) { settings_.subosc_offset = CLAMP<int8_t>(offset, -24, 24); }
    inline int8_t getSubOscOffset() {return settings_.subosc_offset;}

    inline void setSubOscDetune(int8_t detune) { settings_.subosc_detune = CLAMP<int8_t>(detune, -50, 50); }
    inline int8_t getSubOscDetune() {return settings_.subosc_detune;}

    inline void setSubOscMix(uint8_t mix) { settings_.subosc_mix = CLAMP<int8_t>(mix, 0, 100); }
    inline int8_t getSubOscMix() {return settings_.subosc_mix;}

    inline void setSubOscWave(int8_t wave) { settings_.subosc_wave = wave; }
    inline int8_t getSubOscWave() {return settings_.subosc_wave;}
    
    inline int32_t getAppStartTime() { return app_start_time_; }

    inline void setIOGain(int8_t channel, float gain) { settings_.io_gain[channel] = CLAMP<float>(gain, 0.1, 10.0); }
    inline float getIOGain(int8_t channel) { return settings_.io_gain[channel]; }

    inline void setIOBias(int8_t channel, float bias) { settings_.io_bias[channel] = CLAMP<float>(bias, -1.0, 1.0); }
    inline float getIOBias(int8_t channel) { return settings_.io_bias[channel]; }

    void Calibrate(uint16_t c1, uint16_t c5);
    inline float getCalibrationX() { return settings_.calibration_x; }
    inline float getCalibrationY() { return settings_.calibration_y; }

    inline SNAPSHOT * settings_ptr() { return &settings_; }
    
    float RandomFloat(float min, float max);
    int32_t RandomInt(int32_t min, int32_t max);

    bool ResetSettings();
    bool RandomizeSettings();
    bool SaveSnapshot(const char * name, uint8_t index);
    bool LoadSnapshot(int8_t snapshot);
        
private:
    uint32_t app_start_time_;
    SNAPSHOT settings_;
       
    DISALLOW_COPY_AND_ASSIGN (UserSettings);
};
