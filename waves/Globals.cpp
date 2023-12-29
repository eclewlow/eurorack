/*
  ==============================================================================

    Globals.cpp
    Created: 7 Nov 2023 5:32:04pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "Globals.h"

Context context;
UserSettings user_settings;
Storage storage;

// MainMenu mainMenu;
// ModeMenu modeMenu;
// FxMenu fxMenu;
// Oscilloscope oscilloscope;
// ControlStatusMenu controlStatusMenu;
// DeviceConfigurationMenu deviceConfigurationMenu;
// InfoMenu infoMenu;
// DisplayConfigurationMenu displayConfigurationMenu;
// OscilloscopeConfigurationMenu oscilloscopeConfigurationMenu;
// SubOscillatorMenu subOscillatorMenu;
// EnterNameMenu enterNameMenu;
// WavetableModeMenu wavetableModeMenu;
// ABModeMenu abModeMenu;
// WaveEditor waveEditor;
// LoadWaveMenu loadWaveMenu;
// SaveWaveMenu saveWaveMenu;
// Popup popup;
// MatrixMode matrixMode;
// DrumMode drumMode;
// IOConfigurationMenu ioConfigurationMenu;
// CalibrationMenu calibrationMenu;
// QuantizerMenu quantizerMenu;
// ManageMenu manageMenu;
// SnapshotMenu snapshotMenu;
// MemoryClearMenu memoryClearMenu;

ABEngine abEngine;
WavetableEngine wavetableEngine;
MatrixEngine matrixEngine;
DrumEngine drumEngine;
Suboscillator suboscillator;

EffectManager effect_manager;
FM fm;
PhaseDistortion phase_distortion;
RingModulator ring_modulator;
Wavefolder wavefolder;
Wavewrapper wavewrapper;
Bitcrush bitcrush;
Drive drive;
Bypass bypass;

Quantizer quantizer;

Adc adc;
SystemClock system_clock;

int16_t BUF1[2048];
int16_t BUF2[2048];
int16_t BUF3[2048];
int16_t BUF4[2048];
int16_t BUF5[2048];

