#pragma once

namespace waves {

inline float ThisBlepSample(float t) { return 0.5f * t * t; }
inline float NextBlepSample(float t) { t = 1.0f - t; return -0.5f * t * t; }

inline float GetSine(float phase) {
    float index = phase * kSineLUTSize;
    MAKE_INTEGRAL_FRACTIONAL(index)
    float a = lut_sine[index_integral];
    float b = lut_sine[index_integral + 1];

    return a + (b - a) * index_fractional;
}

// This function calculates the PolyBLEPs
inline float poly_blep(float t, float dt)
{
//    double dt = phase;
    
    // t-t^2/2 +1/2
    // 0 < t <= 1
    // discontinuities between 0 & 1
    if (t < dt)
    {
        t /= dt;
        return t + t - t * t - 1.0;
    }
    
    // t^2/2 +t +1/2
    // -1 <= t <= 0
    // discontinuities between -1 & 0
    else if (t > 1 - dt)
    {
        t = (t - 1.0) / dt;
        return t * t + t + t + 1.0;
    }
    
    // no discontinuities
    // 0 otherwise
    else return 0.0;
}

inline float GetRamp(float phase, float phase_increment) {
    float value = 2.0f * phase - 1.0f;
    
    // if(phase + phase_increment >= 1.0f) {
    //     phase = phase + phase_increment - 1.0f;
    //     float t = phase / phase_increment;
    //     value -= ThisBlepSample(t);
    // } else if(phase < phase_increment) {
    //     float t = phase / phase_increment;
    //     value -= NextBlepSample(t);
    // }
    
    return value;
}

inline float GetSawtooth(float phase, float phase_increment) {
    float value = 2.0f * (1.0-phase) - 1.0f;
    
    // if(phase + phase_increment >= 1.0f) {
    //     phase = phase + phase_increment - 1.0f;
    //     float t = phase / phase_increment;
    //     value += ThisBlepSample(t);
    // } else if(phase < phase_increment) {
    //     float t = phase / phase_increment;
    //     value += NextBlepSample(t);
    // }
    
    return value;
}

inline float GetSquare(float phase, float phase_increment) {
    float value = phase < 0.5 ? 0.0 : 1.0;
    //(2.0f * phase - 1.0f) * 1.0;
    
    // if(phase + phase_increment >= 0.5f && phase < 0.5f) {
    //     phase = phase + phase_increment - 0.5f;
    //     float t = phase / phase_increment;
    //     value += ThisBlepSample(t);
    // } else if(phase >= 0.5f && phase - phase_increment < 0.5f) {
    //     float t = (phase - 0.5f) / phase_increment;
    //     value += NextBlepSample(t);
    // }

    // if(phase + phase_increment >= 1.0f) {
    //     phase = phase + phase_increment - 1.0f;
    //     float t = phase / phase_increment;
    //     value -= ThisBlepSample(t);
    // } else if(phase < phase_increment) {
    //     float t = phase / phase_increment;
    //     value -= NextBlepSample(t);
    // }

    return value;
}

inline float GetTriangle(float phase) {
    // float value = -1.0 + (2.0 * phase);
    // value = 2.0 * (fabs(value) - 0.5);
    // value = 2.0 * ((value > 0 ? value : -value) - 0.5);
    float value;
    if(phase < 0.5)
        value = phase;
    else
        value = 1 - phase;
    return value;
}

inline float GetOscillatorSample(float phase, float phase_increment) {
	switch(SINE_SHAPE) {
		case SINE_SHAPE:
			return GetSine(phase);
			break;
		case TRIANGLE_SHAPE:
			return GetTriangle(phase);
			break;
		case SAWTOOTH_SHAPE:
			return GetSawtooth(phase, phase_increment);
			break;
		case RAMP_SHAPE:
	        return GetRamp(phase, phase_increment);
			break;
		case SQUARE_SHAPE:
	        return GetSquare(phase, phase_increment);
			break;
	}
	return 0.0f;
}

}