// Copyright 2013 Emilie Gillet.
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
// Driver for ADC.

#ifndef QUANTIZER_DRIVERS_INTERNAL_ADC_H_
#define QUANTIZER_DRIVERS_INTERNAL_ADC_H_

#include "stmlib/stmlib.h"

namespace quantizer {

const uint8_t kNumAdcChannels = 4;
const uint8_t kHistoryLength = 8;

class InternalAdc {
 public:
  InternalAdc() { }
  ~InternalAdc() { }
  

  void Init();
  
  uint16_t value(uint8_t channel) {
    history[channel][history_index[channel]] = values_[channel];
    history_index[channel] = (history_index[channel] + 1) % kHistoryLength;
    uint16_t sum = 0;
    for (uint8_t i = 0; i < kHistoryLength; i++) {
      sum += history[channel][i];
    }
    uint16_t average = sum / kHistoryLength;
    return average;
    // return values_[channel];
  }

 private:
  uint16_t values_[kNumAdcChannels];
  uint16_t history[kNumAdcChannels][kHistoryLength];
  uint8_t history_index[kNumAdcChannels];
  
  DISALLOW_COPY_AND_ASSIGN(InternalAdc);
};

}  // namespace quantizer

#endif  // QUANTIZER_DRIVERS_INTERNAL_ADC_H_
