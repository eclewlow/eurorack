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

#include "quantizer/drivers/quantizer.h"
#include "quantizer/resources.h"
#include <algorithm>
#include <string.h>
#include <math.h>

namespace quantizer {

static const uint16_t kCMajorChord = 0b000010010001;
static const uint16_t kCSharpMajorChord = 0b000100100010;
static const uint16_t kBFlatMinorChord = 0b010000100010;
static const uint16_t kBDiminishedChord = 0b100100100100;
static const uint16_t kCMinorChord = 0b000010001001;
static const uint16_t kC2Chord = 0b000010000101;
static const uint16_t kBFlatMajorChord = 0b010000100100;
static const uint16_t kFMajor2Chord = 0b001010100001;
static const uint16_t kFMajorChord = 0b001000100001;

static const uint16_t kGMajorChord = 0b100010000100;
static const uint16_t kCMiddleEastChord = 0b000010010011; //??

const uint8_t kChordTableLength = 9;

const uint16_t chord_table[] = {
  kCMajorChord,
  kCSharpMajorChord,
  kBFlatMinorChord,
  kBDiminishedChord,
  kCMinorChord,
  kC2Chord,
  kBFlatMajorChord,
  kFMajor2Chord,
  kCMiddleEastChord,
};

// The scale structure of the Middle Eastern Chord is always 1-b2-3-4-5-b6-7-1
template <typename T> T CLAMP(T value, T low, T high)
{
    return (value < low) ? low : ((value > high) ? high : value);
}


void Quantizer::Init() {
  scale_ = kCMiddleEastChord;
  // scale_ = kCMajorChord;
}

uint16_t Quantizer::Quantize(uint16_t input, uint16_t transpose, uint16_t slew, uint16_t chaos) {

  uint16_t output = 0;

  int8_t midi_val_normalized = 0;

  // chaos = 3000; // range
  // slew = 0;
  // transpose = 2047; // pitch shift

  // param_1 widen / narrows input
  // ccw 0 ... 1 cw 1...10 ?
  // 0.5 to 10

  chaos = 4095 - chaos;

  float factor = lut_pow2[chaos];

  input = static_cast<uint16_t>(2047 + (input - 2047) / (factor - 0.8));
  input = CLAMP<uint16_t>(input, 0, 4095);


  uint32_t d1 = input * 120; // input is 0...4095 +/- 5 volts
  uint32_t d2 = 4095;

  int32_t midi_val = (d1 + (d2>>1)) / d2;

  // transpose, range from -6 to 6
  int8_t pitch_offset = 13 * transpose / 4096 - 6;
  bool out_of_bounds = false;

  midi_val -= pitch_offset;

  if(midi_val > 120 || midi_val < 0) {
    out_of_bounds = true;
    midi_val += pitch_offset;
  }

  midi_val = CLAMP<int32_t>(midi_val, 0, 120);

  uint8_t current_octave = midi_val / 12;

  midi_val_normalized = midi_val % 12;

  uint8_t min_diff = 12;
  uint8_t quantized_val = 0;
  int8_t octave_shift = 0;

  for (int8_t i=0; i < 12; i++) {
    uint8_t is_set = (scale_ >> i) & 0x1;

    if(!is_set)
      continue;

    uint8_t outer_octave_diff = 12 - abs(midi_val_normalized - i);
    uint8_t inner_octave_diff = abs(midi_val_normalized - i);
    uint8_t diff;

    if (outer_octave_diff < inner_octave_diff) {
      // if we receive c8 as the input, but we are quantizing to a b7, will this work?
      if(midi_val_normalized > i && current_octave >= 9) { // upper octave diff but no upper octave
        diff = inner_octave_diff;
      } else if (midi_val_normalized < i && current_octave == 0) {
        diff = inner_octave_diff;
      } else {
        diff = std::min(inner_octave_diff, outer_octave_diff);
      }
    } else {
      diff = std::min(inner_octave_diff, outer_octave_diff);
    }

    if(diff < min_diff) {
      min_diff = diff;
      quantized_val = i;

      // note: maybe we should choose an outer octave diff randomly, rather than default to the inner octave    
      if (diff == outer_octave_diff && diff != inner_octave_diff) {
        octave_shift = midi_val_normalized > i ? 12 : -12;
      }
    }
  }

  int32_t q_val = quantized_val + current_octave * 12 + octave_shift;

  if(!out_of_bounds) {
    q_val += pitch_offset;
  }
  // q_val = CLAMP<int32_t>(q_val, 0, 120);

  d1 = q_val * 4095;
  d2 = 120;

  output = (d1 + (d2>>1)) / d2;

  if(output != target_) {

    // slew.
    target_ = output;
    increment_ = (target_ - last_output_) / (1.0 + (1000.0 * slew) / 4095.0);
    // increment_ = 0;
  }

  last_output_ += increment_;

  if (increment_ > 0 && last_output_ > target_) {
    increment_ = 0;
    last_output_ = target_;
  }
  else if (increment_ < 0 && last_output_ < target_) {
    increment_ = 0;
    last_output_ = target_;    
  }

  if(last_output_ == target_) {
    increment_ = 0;
  }

  return static_cast<uint16_t>(last_output_);
}

}  // namespace braids
