// Copyright 2012 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Driver for DAC.

#include "test/drivers/dac.h"

#include <string.h>

namespace test {
  
void Dac::Init() {

  GPIOC->BSRRH = GPIO_Pin_7;
  GPIOB->BSRRH = GPIO_Pin_6;


  // Initialize CS pin.
  GPIO_InitTypeDef gpio_init;
  gpio_init.GPIO_Pin = GPIO_Pin_6;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  gpio_init.GPIO_Mode = GPIO_Mode_OUT;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOB, &gpio_init);

  // Initialize LDAC pin.
  gpio_init.GPIO_Pin = GPIO_Pin_7;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  gpio_init.GPIO_Mode = GPIO_Mode_OUT;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOC, &gpio_init);
  
  // Initialize MOSI and SCK pins.
  gpio_init.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_Mode = GPIO_Mode_AF;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &gpio_init);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
  // GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_SPI1);

  
  // Initialize SPI
  SPI_InitTypeDef spi_init;
  spi_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spi_init.SPI_Mode = SPI_Mode_Master;
  spi_init.SPI_DataSize = SPI_DataSize_16b;
  spi_init.SPI_CPOL = SPI_CPOL_Low;
  spi_init.SPI_CPHA = SPI_CPHA_1Edge;
  spi_init.SPI_NSS = SPI_NSS_Soft;
  spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  spi_init.SPI_FirstBit = SPI_FirstBit_MSB;
  spi_init.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &spi_init);
  SPI_Cmd(SPI1, ENABLE);
}

}  // namespace braids
