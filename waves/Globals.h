/*
  ==============================================================================

    Globals.h
    Created: 7 Nov 2023 5:16:51pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once


#define USER_WAVE_COUNT         512
#define FACTORY_WAVE_COUNT      512
    
#define USER_SNAPSHOT_COUNT     64
#define FACTORY_SNAPSHOT_COUNT  0
    
#define USER_WAVETABLE_COUNT    32
#define FACTORY_WAVETABLE_COUNT 16
    

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

// #include "MainMenu.h"
// #include "ModeMenu.h"
// #include "FxMenu.h"
// #include "Oscilloscope.h"
// #include "ControlStatusMenu.h"
// #include "DeviceConfigurationMenu.h"
// #include "InfoMenu.h"
// #include "DisplayConfigurationMenu.h"
// #include "OscilloscopeConfigurationMenu.h"
// #include "SubOscillatorMenu.h"
// #include "EnterNameMenu.h"
// #include "WavetableModeMenu.h"
// #include "ABModeMenu.h"
// #include "WaveEditor.h"
// #include "LoadWaveMenu.h"
// #include "SaveWaveMenu.h"
// #include "Popup.h"
// #include "MatrixMode.h"
// #include "DrumMode.h"
// #include "IOConfigurationMenu.h"
// #include "CalibrationMenu.h"
// #include "QuantizerMenu.h"
// #include "ManageMenu.h"
// #include "SnapshotMenu.h"
// #include "MemoryClearMenu.h"

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

#include "Context.h"
#include "UserSettings.h"
#include "Storage.h"

#include "adc.h"
#include "system_clock.h"

extern Context context;
extern UserSettings user_settings;
extern Storage storage;

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

extern EffectManager effect_manager;
extern FM fm;
extern PhaseDistortion phase_distortion;
extern RingModulator ring_modulator;
extern Wavefolder wavefolder;
extern Wavewrapper wavewrapper;
extern Bitcrush bitcrush;
extern Drive drive;
extern Bypass bypass;

extern Quantizer quantizer;

extern Adc adc;
extern SystemClock system_clock;

extern int16_t BUF1[2048];
extern int16_t BUF2[2048];
extern int16_t BUF3[2048];
extern int16_t BUF4[2048];
extern int16_t BUF5[2048];

inline double clamp(double input, double low, double high) {
    return std::min(std::max(input, low), high);
}
