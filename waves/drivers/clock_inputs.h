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
// Driver for the two clock inputs and their normalization probe.

#ifndef WAVES_DRIVERS_CLOCK_INPUTS_H_
#define WAVES_DRIVERS_CLOCK_INPUTS_H_

#include "stmlib/stmlib.h"
#include "waves/io_buffer.h"

namespace waves {

enum ClockInput {
  CLOCK_INPUT_T,
  CLOCK_INPUT_X,
  CLOCK_INPUT_LAST
};

class ClockInputs {
 public:
  ClockInputs() { }
  ~ClockInputs() { }
  
  void Init();
  void Read(const IOBuffer::Slice& slice, size_t size);
  void ReadNormalization(IOBuffer::Block* block);
  
  bool is_normalized(ClockInput input) {
    return normalized_[input];
  }
  
  bool value(ClockInput input) {
    return previous_flags_[input] & stmlib::GATE_FLAG_HIGH;
  }

 private:
  static const int kProbeSequenceDuration = 64;

  stmlib::GateFlags previous_flags_[CLOCK_INPUT_LAST];

  uint32_t normalization_probe_state_;
  bool normalized_[CLOCK_INPUT_LAST];
  int normalization_mismatch_count_[CLOCK_INPUT_LAST];
  int normalization_decision_count_;
  
  DISALLOW_COPY_AND_ASSIGN(ClockInputs);
};

}  // namespace waves

#endif  // WAVES_DRIVERS_CLOCK_INPUTS_H_
