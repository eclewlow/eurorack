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
// Driver for quantization.

#include "quantizer2/drivers/quantizer.h"
#include "quantizer2/resources.h"
#include <algorithm>
#include <string.h>
#include <math.h>

using namespace std;

namespace quantizer2 {

#define ONE_OCTAVE_12BIT (12.0f * 4095.0f / 120.0f)

const uint8_t kChordTableLength = 9;

// The scale structure of the Middle Eastern Chord is always 1-b2-3-4-5-b6-7-1
template <typename T> T CLAMP(T value, T low, T high)
{
    return (value < low) ? low : ((value > high) ? high : value);
}


void Quantizer::Init() {
    scale_ = 0;
}


// Returns element closest to target in arr[]
float Quantizer::findClosest(const float arr[], int n, float target)
{
    // Corner cases
    //left-side case
    if (target <= arr[0])
        return arr[0];
    //right-side case
    if (target >= arr[n - 1])
        return arr[n - 1];

    // Doing binary search
    int i = 0, j = n, mid = 0;
    while (i < j) {
        mid = (i + j) / 2;

        if (arr[mid] == target)
            return arr[mid];

        /* If target is less than array element,
            then search in left */
        if (target < arr[mid]) {

            // If target is greater than previous
            // to mid, return closest of two
            if (mid > 0 && target > arr[mid - 1])
                return getClosest(arr[mid - 1], arr[mid], target);
            j = mid;
        }
        /* Repeat for left half */

        // If target is greater than mid
        else {
            if (mid < n - 1 && target < arr[mid + 1])
                return getClosest(arr[mid], arr[mid + 1], target);
            // update i
            i = mid + 1;
        }
    }
    // Only single element left after search
    return arr[mid];
}

// Method to compare which one is the more close.
// We find the closest by taking the difference
// between the target and both values. It assumes
// that val2 is greater than val1 and target lies
// between these two.
float Quantizer::getClosest(float val1, float val2,
                            float target)
{
    if (target - val1 >= val2 - target)
        return val2;
    else
        return val1;
}

uint16_t Quantizer::Quantize(uint16_t input, uint16_t slew, uint16_t chaos) {

    uint16_t output = 0;

    float factor = lut_pow2[chaos];
    input = factor * static_cast<float>(input);
    input = CLAMP<uint16_t>(input, 0, 4095);

    // if current input is close enough to the last input, then we use the last input
    // if (abs(static_cast<int>(input) - static_cast<int>(last_input_)) < 24) {
    //     input = last_input_;
    // }

    // last_input_ = input;


    const float* arr = lookup_table_table[scale_ + 2];

    const uint8_t octave_range = 7;

    float closest = findClosest(arr, 4 * octave_range, static_cast<float>(input));

    while (closest < 0.0f) {
        closest += ONE_OCTAVE_12BIT;
    }
    while (closest > 4095.0f) {
        closest -= ONE_OCTAVE_12BIT;
    }

    // closest = 0.0;

    uint16_t q_val = static_cast<uint16_t>(closest);

    output = q_val;

    // if current input is within the last Q vals hysteresis range, then we use the last Q val
    // if (abs(static_cast<int>(input) - static_cast<int>(last_q_val_)) <= 1) {
    //     output = last_q_val_;
    // }

    // last_q_val_ = output;


    if (output != target_) {

        // slew.
        target_ = output;
        increment_ = (target_ - last_output_) / (1.0 + (1000.0 * slew) / 4095.0);
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

    if (last_output_ == target_) {
        increment_ = 0;
    }

    return static_cast<uint16_t>(last_output_);
}

}  // namespace quantizer2
