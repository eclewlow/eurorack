/*
  ==============================================================================

    Globals.h
    Created: 7 Nov 2023 5:16:51pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "stdint.h"
#include "defines.h"

inline float clamp(float input, float low, float high) {
    return (input < low) ? low : ((input > high) ? high : input);
}


extern uint32_t _EREG_;
extern FlagStateTypeDef GetFlag(uint32_t*, uint8_t);
extern void SetFlag(uint32_t*, uint8_t, FlagStateTypeDef);

extern EEPROM_PIN eeprom[11];

extern int16_t dataBuffer[2048];
extern int16_t double_waveframe_buffer_1[4096];
extern int16_t double_waveframe_buffer_2[4096];
extern int16_t double_waveframe_buffer_3[4096];
extern int16_t double_waveframe_buffer_4[4096];

extern int16_t triple_waveframe_buffer_1[6144];
extern int16_t triple_waveframe_buffer_2[6144];
extern int16_t triple_waveframe_buffer_3[6144];
extern int16_t triple_waveframe_buffer_4[6144];
extern int16_t triple_waveframe_buffer_5[6144];
extern int16_t triple_waveframe_buffer_6[6144];

extern int16_t * matrix_front_buffer_1;
extern int16_t * matrix_front_buffer_2;
extern int16_t * matrix_front_buffer_3;
extern int16_t * matrix_back_buffer_1;
extern int16_t * matrix_back_buffer_2;
extern int16_t * matrix_back_buffer_3;

extern int16_t * front_buffer;
extern int16_t * back_buffer;
extern int16_t * front_buffer_1;
extern int16_t * front_buffer_2;
extern int16_t * back_buffer_1;
extern int16_t * back_buffer_2;
extern int16_t pump_buffer[6144];
extern float loading;
extern uint8_t lcd_buffer[8][128];

extern const float lut_4x_downsampler_fir[];
// extern float lut_4x_downsampler_fir[];

extern int16_t ROM[32768];

// #include "waves/menu/MainMenu.h"
// #include "waves/menu/ModeMenu.h"
// #include "waves/menu/FxMenu.h"
// #include "waves/menu/Oscilloscope.h"
// #include "waves/menu/ControlStatusMenu.h"
// #include "waves/menu/DeviceConfigurationMenu.h"
// #include "waves/menu/InfoMenu.h"
// #include "waves/menu/DisplayConfigurationMenu.h"
// #include "waves/menu/OscilloscopeConfigurationMenu.h"
// #include "waves/menu/SubOscillatorMenu.h"
// #include "waves/menu/EnterNameMenu.h"
// #include "waves/menu/WavetableModeMenu.h"
// #include "waves/menu/ABModeMenu.h"
// #include "waves/menu/WaveEditor.h"
// #include "waves/menu/LoadWaveMenu.h"
// #include "waves/menu/SaveWaveMenu.h"
// #include "waves/menu/Popup.h"
// #include "waves/menu/MatrixMode.h"
// #include "waves/menu/DrumMode.h"
// #include "waves/menu/IOConfigurationMenu.h"
// #include "waves/menu/CalibrationMenu.h"
// #include "waves/menu/QuantizerMenu.h"
// #include "waves/menu/ManageMenu.h"
// #include "waves/menu/SnapshotMenu.h"
// #include "waves/menu/MemoryClearMenu.h"

// #include "adc.h"
#include "waves/drivers/wavetables.h"
#include "stmlib/system/system_clock.h"
#include "waves/drivers/adc.h"
#include "waves/drivers/audio_dac.h"
#include "waves/drivers/flash.h"

#include "waves/dsp/Engine.h"

#include "waves/dsp/ab_engine.h"
#include "waves/dsp/wavetable_engine.h"
#include "waves/dsp/matrix_engine.h"
#include "waves/dsp/drum_engine.h"
#include "waves/dsp/suboscillator.h"

#include "waves/dsp/fx/effect_manager.h"
#include "waves/dsp/fx/fm.h"
#include "waves/dsp/fx/phase_distortion.h"
#include "waves/dsp/fx/ring_modulator.h"
#include "waves/dsp/fx/wavefolder.h"
#include "waves/dsp/fx/wavewrapper.h"
#include "waves/dsp/fx/bitcrush.h"
#include "waves/dsp/fx/drive.h"
#include "waves/dsp/fx/bypass.h"

#include "waves/dsp/quantizer/quantizer.h"

#include "waves/State.h"
#include "waves/Context.h"
#include "waves/UserSettings.h"

#include "waves/dsp/fx/effect.h"
#include "waves/dsp/Engine.h"
#include "stdint.h"

// #include "waves/Storage.h"

using namespace waves;

extern Context context;
// extern UserSettings user_settings;
// extern SNAPSHOT settings_;
// extern Storage storage;
// extern uint8_t effect;
// extern uint8_t sub_osc_shape;
extern int8_t engine;
extern int8_t switch_1_test;
extern int8_t switch_2_test;
// extern MainMenu mainMenu;
// extern ModeMenu modeMenu;
// extern FxMenu fxMenu;
// extern Oscilloscope oscilloscope;
// extern ControlStatusMenu controlStatusMenu;
// extern DeviceConfigurationMenu deviceConfigurationMenu;
// extern InfoMenu infoMenu;
// extern DisplayConfigurationMenu displayConfigurationMenu;
// extern OscilloscopeConfigurationMenu oscilloscopeConfigurationMenu;
// extern SubOscillatorMenu subOscillatorMenu;
// extern EnterNameMenu enterNameMenu;
// extern WavetableModeMenu wavetableModeMenu;
// extern ABModeMenu abModeMenu;
// extern WaveEditor waveEditor;
// extern LoadWaveMenu loadWaveMenu;
// extern SaveWaveMenu saveWaveMenu;
// extern Popup popup;
// extern MatrixMode matrixMode;
// extern DrumMode drumMode;
// extern IOConfigurationMenu ioConfigurationMenu;
// extern CalibrationMenu calibrationMenu;
// extern QuantizerMenu quantizerMenu;
// extern ManageMenu manageMenu;
// extern SnapshotMenu snapshotMenu;
// extern MemoryClearMenu memoryClearMenu;

extern ABEngine abEngine;
extern WavetableEngine wavetableEngine;
extern MatrixEngine matrixEngine;
extern DrumEngine drumEngine;
extern Suboscillator suboscillator;

// extern EffectManager effect_manager;
extern FM fm;
extern PhaseDistortion phase_distortion;
extern RingModulator ring_modulator;
extern Wavefolder wavefolder;
extern Wavewrapper wavewrapper;
extern Bitcrush bitcrush;
extern Drive drive;
extern Bypass bypass;

extern Quantizer quantizer;

// extern Adc adc;
// extern SystemClock system_clock;
extern Flash flash;

extern int16_t BUF1[2048];
extern int16_t BUF2[2048];
extern int16_t BUF3[2048];
extern int16_t BUF4[2048];
extern int16_t BUF5[2048];

