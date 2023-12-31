
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
