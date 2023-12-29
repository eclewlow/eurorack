#include "waves/drivers/globals.h"

// GPIO_TypeDef* eeprom_clock_gpio = GPIOA;
// uint16_t eeprom_clock_pin = GPIO_Pin_5;

// GPIO_TypeDef* eeprom_miso_gpio = GPIOB;
// uint16_t eeprom_miso_pin = GPIO_Pin_4;

// GPIO_TypeDef* eeprom_mosi_gpio = GPIOB;
// uint16_t eeprom_mosi_pin = GPIO_Pin_5;

// GPIO_TypeDef* eeprom_ss_gpio = GPIOA;
// uint16_t eeprom_ss_pin = GPIO_Pin_4;

EEPROM_PIN eeprom[11] = {
	{ GPIOA, GPIO_Pin_4 },
	{ GPIOA, GPIO_Pin_3 },
	{ GPIOA, GPIO_Pin_2 },
	{ GPIOA, GPIO_Pin_5 },
	{ GPIOA, GPIO_Pin_7 },
	{ GPIOA, GPIO_Pin_6 },
	{ GPIOA, GPIO_Pin_15 },
   	{ GPIOB, GPIO_Pin_4 },
	{ GPIOC, GPIO_Pin_10 },
   	{ GPIOC, GPIO_Pin_12 },
   	{ GPIOC, GPIO_Pin_11 },
};

int16_t dataBuffer[2048];
int16_t double_waveframe_buffer_1[4096];
int16_t double_waveframe_buffer_2[4096];
int16_t pump_buffer[4096];
int16_t * front_buffer = double_waveframe_buffer_1;
int16_t * back_buffer = double_waveframe_buffer_2;
uint8_t loading = 0;

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