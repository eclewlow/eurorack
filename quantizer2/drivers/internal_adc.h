// Copyright 2023 Eugene Clewlow.
//
// Author: Eugene Clewlow (eugene.clewlow@gmail.com)
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

#ifndef QUANTIZER2_DRIVERS_INTERNAL_ADC_H_
#define QUANTIZER2_DRIVERS_INTERNAL_ADC_H_

#include "stmlib/stmlib.h"
#include <algorithm>

namespace quantizer2 {

const uint8_t kNumAdcChannels = 3;
const uint8_t kHistoryLength = 32;

class InternalAdc {
public:
  InternalAdc() { }
  ~InternalAdc() { }


  void Init();

  uint16_t value(uint8_t channel) {

    // apply hysteresis
    if (abs(static_cast<int>(values_[channel]) - static_cast<int>(pre_hysteresis[channel])) < 120) {
      history[channel][history_index[channel]] = pre_hysteresis[channel];
    } else {
      history[channel][history_index[channel]] = values_[channel];
      pre_hysteresis[channel] = values_[channel];
    }

    // history[channel][history_index[channel]] = values_[channel];

    history_index[channel] = (history_index[channel] + 1) % kHistoryLength;
    uint32_t sum = 0;
    for (uint8_t i = 0; i < kHistoryLength; i++) {
      sum += history[channel][i];
    }
    uint16_t average = sum / kHistoryLength;

    // apply hysteresis
    if (abs(static_cast<int>(average) - static_cast<int>(post_hysteresis[channel])) < 120) {
      average = post_hysteresis[channel];
    } else {
      post_hysteresis[channel] = average;
    }

    return average;
  }

private:
  uint16_t values_[kNumAdcChannels];
  uint16_t history[kNumAdcChannels][kHistoryLength];
  uint8_t history_index[kNumAdcChannels];
  uint16_t last_values[kNumAdcChannels];
  uint16_t pre_hysteresis[kNumAdcChannels];
  uint16_t post_hysteresis[kNumAdcChannels];

  DISALLOW_COPY_AND_ASSIGN(InternalAdc);
};

}  // namespace quantizer2

#endif  // QUANTIZER2_DRIVERS_INTERNAL_ADC_H_
