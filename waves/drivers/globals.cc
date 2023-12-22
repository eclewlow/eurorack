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