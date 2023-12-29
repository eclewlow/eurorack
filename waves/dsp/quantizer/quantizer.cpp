/*
  ==============================================================================

    quantizer.cpp
    Created: 4 Dec 2023 4:50:49pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "Globals.h"
#define ONE_OCTAVE_12BIT (12.0f * 4095.0f / 120.0f)

Quantizer::Quantizer() {
    scale_ = QUANTIZER_SCALE_OFF;
}

Quantizer::~Quantizer() {
    
}

void Quantizer::Init() {
    scale_ = QUANTIZER_SCALE_OFF;
}

void Quantizer::triggerUpdate() {
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


float Quantizer::Quantize(float input) {

    if(scale_ == QUANTIZER_SCALE_OFF)
        return input;
    
    // input is a 0.0f to 120.0f [???]  float - type midi value.
    
    uint16_t tune = adc.getChannel(Adc::ADC_CHANNEL_PITCH_POT);
    float tonic = (120.0f * tune) / 4095.0;
    
    float tonic_mod = fmod(tonic, 12.0f);
    float input_mod = fmod(input, 12.0f);
    
    int16_t tonic_octave = tonic / 12;
    int16_t input_octave = input / 12;

//    const float quantizer_scale_pentatonic[11] = { -12.0,-10.0,-8.0,-5.0,-3.0,0.0,2.0,4.0,7.0,9.0,12.0 };
//    const float quantizer_scale_blues[13] = { -12.0,-10.0,-9.0,-8.0,-5.0,-3.0,0.0,2.0,3.0,4.0,7.0,9.0,12.0 };
//    const float quantizer_scale_diminish[17] = { -12.0,-10.0,-9.0,-7.0,-6.0,-4.0,-3.0,-1.0,0.0,2.0,3.0,5.0,6.0,8.0,9.0,11.0,12.0 };
//    const float quantizer_scale_harmonic_major[15] = { -12.0,-10.0,-8.0,-7.0,-5.0,-4.0,-1.0,0.0,2.0,4.0,5.0,7.0,8.0,11.0,12.0 };
//    const float quantizer_scale_harmonic_minor[15] = { -12.0,-10.0,-9.0,-7.0,-5.0,-4.0,-1.0,0.0,2.0,3.0,5.0,7.0,8.0,11.0,12.0 };
//    const float quantizer_scale_wholetone[13] = { -12.0,-10.0,-8.0,-6.0,-4.0,-2.0,0.0,2.0,4.0,6.0,8.0,10.0,12.0 };
//    const float quantizer_scale_chromatic[25] = { -12.0,-11.0,-10.0,-9.0,-8.0,-7.0,-6.0,-5.0,-4.0,-3.0,-2.0,-1.0,0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0 };
//    const float quantizer_scale_arabian[15] = { -12.0,-11.0,-8.0,-7.0,-5.0,-4.0,-1.0,0.0,1.0,4.0,5.0,7.0,8.0,11.0,12.0 };
//    const float quantizer_scale_diatonic[15] = { -12.0,-10.0,-8.0,-7.0,-5.0,-3.0,-1.0,0.0,2.0,4.0,5.0,7.0,9.0,11.0,12.0 };

    const float* arr;
    int8_t size;
    switch(scale_) {
        case QUANTIZER_SCALE_PENTATONIC:
            arr = quantizer_scale_pentatonic;
            size = sizeof(quantizer_scale_pentatonic) / sizeof(float);
            break;
        case QUANTIZER_SCALE_BLUES:
            arr = quantizer_scale_blues;
            size = sizeof(quantizer_scale_blues) / sizeof(float);
            break;
        case QUANTIZER_SCALE_DIMINISH:
            arr = quantizer_scale_diminish;
            size = sizeof(quantizer_scale_diminish) / sizeof(float);
            break;
        case QUANTIZER_SCALE_HARMONIC_MAJOR:
            arr = quantizer_scale_harmonic_major;
            size = sizeof(quantizer_scale_harmonic_major) / sizeof(float);
            break;
        case QUANTIZER_CALE_HARMONIC_MINOR:
            arr = quantizer_scale_harmonic_minor;
            size = sizeof(quantizer_scale_harmonic_minor) / sizeof(float);
            break;
        case QUANTIZER_SCALE_WHOLETONE:
            arr = quantizer_scale_wholetone;
            size = sizeof(quantizer_scale_wholetone) / sizeof(float);
            break;
        case QUANTIZER_SCALE_CHROMATIC:
            arr = quantizer_scale_chromatic;
            size = sizeof(quantizer_scale_chromatic) / sizeof(float);
            break;
        case QUANTIZER_SCALE_ARABIAN:
            arr = quantizer_scale_arabian;
            size = sizeof(quantizer_scale_arabian) / sizeof(float);
            break;
        case QUANTIZER_SCALE_DIATONIC:
            arr = quantizer_scale_diatonic;
            size = sizeof(quantizer_scale_diatonic) / sizeof(float);
            break;
        default:
            return input;
    }

    float closest = findClosest(arr, size, input_mod - tonic_mod);
    
    closest = tonic + (input_octave - tonic_octave) * 12.0f + closest;

    while (closest < 0.0f) {
        closest += 12.0f;
    }
    while (closest > 120.0f) {
        closest -= 12.0f;
    }

    return closest;
}
//
//float Quantizer::Quantize(float input) {
//
//    if(scale_ == QUANTIZER_SCALE_OFF)
//        return input;
//    
//    // input is a 0.0f to 120.0f [???]  float - type midi value.
//    
//    uint16_t tune = adc.getChannel(Adc::ADC_CHANNEL_PITCH_POT);
//    float tonic = (120.0f * tune) / 4095.0;
//    
//    float offset = input - tonic;
//
//    const float* arr = lut_test;
//
//    float closest = findClosest(arr, 120, offset);
//    
//    closest = tonic + closest;
//
//    while (closest < 0.0f) {
//        closest += 12.0f;
//    }
//    while (closest > 120.0f) {
//        closest -= 12.0f;
//    }
//
//    return closest;
//}
