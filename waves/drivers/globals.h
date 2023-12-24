#include "stmlib/stmlib.h"
#include <stm32f4xx_conf.h>

#ifndef WAVES_GLOBALS_H_
#define WAVES_GLOBALS_H_


#define USER_WAVE_COUNT         512
#define FACTORY_WAVE_COUNT      512
    
#define USER_SNAPSHOT_COUNT     64
#define FACTORY_SNAPSHOT_COUNT  0
    
#define USER_WAVETABLE_COUNT    16
#define FACTORY_WAVETABLE_COUNT 16
    


// extern GPIO_TypeDef* eeprom_clock_gpio;
// extern uint16_t eeprom_clock_pin;

// extern GPIO_TypeDef* eeprom_miso_gpio;
// extern uint16_t eeprom_miso_pin;

// extern GPIO_TypeDef* eeprom_mosi_gpio;
// extern uint16_t eeprom_mosi_pin;

// extern GPIO_TypeDef* eeprom_ss_gpio;
// extern uint16_t eeprom_ss_pin;


#define EEPROM_FACTORY_SS      	0
#define EEPROM_USER_SS         	1
#define EEPROM_PERSISTENT_SS   	2
#define EEPROM_CLOCK 			3
#define EEPROM_MOSI 			4
#define EEPROM_MISO 			5

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

#define _RXTC_  0 // Rx transfer complete
#define _TXTC_  1 // Tx transfer complete

extern uint32_t _EREG_;
FlagStateTypeDef GetFlag(uint32_t*, uint8_t);
void SetFlag(uint32_t*, uint8_t, FlagStateTypeDef);

extern EEPROM_PIN eeprom[6];

extern uint32_t dataBuffer[1024];

#endif