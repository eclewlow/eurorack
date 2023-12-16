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
    


extern GPIO_TypeDef* eeprom_clock_gpio;
extern uint16_t eeprom_clock_pin;

extern GPIO_TypeDef* eeprom_miso_gpio;
extern uint16_t eeprom_miso_pin;

extern GPIO_TypeDef* eeprom_mosi_gpio;
extern uint16_t eeprom_mosi_pin;

extern GPIO_TypeDef* eeprom_ss_gpio;
extern uint16_t eeprom_ss_pin;



#endif