/*
 ==============================================================================
 
 4x_downsampler.h
 Created: 12 Nov 2023 9:26:11pm
 Author:  Eugene Clewlow
 
 ==============================================================================
 */

#ifndef DOWNSAMPLER_H
#define DOWNSAMPLER_H

const size_t kOversampling = 4;
const float lut_4x_downsampler_fir[] = {
   2.442415000e-02,  9.297315000e-02,  1.671293800e-01,  2.154733200e-01,
};

class Downsampler {
public:
    Downsampler(float* state) {
        head_ = *state;
        tail_ = 0.0f;
        state_ = state;
    }
    ~Downsampler() {
        *state_ = head_;
    }
    inline void Accumulate(int i, float sample) {
        head_ += sample * lut_4x_downsampler_fir[3 - (i & 3)];
        tail_ += sample * lut_4x_downsampler_fir[i & 3];
    }
    
    inline float Read() {
        float value = head_;
        head_ = tail_;
        tail_ = 0.0f;
        return value;
    }
private:
    float head_;
    float tail_;
    float* state_;

  DISALLOW_COPY_AND_ASSIGN(Downsampler);
};

#endif