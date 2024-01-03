// Copyright 2015 Emilie Gillet.
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
// Driver for ADC. ADC1 is used for the 8 pots ; ADC2 for the 8 CV inputs.

#ifndef WAVES_DRIVERS_ADC_H_
#define WAVES_DRIVERS_ADC_H_

#include "stmlib/stmlib.h"
#include <algorithm>

namespace waves {

const uint8_t kNumAdcChannels = 8;
const uint8_t kHistoryLength = 64;
const uint16_t kThreshold = 70;

class Adc {
 public:
  Adc() { }
  ~Adc() { }
  
  enum Adc_Channel {
      ADC_CHANNEL_PITCH_POT        = 0,
      ADC_CHANNEL_FX_AMOUNT_POT    = 1,
      ADC_CHANNEL_FX_POT           = 2,
      ADC_CHANNEL_MORPH_POT        = 3,
      ADC_CHANNEL_PITCH_CV         = 4,
      ADC_CHANNEL_FX_AMOUNT_CV     = 5,
      ADC_CHANNEL_FX_CV            = 6,
      ADC_CHANNEL_MORPH_CV         = 7,
  };

  void Init(bool single_channel);
  void DeInit();
  void Convert();
  
  float float_value(int channel) {
    return static_cast<float>(value(channel)) / 65536.0f;
  }

  uint16_t getChannel(int channel) {

    // // apply hysteresis
    // if (abs(static_cast<int>(values_[channel]) - static_cast<int>(pre_hysteresis[channel])) < kThreshold) {
    //   history[channel][history_index[channel]] = pre_hysteresis[channel];
    // } else {
    //   history[channel][history_index[channel]] = values_[channel];
    //   pre_hysteresis[channel] = values_[channel];
    // }

    // // history[channel][history_index[channel]] = values_[channel];

    // history_index[channel] = (history_index[channel] + 1) % kHistoryLength;
    // uint32_t sum = 0;
    // for (uint8_t i = 0; i < kHistoryLength; i++) {
    //   sum += history[channel][i];
    // }
    // uint16_t average = sum / kHistoryLength;

    // // apply hysteresis
    // if (abs(static_cast<int>(average) - static_cast<int>(post_hysteresis[channel])) < kThreshold) {
    //   average = post_hysteresis[channel];
    // } else {
    //   post_hysteresis[channel] = average;
    // }

    // return average;
    return values_[channel];
  }

  uint16_t value(int channel) {

    // apply hysteresis
    if (abs(static_cast<int>(values_[channel]) - static_cast<int>(pre_hysteresis[channel])) < kThreshold) {
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
    if (abs(static_cast<int>(average) - static_cast<int>(post_hysteresis[channel])) < kThreshold) {
      average = post_hysteresis[channel];
    } else {
      post_hysteresis[channel] = average;
    }

    return average;
    // return values_[channel];
  }

  uint16_t getChannelProcessed(int channel) {
    return value(channel);
  }
  // inline const uint16_t* values() const {
    // return &values_[0];
  // }
  
 private:
  uint16_t history[kNumAdcChannels][kHistoryLength];
  uint8_t history_index[kNumAdcChannels];
  uint16_t pre_hysteresis[kNumAdcChannels];
  uint16_t post_hysteresis[kNumAdcChannels];

  uint16_t values_[kNumAdcChannels];
  
  DISALLOW_COPY_AND_ASSIGN(Adc);
};

extern Adc adc;


}  // namespace waves

#endif  // WAVES_DRIVERS_ADC_H_
