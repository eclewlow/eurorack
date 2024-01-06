
#pragma once


#include "stmlib/stmlib.h"
#include <stm32f4xx_conf.h>


#define USER_WAVE_COUNT         512
#define FACTORY_WAVE_COUNT      512
    
#define USER_SNAPSHOT_COUNT     64
#define FACTORY_SNAPSHOT_COUNT  0
    
#define USER_WAVETABLE_COUNT    32
#define FACTORY_WAVETABLE_COUNT 16


#define _RXTC_  0 // Rx transfer complete
#define _TXTC_  1 // Tx transfer complete
#define _BUSY_  2 // Busy processing
#define _RXNE_  3 // Receive Buffer is full

#define EEPROM_FACTORY_SS       0
#define EEPROM_USER_SS          1
#define EEPROM_PERSISTENT_SS    2
#define EEPROM_CLOCK      3
#define EEPROM_MOSI       4
#define EEPROM_MISO       5
#define LCD_SS          6
#define LCD_CMD         7
#define LCD_CLOCK       8
#define LCD_MOSI        9
#define LCD_RESET       10



#define KEYCODE_A   65
#define KEYCODE_B   66
#define KEYCODE_C   67
#define KEYCODE_D   68
#define KEYCODE_E   69
#define KEYCODE_F   70
#define KEYCODE_G   71
#define KEYCODE_H   72
#define KEYCODE_I   73
#define KEYCODE_J   74
#define KEYCODE_K   75
#define KEYCODE_L   76
#define KEYCODE_M   77
#define KEYCODE_N   78
#define KEYCODE_O   79
#define KEYCODE_P   80
#define KEYCODE_Q   81
#define KEYCODE_R   82
#define KEYCODE_S   83
#define KEYCODE_T   84
#define KEYCODE_U   85
#define KEYCODE_V   86
#define KEYCODE_W   87
#define KEYCODE_X   88
#define KEYCODE_Y   89
#define KEYCODE_Z   90
#define KEYCODE_ENTER   13
#define KEYCODE_ESCAPE  27

#define LEFT_ENCODER_CCW        KEYCODE_Q
#define LEFT_ENCODER_CLICK      KEYCODE_W
#define LEFT_ENCODER_CW         KEYCODE_E

#define RIGHT_ENCODER_CCW       KEYCODE_I
#define RIGHT_ENCODER_CLICK     KEYCODE_O
#define RIGHT_ENCODER_CW        KEYCODE_P

#define PITCH_POT_CCW           KEYCODE_A
#define PITCH_POT_CW            KEYCODE_S

#define FX_AMOUNT_POT_CCW       KEYCODE_D
#define FX_AMOUNT_POT_CW        KEYCODE_F

#define FX_POT_CCW              KEYCODE_G
#define FX_POT_CW               KEYCODE_H

#define MORPH_POT_CCW           KEYCODE_J
#define MORPH_POT_CW            KEYCODE_K

#define FX_AMOUNT_CV_TRIGGER    KEYCODE_U

#define BACK_BUTTON             KEYCODE_ESCAPE


template <typename T> T CLAMP(T value, T low, T high)
{
    return (value < low) ? low : ((value > high) ? high : value);
}


typedef struct {
    GPIO_TypeDef* gpio;
    uint16_t pin;
} EEPROM_PIN;

typedef enum
{
  TM1637 = 0,
  TM1803 = 1
} DisplayTypeTypeDef;

typedef enum
{
  FLAG_SET = 1,
  FLAG_CLEAR = 0
} FlagStateTypeDef;

typedef enum
{
  DIR_NEUTRAL = 2,
  DIR_READ = 1,
  DIR_WRITE = 0
} DataDirectionTypeDef;

typedef enum
{
  ERR_0 = 0, 
  ERR_1 = 1, 
  ERR_2 = 2, 
  ERR_3 = 3, 
  ERR_4 = 4,
  ERR_5 = 5,
  ERR_6 = 6,
  ERR_7 = 7,
  ERR_8 = 8,
  ERR_9 = 9,
  NO_ERR = 0xff
} ErrorState;

typedef enum {
    LOADING_DONE            		= 0,
    LOADING_AB_LEFT 	 			= 1,
    LOADING_AB_RIGHT     			= 2,
    LOADING_WAVETABLE_DOUBLE_FRAME	= 3,
    LOADING_MATRIX_DOULE_FRAME_X  	= 4,
    LOADING_MATRIX_DOULE_FRAME_Y  	= 5,
    LOADING_DRUM_DOUBLE_FRAME		= 6,
}  LoadingState;

enum OscillatorShape {
    SINE_SHAPE              = 0,
    TRIANGLE_SHAPE          = 1,
    SAWTOOTH_SHAPE          = 2,
    RAMP_SHAPE              = 3,
    SQUARE_SHAPE            = 4,
    SNH_SHAPE               = 5,
    OSCILLATOR_SHAPE_LAST   = 6,
};

enum ControlType {
    MANUAL_CONTROL      = 0,
    EXTERNAL_MODULATOR  = 1,
    INTERNAL_MODULATOR  = 2,
    CONTROL_TYPE_LAST   = 3,
};

enum EffectType {
    EFFECT_TYPE_BYPASS              = 0,
    EFFECT_TYPE_FM                  = 1,
    EFFECT_TYPE_RING_MODULATOR      = 2,
    EFFECT_TYPE_PHASE_DISTORTION    = 3,
    EFFECT_TYPE_WAVEFOLDER          = 4,
    EFFECT_TYPE_WAVEWRAPPER         = 5,
    EFFECT_TYPE_BITCRUSH            = 6,
    EFFECT_TYPE_DRIVE               = 7,
};

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

enum EngineType {
    ENGINE_TYPE_AB          = 0,
    ENGINE_TYPE_WAVETABLE   = 1,
    ENGINE_TYPE_MATRIX      = 2,
    ENGINE_TYPE_DRUM        = 3,
};


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

