/*
  ==============================================================================

    fft.h
    Created: 26 Nov 2023 12:10:29am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

class FFT {
public:
    FFT() {}
    ~FFT() {}
    typedef struct {
        float real;
        float imag;
    } COMPLEX_NUMBER;
    
    static COMPLEX_NUMBER complexAdd(COMPLEX_NUMBER a, COMPLEX_NUMBER b);
    static COMPLEX_NUMBER complexSubtract(COMPLEX_NUMBER a, COMPLEX_NUMBER b);
    static COMPLEX_NUMBER complexMultiply(COMPLEX_NUMBER a, COMPLEX_NUMBER b);
    static float complexMagnitude(COMPLEX_NUMBER c);
    static COMPLEX_NUMBER exponent(float k, float N);
    static void fft(float * input, int size, COMPLEX_NUMBER * output);
    static void fft(COMPLEX_NUMBER * input, int size, COMPLEX_NUMBER * output);
    static void ifft(COMPLEX_NUMBER * input, int size, COMPLEX_NUMBER * output);
private:
    DISALLOW_COPY_AND_ASSIGN (FFT);
};
