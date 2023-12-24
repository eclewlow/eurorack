#include "waves/drivers/globals.h"

// GPIO_TypeDef* eeprom_clock_gpio = GPIOA;
// uint16_t eeprom_clock_pin = GPIO_Pin_5;

// GPIO_TypeDef* eeprom_miso_gpio = GPIOB;
// uint16_t eeprom_miso_pin = GPIO_Pin_4;

// GPIO_TypeDef* eeprom_mosi_gpio = GPIOB;
// uint16_t eeprom_mosi_pin = GPIO_Pin_5;

// GPIO_TypeDef* eeprom_ss_gpio = GPIOA;
// uint16_t eeprom_ss_pin = GPIO_Pin_4;

EEPROM_PIN eeprom[6] = {
	{ GPIOA, GPIO_Pin_4 },
	{ GPIOA, GPIO_Pin_3 },
	{ GPIOA, GPIO_Pin_2 },
	{ GPIOA, GPIO_Pin_5 },
	{ GPIOA, GPIO_Pin_7 },
	{ GPIOA, GPIO_Pin_6 },
};

uint32_t dataBuffer[1024];



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