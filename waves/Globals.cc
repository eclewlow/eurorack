/*
  ==============================================================================

    Globals.cpp
    Created: 7 Nov 2023 5:32:04pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Globals.h"

using namespace waves;

Context context;
// UserSettings user_settings;
SNAPSHOT settings_;
// Storage storage;

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

// Adc adc;
// SystemClock system_clock;
Flash flash;

int16_t BUF1[2048];
int16_t BUF2[2048];
int16_t BUF3[2048];
int16_t BUF4[2048];
int16_t BUF5[2048];

EEPROM_PIN eeprom[11] = {
  { GPIOA, GPIO_Pin_4  },
  { GPIOA, GPIO_Pin_3  },
  { GPIOA, GPIO_Pin_2  },
  { GPIOA, GPIO_Pin_5  },
  { GPIOA, GPIO_Pin_7  },
  { GPIOA, GPIO_Pin_6  },
  { GPIOA, GPIO_Pin_15 },
  { GPIOB, GPIO_Pin_4  },
  { GPIOC, GPIO_Pin_10 },
  { GPIOC, GPIO_Pin_12 },
  { GPIOC, GPIO_Pin_11 },
};

int16_t dataBuffer[2048];
int16_t double_waveframe_buffer_1[4096];
int16_t double_waveframe_buffer_2[4096];
int16_t double_waveframe_buffer_3[4096];
int16_t double_waveframe_buffer_4[4096];
int16_t pump_buffer[4096];

int16_t * front_buffer = double_waveframe_buffer_1;
int16_t * back_buffer = double_waveframe_buffer_2;

int16_t * front_buffer_1 = double_waveframe_buffer_1;
int16_t * front_buffer_2 = double_waveframe_buffer_2;
int16_t * back_buffer_1 = double_waveframe_buffer_3;
int16_t * back_buffer_2 = double_waveframe_buffer_4;


float loading = 0;

uint8_t lcd_buffer[8][128];

uint32_t _EREG_ = 0;

FlagStateTypeDef GetFlag(uint32_t* __registry, uint8_t __flag)
{
  return (*__registry & (1 << __flag)) ? FLAG_SET : FLAG_CLEAR;
}

void SetFlag(uint32_t* __registry, uint8_t __flag, FlagStateTypeDef __state)
{
  if (__state == FLAG_SET)
  {
    *__registry |= (1 << __flag);
  }
  else if (__state == FLAG_CLEAR)
  {
    *__registry &= ~(1 << __flag);
  }
  else
  {
    // Error_Handler("Flag Error!");
  }
}

