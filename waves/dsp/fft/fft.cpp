/*
  ==============================================================================

    fft.cpp
    Created: 26 Nov 2023 12:10:29am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/fft/fft.h"

//-------------------------------------------------
// Add two complex numbers
//-------------------------------------------------
FFT::COMPLEX_NUMBER FFT::complexAdd(COMPLEX_NUMBER a, COMPLEX_NUMBER b)
{
//    return [a[0] + b[0], a[1] + b[1]];
    return { a.real + b.real, a.imag + b.imag };
};

//-------------------------------------------------
// Subtract two complex numbers
//-------------------------------------------------
FFT::COMPLEX_NUMBER FFT::complexSubtract(COMPLEX_NUMBER a, COMPLEX_NUMBER b)
{
//    return [a[0] - b[0], a[1] - b[1]];
    return { a.real - b.real, a.imag - b.imag };
};

//-------------------------------------------------
// Multiply two complex numbers
//
// (a + bi) * (c + di) = (ac - bd) + (ad + bc)i
//-------------------------------------------------
FFT::COMPLEX_NUMBER FFT::complexMultiply(COMPLEX_NUMBER a, COMPLEX_NUMBER b)
{
//    return [(a[0] * b[0] - a[1] * b[1]),
//            (a[0] * b[1] + a[1] * b[0])];
    return {
        a.real * b.real - a.imag * b.imag,
        a.real * b.imag + a.imag * b.real
    };
};

//-------------------------------------------------
// Calculate |a + bi|
//
// sqrt(a*a + b*b)
//-------------------------------------------------
float FFT::complexMagnitude(COMPLEX_NUMBER c)
{
//    return Math.sqrt(c[0]*c[0] + c[1]*c[1]);
    return sqrt(c.real * c.real + c.imag * c.imag);
};

FFT::COMPLEX_NUMBER FFT::exponent(float k, float N) {
    float x = -2 * M_PI * (k / N);
    
    return { cos(x), sin(x) };
    //     mapExponent[N] = mapExponent[N] || {};
    //     mapExponent[N][k] = mapExponent[N][k] || [Math.cos(x), Math.sin(x)];// [Real, Imaginary]
    
    //     return mapExponent[N][k];
}


//void FFT::fftInPlace(float * vector) {
//    var N = vector.length;
//
//    var trailingZeros = twiddle.countTrailingZeros(N); //Once reversed, this will be leading zeros
//
//    // Reverse bits
//    for (var k = 0; k < N; k++) {
//      var p = twiddle.reverse(k) >>> (twiddle.INT_BITS - trailingZeros);
//      if (p > k) {
//        var complexTemp = [vector[k], 0];
//        vector[k] = vector[p];
//        vector[p] = complexTemp;
//      } else {
//        vector[p] = [vector[p], 0];
//      }
//    }
//
//    //Do the DIT now in-place
//    for (var len = 2; len <= N; len += len) {
//      for (var i = 0; i < len / 2; i++) {
//        var w = fftUtil.exponent(i, len);
//        for (var j = 0; j < N / len; j++) {
//          var t = complex.multiply(w, vector[j * len + i + len / 2]);
//          vector[j * len + i + len / 2] = complex.subtract(vector[j * len + i], t);
//          vector[j * len + i] = complex.add(vector[j * len + i], t);
//        }
//      }
//    }
//  }

//
void FFT::fft(float * input, int size, COMPLEX_NUMBER * output) {
//   var X = [],
   int N = size;

   // Base case is X = x + 0i since our input is assumed to be real only.
   if (N == 1) {
//     if (Array.isArray(vector[0])) //If input vector contains complex numbers
//       return [[vector[0][0], vector[0][1]]];
//     else
//       return [[vector[0], 0]];
       output[0].real = input[0];
       output[0].imag = 0;
       return;
   }
    
    float evens_input[N/2];
    float odds_input[N/2];
    
    for(int i = 0; i < N; i++) {
        if(i % 2 == 0)
            evens_input[i / 2] = input[i];
        else
            odds_input[i / 2] = input[i];
    }
    
    COMPLEX_NUMBER evens_output[N/2];
    COMPLEX_NUMBER odds_output[N/2];
    
    fft(evens_input, N / 2, evens_output);
    fft(odds_input, N / 2, odds_output);
    
   // Now, perform N/2 operations!
   for (int k = 0; k < N / 2; k++) {
     // t is a complex number!
       COMPLEX_NUMBER t = evens_output[k];
       COMPLEX_NUMBER e = complexMultiply(exponent(k, N), odds_output[k]);

       output[k] = complexAdd(t, e);
       output[k + (N / 2)] = complexSubtract(t, e);
   }
 }

void FFT::fft(COMPLEX_NUMBER * input, int size, COMPLEX_NUMBER * output) {
//   var X = [],
   int N = size;

   // Base case is X = x + 0i since our input is assumed to be real only.
   if (N == 1) {
       output[0].real = input[0].real;
       output[0].imag = input[0].imag;
       return;
   }
    
    COMPLEX_NUMBER evens_input[N/2];
    COMPLEX_NUMBER odds_input[N/2];
    
    for(int i = 0; i < N; i++) {
        if(i % 2 == 0)
            evens_input[i / 2] = input[i];
        else
            odds_input[i / 2] = input[i];
    }
    
    COMPLEX_NUMBER evens_output[N/2];
    COMPLEX_NUMBER odds_output[N/2];
    
    fft(evens_input, N / 2, evens_output);
    fft(odds_input, N / 2, odds_output);
    
   // Now, perform N/2 operations!
   for (int k = 0; k < N / 2; k++) {
     // t is a complex number!
       COMPLEX_NUMBER t = evens_output[k];
       COMPLEX_NUMBER e = complexMultiply(exponent(k, N), odds_output[k]);

       output[k] = complexAdd(t, e);
       output[k + (N / 2)] = complexSubtract(t, e);
   }
 }

void FFT::ifft(COMPLEX_NUMBER * input, int size, COMPLEX_NUMBER * output) {
    //Interchange real and imaginary parts
    COMPLEX_NUMBER csignal[size];
    for(int i = 0; i < size; i++) {
        //            csignal[i]= {signal[i][1], signal[i][0]};
        csignal[i]= { input[i].imag, input[i].real };
    }
    
    //Apply fft
    COMPLEX_NUMBER ps[size];
    fft(csignal, size, ps);

    //Interchange real and imaginary parts and normalize
    //        var res=[];
    for(int j=0;  j < size; j++){
        output[j] = { ps[j].imag / size, ps[j].real / size };
    }
    //        return res;
}
