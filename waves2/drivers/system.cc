
// System level initialization.

#include "waves2/drivers/system.h"
#include <stm32f4xx_conf.h>

namespace waves2 {

void System::Init(bool application) {
  SystemInit();

  if (application) {
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000);
  }
  // IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  // IWDG_SetPrescaler(IWDG_Prescaler_32);

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef gpio_init;
  gpio_init.GPIO_Pin = GPIO_Pin_5;
  gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
  gpio_init.GPIO_Mode = GPIO_Mode_OUT;
  gpio_init.GPIO_Speed = GPIO_High_Speed;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &gpio_init);


  gpio_init.GPIO_Pin = GPIO_Pin_13;
  gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
  gpio_init.GPIO_Mode = GPIO_Mode_IN;
  gpio_init.GPIO_Speed = GPIO_High_Speed;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOC, &gpio_init);
}

void System::StartTimers() {
  SysTick_Config(F_CPU / 1000);
}


}  // namespace waves2
