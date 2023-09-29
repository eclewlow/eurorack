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
// Driver for ADC scanning.

#ifndef QUANTIZER2_DRIVERS_ADC_H_
#define QUANTIZER2_DRIVERS_ADC_H_

#include <stm32f10x_conf.h>
#include "stmlib/stmlib.h"

namespace quantizer2 {

const size_t kNumChannels = 1;
const uint8_t kADCHistoryLength = 1;

class Adc {
public:
  Adc() { }
  ~Adc() { }

  void Init();

  // Inlined and optimized!
  inline bool PipelinedScan() {
    switch (acquisition_stage_) {
    case 0:
      rx_word_ |= SPI2->DR;
      channels_[active_channel_] = 4095 - rx_word_; // flip -5...5 to 2.5...0 because of inverted opamp
      GPIOB->BSRR = GPIO_Pin_12;
      GPIOB->BRR = GPIO_Pin_12;
      SPI2->DR = 0x04 | 0x02;
      active_channel_ = (active_channel_ + 1) % kNumChannels;
      acquisition_stage_ = 1;
      break;

    case 1:
      SPI2->DR;
      SPI2->DR = active_channel_ << 6;
      acquisition_stage_ = 2;
      break;

    case 2:
      rx_word_ = (SPI2->DR & 0xf) << 8;
      SPI2->DR = 0x00;  // Dummy trailing data.
      acquisition_stage_ = 0;
      break;
    }
    return (active_channel_ == 0 && acquisition_stage_ == 1);
  }

  bool PipelinedRead(uint8_t channel);

  uint16_t Read(uint8_t channel);

  uint16_t channel(uint8_t index) { 
    history[history_index] = channels_[index];
    history_index = (history_index + 1) % kADCHistoryLength;
    uint32_t sum = 0;
    for (uint8_t i = 0; i < kADCHistoryLength; i++) {
      sum += history[i];
    }
    uint16_t average = sum / kADCHistoryLength;
    return average;

    // return channels_[index];
  }

private:
  uint16_t rx_word_;
  size_t active_channel_;
  size_t acquisition_stage_;
  uint16_t channels_[kNumChannels];
  uint16_t history[kADCHistoryLength];
  uint8_t history_index;

  DISALLOW_COPY_AND_ASSIGN(Adc);
};

}  // namespace quantizer2

#endif  // QUANTIZER2_DRIVERS_ADC_H_
