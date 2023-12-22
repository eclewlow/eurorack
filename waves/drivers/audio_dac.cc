// Copyright 2016 Emilie Gillet.
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
// Drivers for the PCM5100 codec.

#include "waves/drivers/audio_dac.h"

#include <stm32f4xx_conf.h>

namespace waves {

/* static */
AudioDac* AudioDac::instance_;

void AudioDac::Init(int sample_rate, size_t block_size) {
  instance_ = this;
  block_size_ = block_size;
  callback_ = NULL;
  InitializeGPIO();
  InitializeAudioInterface(sample_rate);
  InitializeDMA(block_size);
}

void AudioDac::InitializeGPIO() {
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef gpio_init;
  gpio_init.GPIO_Mode = GPIO_Mode_AF;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;

  gpio_init.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15;
  GPIO_Init(GPIOB, &gpio_init);

  gpio_init.GPIO_Pin = GPIO_Pin_13;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &gpio_init);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

  // gpio_init.GPIO_Pin = GPIO_Pin_1;
  // GPIO_Init(GPIOC, &gpio_init);

  // gpio_init.GPIO_Pin = GPIO_Pin_9;
  // GPIO_Init(GPIOB, &gpio_init);

  // gpio_init.GPIO_Pin = GPIO_Pin_10;
  // GPIO_Init(GPIOB, &gpio_init);


  // GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_SPI2);
  // GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_SPI2);
  // GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);
}

void AudioDac::InitializeAudioInterface(int sample_rate) {
  // RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);

  // RCC_PLLI2SCmd(DISABLE);

  // // Best results for multiples of 32kHz.
  // if (sample_rate == 48000) {
  //   // 47.992kHz
  //   RCC_PLLI2SConfig(258, 3);
  // } else if (sample_rate == 44100) {
  //   // 44.11kHz
  //   RCC_PLLI2SConfig(271, 6);
  // } else if (sample_rate == 32000) {
  //   // 32.003kHz
  //   RCC_PLLI2SConfig(426, 4);
  // } else if (sample_rate == 96000) {
  //   // 95.95 kHz
  //   RCC_PLLI2SConfig(393, 4);
  // } else {
  //   // Unsupported sample rate!
  //   // return false;
  // }



  // RCC_PLLI2SConfig(16, 344, 2, 7, 2);
  RCC_PLLI2SConfig(8, 344, 2, 7, 2);  // this seems to be accurate

  RCC_SAIPLLI2SClkDivConfig(2);

  RCC_SAICLKConfig(RCC_SAIInstance_SAI1, RCC_SAICLKSource_PLLI2S);




  RCC_PLLI2SCmd(ENABLE);
  while (RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY) == RESET);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  I2S_InitTypeDef i2s_init;

  SPI_I2S_DeInit(SPI2);

  i2s_init.I2S_Mode = I2S_Mode_MasterTx;
  i2s_init.I2S_Standard = I2S_Standard_Phillips;
  i2s_init.I2S_DataFormat = I2S_DataFormat_16b;
  i2s_init.I2S_MCLKOutput = I2S_MCLKOutput_Disable;  
  i2s_init.I2S_AudioFreq = I2S_AudioFreq_48k;
  i2s_init.I2S_CPOL = I2S_CPOL_Low;
  I2S_Init(SPI2, &i2s_init);
  I2S_Cmd(SPI2, ENABLE);
}

void AudioDac::InitializeDMA(size_t block_size) {
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
  
  DMA_Cmd(DMA1_Stream4, DISABLE);
  DMA_DeInit(DMA1_Stream4);

  DMA_InitTypeDef dma_init;
  dma_init.DMA_Channel = DMA_Channel_0;
  dma_init.DMA_PeripheralBaseAddr = (uint32_t)&(SPI2->DR);
  dma_init.DMA_Memory0BaseAddr = (uint32_t)(&tx_dma_buffer_[0]);
  dma_init.DMA_DIR = DMA_DIR_MemoryToPeripheral;  // ?? DMA_DIR_PeripheralDST;
  dma_init.DMA_BufferSize = 2 * block_size * 2;  // 2 channels, 2 half blocks.
  dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  dma_init.DMA_Mode = DMA_Mode_Circular;
  dma_init.DMA_Priority = DMA_Priority_High;
  dma_init.DMA_FIFOMode = DMA_FIFOMode_Disable;
  dma_init.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  dma_init.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  dma_init.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream4, &dma_init);
  
  // Enable the interrupts: half transfer and transfer complete.
  DMA_ITConfig(DMA1_Stream4, DMA_IT_TC | DMA_IT_HT, ENABLE);
  
  // Enable the IRQ.
  NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  
  SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
}

void AudioDac::Start(FillBufferCallback callback) {
  callback_ = callback;
  DMA_Cmd(DMA1_Stream4, ENABLE);
}

void AudioDac::Stop() {
  DMA_Cmd(DMA1_Stream4, DISABLE);
}

void AudioDac::Fill(size_t offset) {
  (*callback_)(&tx_dma_buffer_[offset * block_size_], block_size_);
}

}  // namespace waves

extern "C" {

void DMA1_Stream4_IRQHandler(void) {
  uint32_t flags = DMA1->HISR;
  DMA1->HIFCR = DMA_FLAG_TCIF4 | DMA_FLAG_HTIF4;
  if (flags & DMA_FLAG_TCIF4) {
    waves::AudioDac::GetInstance()->Fill(1);
  } else if (flags & DMA_FLAG_HTIF4) {
    waves::AudioDac::GetInstance()->Fill(0);
  }
}
  
}
