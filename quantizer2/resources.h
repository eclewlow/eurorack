// Copyright 2023 Eugene Clewlow.
//
// Author: Eugene Clewlow (eugene.clewlowgmail.com)
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
// Resources definitions.
//
// Automatically generated with:
// make resources


#ifndef QUANTIZER2_RESOURCES_H_
#define QUANTIZER2_RESOURCES_H_


#include "stmlib/stmlib.h"



namespace quantizer2 {

typedef uint8_t ResourceId;

extern const float* const lookup_table_table[];

extern const float lut_pow2[];
extern const float lut_midi_to_12bit[];
extern const float lut_c_major_12bit[];
extern const float lut_c_sharp_major_12bit[];
extern const float lut_b_flat_minor_12bit[];
extern const float lut_b_dim_12bit[];
extern const float lut_c_minor_12bit[];
extern const float lut_c_2_12bit[];
extern const float lut_b_flat_major_12bit[];
extern const float lut_f_2_12bit[];
extern const float lut_c_me_12bit[];
extern const float lut_test[];
#define LUT_POW2 0
#define LUT_POW2_SIZE 4096
#define LUT_MIDI_TO_12BIT 1
#define LUT_MIDI_TO_12BIT_SIZE 121
#define LUT_C_MAJOR_12BIT 2
#define LUT_C_MAJOR_12BIT_SIZE 28
#define LUT_C_SHARP_MAJOR_12BIT 3
#define LUT_C_SHARP_MAJOR_12BIT_SIZE 28
#define LUT_B_FLAT_MINOR_12BIT 4
#define LUT_B_FLAT_MINOR_12BIT_SIZE 28
#define LUT_B_DIM_12BIT 5
#define LUT_B_DIM_12BIT_SIZE 28
#define LUT_C_MINOR_12BIT 6
#define LUT_C_MINOR_12BIT_SIZE 28
#define LUT_C_2_12BIT 7
#define LUT_C_2_12BIT_SIZE 28
#define LUT_B_FLAT_MAJOR_12BIT 8
#define LUT_B_FLAT_MAJOR_12BIT_SIZE 28
#define LUT_F_2_12BIT 9
#define LUT_F_2_12BIT_SIZE 28
#define LUT_C_ME_12BIT 10
#define LUT_C_ME_12BIT_SIZE 28
#define LUT_TEST 11
#define LUT_TEST_SIZE 12

}  // namespace quantizer2

#endif  // QUANTIZER2_RESOURCES_H_
