// Copyright 2017 Emilie Gillet.
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
// Lightweight DAC driver used for the firmware update procedure.
// Initializes the I2S port as SPI, and relies on a timer for clock generation.

// #include "waves/drivers/flash.h"
#include "waves/Globals.h"

namespace waves {

/* static */
Flash* Flash::instance_;

void Flash::Init() {
  instance_ = this;


  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  // RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  
  DMA_Cmd(DMA2_Stream0, DISABLE);
  DMA_DeInit(DMA2_Stream0);

  DMA_Cmd(DMA2_Stream3, DISABLE);
  DMA_DeInit(DMA2_Stream3);

  // Initialize 3 ROM SS pin.
  GPIO_InitTypeDef gpio_init;
  gpio_init.GPIO_Mode = GPIO_Mode_OUT;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  gpio_init.GPIO_Pin = eeprom[EEPROM_FACTORY_SS].pin;
  GPIO_Init(eeprom[EEPROM_FACTORY_SS].gpio, &gpio_init);

  // miso
  gpio_init.GPIO_Mode = GPIO_Mode_AF;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  gpio_init.GPIO_Pin = eeprom[EEPROM_MISO].pin;
  GPIO_Init(eeprom[EEPROM_MISO].gpio, &gpio_init);

  // mosi
  gpio_init.GPIO_Mode = GPIO_Mode_AF;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  gpio_init.GPIO_Pin = eeprom[EEPROM_MOSI].pin;
  GPIO_Init(eeprom[EEPROM_MOSI].gpio, &gpio_init);

  // clock
  gpio_init.GPIO_Mode = GPIO_Mode_AF;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  gpio_init.GPIO_Pin = eeprom[EEPROM_CLOCK].pin;
  GPIO_Init(eeprom[EEPROM_CLOCK].gpio, &gpio_init);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

  HIGH(EEPROM_FACTORY_SS);
  // LOW(EEPROM_CLOCK);

  // Initialize SPI.
  SPI_InitTypeDef spi_init;
  spi_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spi_init.SPI_Mode = SPI_Mode_Master;
  spi_init.SPI_DataSize = SPI_DataSize_8b;
  spi_init.SPI_CPOL = SPI_CPOL_Low;
  spi_init.SPI_CPHA = SPI_CPHA_1Edge;
  spi_init.SPI_NSS = SPI_NSS_Soft;
  spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  spi_init.SPI_FirstBit = SPI_FirstBit_MSB;
  spi_init.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &spi_init);

  SPI_Cmd(SPI1, ENABLE);


  DMA_InitTypeDef dma_init;

  // SPI RX
  dma_init.DMA_Channel = DMA_Channel_3;
  dma_init.DMA_PeripheralBaseAddr = (uint32_t)&(SPI1->DR);
  dma_init.DMA_Memory0BaseAddr = (uint32_t)(dataBuffer);
  dma_init.DMA_DIR = DMA_DIR_PeripheralToMemory;
  dma_init.DMA_BufferSize = 0;
  dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  dma_init.DMA_Mode = DMA_Mode_Normal;
  dma_init.DMA_Priority = DMA_Priority_High;
  dma_init.DMA_FIFOMode = DMA_FIFOMode_Disable;
  dma_init.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  dma_init.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  dma_init.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &dma_init);
  
  // Enable the interrupts: half transfer and transfer complete.
  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
  
  // Enable the IRQ.
  NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  
  // SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);

  // SPI TX
  dma_init.DMA_Channel = DMA_Channel_3;
  dma_init.DMA_PeripheralBaseAddr = (uint32_t)&(SPI1->DR);
  dma_init.DMA_Memory0BaseAddr = (uint32_t)(dataBuffer);
  dma_init.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  dma_init.DMA_BufferSize = 0;
  dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  dma_init.DMA_Mode = DMA_Mode_Normal;
  dma_init.DMA_Priority = DMA_Priority_High;
  dma_init.DMA_FIFOMode = DMA_FIFOMode_Disable;
  dma_init.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  dma_init.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  dma_init.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream3, &dma_init);

  // DMA_ITConfig(DMA2_Stream3, DMA_IT_TC, ENABLE);

  // Enable the IRQ.
  // NVIC_EnableIRQ(DMA2_Stream3_IRQn);

  // SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);

}

void Flash::StartDMARead(uint16_t __bytes) {
  uint8_t pump = 0;

  DMA_SetCurrDataCounter(DMA2_Stream3, __bytes);
  DMA_SetCurrDataCounter(DMA2_Stream0, __bytes);
  DMA2_Stream3->M0AR = (int)&pump;
  DMA2_Stream0->M0AR = (int)dataBuffer;
  DMA_Cmd(DMA2_Stream3, ENABLE);
  DMA_Cmd(DMA2_Stream0, ENABLE);
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);

  SetFlag(&_EREG_, _RXTC_, FLAG_CLEAR);
  SetFlag(&_EREG_, _TXTC_, FLAG_CLEAR);
  SetFlag(&_EREG_, _BUSY_, FLAG_SET);
}

void Flash::StartFrameDMARead(uint32_t * buffer, uint32_t __bytes, uint32_t address, void (* func)()) {
  // if(GetFlag(&_EREG_, _BUSY_))
  StopDMA(true);
  SetFlag(&_EREG_, _RXTC_, FLAG_CLEAR);
  SetFlag(&_EREG_, _TXTC_, FLAG_CLEAR);
  SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
  SetFlag(&_EREG_, _BUSY_, FLAG_SET);

  set_on_dma_read_finished_func(func);

  loading = 3;
  LOW(EEPROM_FACTORY_SS);

  uint8_t send_buf[5];
  send_buf[0] = READ_66MHZ;
  send_buf[1] = ((address >> 16) & 0xFF);
  send_buf[2] = ((address >> 8) & 0xFF);
  send_buf[3] = ((address) & 0xFF);
  send_buf[4] = 0x00;

  Write(send_buf, 5);

  // system_clock.Delay(1);
  // uint8_t pump = 0;

  loading = 4;

  SPI_I2S_SendData(SPI1, 0);
  while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
  SPI_I2S_ReceiveData(SPI1);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)) {
    SPI_I2S_ReceiveData(SPI1);    
  }

  loading = 5;

  DMA_SetCurrDataCounter(DMA2_Stream3, __bytes);
  DMA_SetCurrDataCounter(DMA2_Stream0, __bytes);
  DMA2_Stream3->M0AR = (uint32_t)pump_buffer;
  DMA2_Stream0->M0AR = (uint32_t)buffer;
  DMA_Cmd(DMA2_Stream3, ENABLE);
  DMA_Cmd(DMA2_Stream0, ENABLE);
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);

  loading = 6;

  // loading++;

  // while(!DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0))
  //   DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0);
  // while(!DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3))
  //   DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3);
  // loading++;
}

void Flash::StartDMAWrite(uint16_t __bytes) {
  DMA_SetCurrDataCounter(DMA2_Stream3, __bytes);
  DMA2_Stream3->M0AR = (int)dataBuffer;
  DMA_Cmd(DMA2_Stream3, ENABLE);
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);

  SetFlag(&_EREG_, _RXTC_, FLAG_CLEAR);
  SetFlag(&_EREG_, _TXTC_, FLAG_CLEAR);
  SetFlag(&_EREG_, _BUSY_, FLAG_SET);
}

void Flash::StopDMA(bool bypass) {
  loading = 2;
  DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0);
  DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3);

  // while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
  // while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)) {
    SPI_I2S_ReceiveData(SPI1);    
  }

  DMA_Cmd(DMA2_Stream3, DISABLE);
  DMA_Cmd(DMA2_Stream0, DISABLE);
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, DISABLE);

  HIGH(EEPROM_FACTORY_SS);

  if(!bypass)
    SetFlag(&_EREG_, _RXNE_, FLAG_SET);

  if(on_dma_read_finished_func && !bypass)
    (*on_dma_read_finished_func)();
}

}  // namespace waves


extern "C" {

// Rx Transfer complete
void DMA2_Stream0_IRQHandler(void) {
    loading = 1;
  if(DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0)) {
    DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0);

    SetFlag(&_EREG_, _RXTC_, FLAG_SET);

    // if(GetFlag(&_EREG_, _TXTC_)) {
      waves::Flash::GetInstance()->StopDMA();
    // }
  }
}

// Tx Transfer complete
void DMA2_Stream3_IRQHandler(void) {
  // loading++;
  if(DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3)) {
    DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3);

    SetFlag(&_EREG_, _TXTC_, FLAG_SET);

    // if(GetFlag(&_EREG_, _RXTC_)) {
      // waves::Flash::GetInstance()->StopDMA();
    // }
  }
}

  
}
