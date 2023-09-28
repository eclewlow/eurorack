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
// Driver for quantization.

#ifndef QUANTIZER2_DRIVERS_QUANTIZER_H_
#define QUANTIZER2_DRIVERS_QUANTIZER_H_

#include <stm32f10x_conf.h>
#include "stmlib/stmlib.h"

namespace quantizer2 {

extern const uint16_t chord_table[];
extern const uint8_t kChordTableLength;

class Quantizer {
public:
  Quantizer() { }
  ~Quantizer() { }

  void Init();

  uint16_t Quantize(uint16_t input, uint16_t param_1, uint16_t param_2);
  float getClosest(float, float, float);
  float findClosest(const float arr[], int n, float target);

  inline void set_scale(uint16_t scale) {
    scale_ = scale;
  }

private:
  uint16_t scale_;
  uint16_t target_;
  uint16_t last_input_;
  float increment_;
  float last_output_;


  DISALLOW_COPY_AND_ASSIGN(Quantizer);
};

}  // namespace quantizer2

#endif  // QUANTIZER2_DRIVERS_QUANTIZER_H_
