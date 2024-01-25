/*
  ==============================================================================

    matrix_engine.cpp
    Created: 19 Nov 2023 3:53:03am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/dsp/Engine.h"
#include "waves/wavetables.h"
// #include "waves/dsp/ParameterInterpolator.h"
#include "waves/Globals.h"
#include "waves/dsp/downsampler/4x_downsampler.h"
#include "waves/dsp/dsp.h"

MatrixEngine::MatrixEngine() {
    phase_ = 0;
    sub_phase_ = 0;

    current_frame_x = 0;
    target_frame_x = 0;
    current_frame_y = 0;
    target_frame_y = 0;
}

MatrixEngine::~MatrixEngine() {
    
}

void MatrixEngine::Init() {
    phase_ = 0;
    sub_phase_ = 0;

    current_frame_x = 0;
    target_frame_x = 0;
    current_frame_y = 0;
    target_frame_y = 0;
}

float MatrixEngine::GetSample(int16_t wavetable, int16_t frame, float phase) {
    float sample = 0;
    float next_sample = 0;
    float interpolated16 = 0;
    float interpolatedFloat = 0;

    float index = phase * 2048.0;
    uint16_t integral = floor(index);
    float fractional = index - integral;
    
    uint16_t nextIntegral = (integral + 1) % 2048;
    
    int16_t * buffer;

    buffer = (wavetable == 0 ? front_buffer_1 : front_buffer_2);

    sample = buffer[frame * 2048 + integral];
    next_sample = buffer[frame * 2048 + nextIntegral];
    
    interpolated16 = sample + (next_sample - sample) * fractional;
    interpolatedFloat = interpolated16 / 32768.0f;

    return interpolatedFloat;

}


void MatrixEngine::on_triple_load_1_finished() {
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    flash.StartFrameDMARead((uint32_t*)matrix_back_buffer_2, 12288, (matrixEngine.target_frame_y + 1) * 65536 + (matrixEngine.target_frame_x) * 4096, MatrixEngine::on_triple_load_2_finished);
}

void MatrixEngine::on_triple_load_2_finished() {
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    flash.StartFrameDMARead((uint32_t*)matrix_back_buffer_3, 12288, (matrixEngine.target_frame_y + 2) * 65536 + (matrixEngine.target_frame_x) * 4096, MatrixEngine::on_triple_load_3_finished);
}

void MatrixEngine::on_triple_load_3_finished() {
    matrixEngine.buffered_frame_x = matrixEngine.target_frame_x;
    matrixEngine.buffered_frame_y = matrixEngine.target_frame_y;

    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}


void MatrixEngine::on_load_1_finished() {

    // x morphs between frames;
    // y morphs between wavetables  morph
    
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    flash.StartFrameDMARead((uint32_t*)back_buffer_2, 8192, (matrixEngine.target_frame_y + 1) * 65536 + (matrixEngine.target_frame_x) * 4096, MatrixEngine::on_load_2_finished);
}

void MatrixEngine::on_load_2_finished() {
    // int16_t * temp_buffer = front_buffer_1;
    // front_buffer_1 = back_buffer_1;
    // back_buffer_1 = temp_buffer;

    // temp_buffer = front_buffer_2;
    // front_buffer_2 = back_buffer_2;
    // back_buffer_2 = temp_buffer;

    // matrixEngine.current_frame_x = matrixEngine.target_frame_x;
    // matrixEngine.current_frame_y = matrixEngine.target_frame_y;
    matrixEngine.buffered_frame_x = matrixEngine.target_frame_x;
    matrixEngine.buffered_frame_y = matrixEngine.target_frame_y;

    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}

/**
 * 
 * step 1: calculate integral x,y
 * step 2: is current?
 *      (HOW do we know if it is current?)
 *                      if the integral is 1,1 and the current is (0, 0) -> (2, 2),
 *                          x: 0 <= 1 < 0+2, and y: 0 <= 1 < 0+2, so it is buffered.
 *                      )
 *      
 *      a: NO
 *          1: is back buffered?
 *                      (how do we know if it is back buffered? 
 *                      the buffer is a 3x3 matrix.
 *                      we store the top-left corner index as the current buffer?
 *                      we can then calculate if the calculated integral falls inside the buffered zone
 *                      if the integral is 1,1 and the buffer is (0, 0) -> (2, 2),
 *                          x: 0 <= 1 < 0+2, and y: 0 <= 1 < 0+2, so it is buffered.
 *                      )
 *              a: NO
 *                  1: load buffer.
 *                      (which 3x3 index to we want to buffer?
 *                      well we want to determine which quadrant the target/integral x,y lies in.
 *                      if the integral is 1,1 and bottom-right quadrant, then we want to buffer (1,1)-> (3,3)
 *                      if the integral is 1, 1 and the top-right quandrant, then we want to buffer (1, 0) -> (3, 2)
 *                      1, 1 and bottom-left, then buffer (0, 1) -> (2, 3)
 *                      1, 1 and top-left, then buffer (0, 0)
 *                      // so top-left  is -1,-1
 *                      // top-right    is +0, -1
 *                      // bottom-right is +0,+0
 *                      // bottom-left  is -1,+0
 * 
 *                      // exception. x = 0 and left, y = 0 and upper quadrants
 *                      // exception. x >= 14 and right, y >= 14 and lower quadrants
 *              b: YES
 *                  1. swap buffer with front
 *                      (how do we swap the buffer?
 *                      swap all front buffers with back buffers
 *                      // so we have a current front 3x3 frame, and current back 3x3 frame.
 *                  2. sample front
 *                      first we need to know which buffer to use.
 *                      how do we know which buffer to use?
 *                      if the integral is 1, 1 and the buffer is (0, 0) -> (2,2) then the buffer to use is 1 - 0 = 1, 1 - 0 = 1
 *                      if the integral is 1, 1 and the buffer is (1, 1) -> (2,2) then the buffer to use is 1 - 1 = 0, 1 - 1 = 0
 *                      if the integral is 1, 1 and the buffer is (0, 1) -> (3, 4) then the buffer to use is 1 - 0 = 1, 1 - 1 = 0
 *                      // so the buffer to use is integral-x minus buf-x, integral-y - buf-y.
 *                      // IF integral-y minus buf-y = 0 ,
 *                                      THEN front_buffer_1 = triple_waveframe_buffer_1[2048 * (integral-x minus buf-x)]
 *                                      THEN front_buffer_2 = triple_waveframe_buffer_2[2048 * (integral-x minus buf-x)]
 *                      // IF integral-y minus buf-y = 1 , THEN use front_buffer = triple_waveframe_buffer_2[2048 * (integral-x minus buf-x)]
 *                                      THEN front_buffer_1 = triple_waveframe_buffer_2[2048 * (integral-x minus buf-x)]
 *                                      THEN front_buffer_2 = triple_waveframe_buffer_3[2048 * (integral-x minus buf-x)]

 *                      (how do we sample the front?
 *                      the front buffer is a double frame buffer pointer that points to an index in a triple frame buffer.
 *                      // so we use the getsample function which samples between frames from a double frame buffer.
 *                      // but we need to first offset the integral.
 *                      // so if the integral is 2, 2 and the current is 1, 1, we want to sample offset (1,1)-> (2, 2) within the buffer
 *                      // so we sample the correctly adjusted frame index -> (2 - 1, 2 - 1), 
 *                                  2-1 = 1:
 *                                      THEN front_buffer_1 = triple_waveframe_buffer_2[2048 * (2-1)]
 *                                      THEN front_buffer_2 = triple_waveframe_buffer_3[2048 * (2-1)]
 *      b: YES
 *          1. sample front.
 *          // which buffer do we sample from?
 *          // since the calculated integral is current, we sample from the current front buffer.
 *          // just using the getsample function
 * 
 * step 3: caculate fractional quadrant
 * step 4: does the quadrant align with the current buffer?
 *                      (how do we know if the quadrant aligns with the current buffer?)
 *                      if the front buffer index is 0, 0, and the calculated integral is 1,1, we would expect the quadrant
 *                      to be the top-left.
 *                      BUT if the calculated quadrant (derived from fractional) is anywhere else, the back buffer should be loaded
 *      a: NO
 *          1. load buffer based on integral/fractional
 *              integral is 1, 1. current is for example 0, 0. quadrant is 0.5, 0.5. so target buffer 1, 1.
 *              target-frame 1, 1. if buffered frame is not 1, 1, load 1, 1. when dma finishes set buffered_frame to target frame.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 **/

float MatrixEngine::GetSampleBetweenFrames(float phase, float morph_x, float morph_y) {
    // if x1 = 8 and x2 = 12. and morph_x = 0.5, then x1 + morph_x * (x2 - x1)

    // y sits between two wavetable numbers
    // x sists between two wavetable frames
    float frame_x = GetX1() + morph_x * ( GetX2() -  GetX1());
    uint16_t frame_x_integral = floor(frame_x);
    float frame_x_fractional = frame_x - frame_x_integral;

    // uint16_t next_frame_x_integral = (frame_x_integral + 1) % 16;

    float frame_y =  GetY1() + morph_y * ( GetY2() -  GetY1());
    uint16_t frame_y_integral = floor(frame_y);
    float frame_y_fractional = frame_y - frame_y_integral;

    // uint16_t next_frame_y_integral = (frame_y_integral + 1) % 16;

    if(!GetFlag(&_EREG_, _BUSY_) && !GetFlag(&_EREG_, _RXNE_)) {

        if(!is_in_buffer(frame_x_integral, frame_y_integral, current_frame_x, current_frame_y)) {

            // target_frame_ = frame_integral;

            if(is_in_buffer(frame_x_integral, frame_y_integral, buffered_frame_x, buffered_frame_y)) {
                swap_buffers(&matrix_front_buffer_1, &matrix_back_buffer_1);
                swap_buffers(&matrix_front_buffer_2, &matrix_back_buffer_2);
                swap_buffers(&matrix_front_buffer_3, &matrix_back_buffer_3);

                swap_int(&current_frame_x, &buffered_frame_x);
                swap_int(&current_frame_y, &buffered_frame_y);
            }
        }
        /* check fractional */
        // >= 0.5 is closer to 1. < 0.5 is closer to 0
        // check first which is closer to 0/1, x or y fractional.
        // basically trigger dma if the back buffered frame doesn't match the calculated 
        int8_t test_frame_x = round(frame_x) - 1;
        int8_t test_frame_y = round(frame_y) - 1;

        test_frame_x = CLAMP<int8_t>(test_frame_x, GetX1(), GetX2() - 2);
        test_frame_y = CLAMP<int8_t>(test_frame_y, GetY1(), GetY2() - 2);

        if((test_frame_x != buffered_frame_x || test_frame_y != buffered_frame_y) \
            && (test_frame_x != current_frame_x || test_frame_y != current_frame_y)) {
            target_frame_x = test_frame_x;
            // loading = test_frame_y;
            target_frame_y = test_frame_y;
            flash.StartFrameDMARead((uint32_t*)matrix_back_buffer_1, 12288, (target_frame_y) * 65536 + (target_frame_x) * 4096, MatrixEngine::on_triple_load_1_finished);
        }
    }

    if(frame_y_integral <= current_frame_y) {
        if(frame_x_integral <= current_frame_x) {
            front_buffer_1 = &matrix_front_buffer_1[0];
            front_buffer_2 = &matrix_front_buffer_2[0];
        } else {
            front_buffer_1 = &matrix_front_buffer_1[2048];
            front_buffer_2 = &matrix_front_buffer_2[2048];
        }
    } else {
        if(frame_x_integral <= current_frame_x) {
            front_buffer_1 = &matrix_front_buffer_2[0];
            front_buffer_2 = &matrix_front_buffer_3[0];
        } else {
            front_buffer_1 = &matrix_front_buffer_2[2048];
            front_buffer_2 = &matrix_front_buffer_3[2048];
        }
    }

    // TODO: get sample across 4 frames
    float frame_x1y1_sample = GetSample(0, 0, phase);
    float frame_x2y1_sample = GetSample(0, 1, phase);

    float frame_x1y2_sample = GetSample(1, 0, phase);
    float frame_x2y2_sample = GetSample(1, 1, phase);

    float upper_sample;
    float lower_sample;
    float sample;
    
    // if(frame_x_integral > current_frame_x)
        // upper_sample = frame_x2y1_sample;
    // else if(frame_x_integral < current_frame_x)
        // upper_sample = frame_x1y1_sample;
    // else
        upper_sample = frame_x1y1_sample * (1 - frame_x_fractional) + frame_x2y1_sample * frame_x_fractional;

    // if(frame_x_integral > current_frame_x)
    //     lower_sample = frame_x2y2_sample;
    // else if(frame_x_integral < current_frame_x)
    //     lower_sample = frame_x1y2_sample;
    // else
        lower_sample = frame_x1y2_sample * (1 - frame_x_fractional) + frame_x2y2_sample * frame_x_fractional;

    // if(frame_y_integral > current_frame_y)
    //     sample = lower_sample;
    // else if(frame_y_integral < current_frame_y)
    //     sample = upper_sample;
    // else
        sample = upper_sample * (1.0f - frame_y_fractional) + lower_sample * frame_y_fractional;

    return sample;
}

void MatrixEngine::FillWaveform(int16_t * waveform, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph, bool withFx) {
    float frequency = 23.4375;

    float phaseIncrement = frequency / 48000.0f;
    
    float temp_phase = 0.0f;
    
    // if(withFx)
    //     effect_manager.getEffect()->Sync_phases();

    for(int i = 0; i < 2048; i++) {
        float morph_y = morph_;
        float morph_x = fx_;
        morph_x = CLAMP<float>(morph_x, 0.0, 1.0);
        morph_y = CLAMP<float>(morph_y, 0.0, 1.0);

        float sample = GetSampleBetweenFrames(temp_phase, morph_x, morph_y);
        
        temp_phase += phaseIncrement;
        
        if(temp_phase >= 1.0f)
            temp_phase -= 1.0f;
        
        waveform[i] = static_cast<int16_t>(sample * 32767.0f);
    }
}

float MatrixEngine::GetSampleNoFX(float phase, float fx, float morph) {
    float sample = GetSampleBetweenFrames(phase, fx, morph);
    return sample;
}

void MatrixEngine::triggerUpdate() {
    phase_ = 0.0f;

    current_frame_x = 1;
    current_frame_y = 1;
    target_frame_y = 0;
    target_frame_x = 0;
    flash.StopDMA(true);
    flash.StartFrameDMARead((uint32_t*)matrix_back_buffer_1, 12288, (target_frame_y) * 65536 + (target_frame_x) * 4096, MatrixEngine::on_triple_load_1_finished);
    // flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, (target_frame_y) * 65536 + (target_frame_x) * 4096, MatrixEngine::on_load_1_finished);

    // current_frame_ = 0;
    // target_frame_ = 0;
    // // swap_counter_ = 0.0f;
    // // swap_ = false;
    // // target_frame_ = 0;
    // flash.StartFrameDMARead((uint32_t*)back_buffer_1, 8192, target_frame_ * 4096, WavetableEngine::on_load_finished);
}

void MatrixEngine::Render(AudioDac::Frame* output, size_t size, uint16_t tune, uint16_t fx_amount, uint16_t fx, uint16_t morph)
{
    float morphTarget = morph / 65535.0;
    float fxTarget = fx / 65535.0;

    float tuneTarget = static_cast<float>(tune);
    
    if(!started_) {
        started_ = true;
        morph_ = morphTarget;
        tune_ = tuneTarget;
        fx_ = fxTarget;
    }

    ParameterInterpolator morph_interpolator(&morph_, morphTarget, size);
    ParameterInterpolator fx_interpolator(&fx_, fxTarget, size);
    // ParameterInterpolator tune_interpolator(&tune_, tuneTarget, size);
    Downsampler carrier_downsampler(&carrier_fir_);

    // float note = (120.0f * tune_interpolator.Next()) / 4095.0;
    float note = tuneTarget * calibration_x_ + calibration_y_;
    note = CLAMP<float>(note, 0.0f, 120.0f);

    note = quantizer.Quantize(note);

    // note = note - 24.0f;

    ParameterInterpolator phase_increment_interpolator(&phase_increment_, NoteToFrequency(note), size);
    ParameterInterpolator sub_phase_increment_interpolator(&sub_phase_increment_, NoteToFrequency((note + subosc_detune_ / 100.0f + subosc_offset_)), size);
    
    // float phase = 0;
    float sample = 0;
    float sub_sample = 0;
    // bool isOscilloscope = false;

    while (size--) {
        
        float interpolated_morph = morph_interpolator.Next();
        interpolated_morph = CLAMP<float>(interpolated_morph, 0.0, 0.9999);

        float interpolated_fx = fx_interpolator.Next();
        interpolated_fx = CLAMP<float>(interpolated_fx, 0.0, 0.9999);

        float phase_increment = phase_increment_interpolator.Next();
        phase_increment = CLAMP<float>(phase_increment, 0.0f, 1.0f);
        float sub_phase_increment = sub_phase_increment_interpolator.Next();
        sub_phase_increment = CLAMP<float>(sub_phase_increment, 0.0f, 1.0f);


        // for (size_t j = 0; j < kOversampling; ++j) {
            // float sample = GetSampleBetweenFrames(phase_, interpolated_fx, interpolated_morph);
            sample = GetSampleBetweenFrames(phase_, interpolated_fx, interpolated_morph);

            if(subosc_wave_ == SUBOSC_WAVE_SINE) {
                sub_sample = GetSine(sub_phase_);
            }
            else if(subosc_wave_ == SUBOSC_WAVE_TRIANGLE) {
                sub_sample = GetTriangle(sub_phase_);
            }
            else if(subosc_wave_ == SUBOSC_WAVE_SAWTOOTH) {
                sub_sample = GetSawtooth(sub_phase_, sub_phase_increment);
            }
            else if(subosc_wave_ == SUBOSC_WAVE_RAMP) {
                sub_sample = GetRamp(sub_phase_, sub_phase_increment);
            }
            else if(subosc_wave_ == SUBOSC_WAVE_SQUARE) {
                sub_sample = GetSquare(sub_phase_, sub_phase_increment);
            }
            else if(subosc_wave_ == SUBOSC_WAVE_COPY) {
                sub_sample = GetSampleBetweenFrames(sub_phase_, interpolated_fx, interpolated_morph);
            }

            sub_sample = subosc_mix_ * sample + (1.0f - subosc_mix_) * sub_sample;

            phase_ += phase_increment;
            sub_phase_ += sub_phase_increment;
            
            if(phase_ >= 1.0f)
                phase_ -= 1.0f;

            if(sub_phase_ >= 1.0f)
                sub_phase_ -= 1.0f;
            
            // carrier_downsampler.Accumulate(j, sample);
        // }
        
        // float sample = carrier_downsampler.Read();
        
        output->l = sample * 26000.0f;
        output->r = sub_sample * 26000.0f;
        ++output;
    }
}

void MatrixEngine::SetX1(int8_t x1) { x1_ = x1; }
void MatrixEngine::SetY1(int8_t y1) { y1_ = y1; }
void MatrixEngine::SetX2(int8_t x2) { x2_ = x2; }
void MatrixEngine::SetY2(int8_t y2) { y2_ = y2; }
void MatrixEngine::IncrementX1(int8_t dx) {
    x1_ = CLAMP<int8_t>(x1_ + dx, 0, x2_);
}
void MatrixEngine::IncrementY1(int8_t dy) {
    y1_ = CLAMP<int8_t>(y1_ + dy, 0, y2_);
}
void MatrixEngine::IncrementX2(int8_t dx) {
    x2_ = CLAMP<int8_t>(x2_ + dx, x1_, 15);
}
void MatrixEngine::IncrementY2(int8_t dy) {
    y2_ = CLAMP<int8_t>(y2_ + dy, y1_, 15);
}
int8_t MatrixEngine::GetX1() { return x1_; }
int8_t MatrixEngine::GetY1() { return y1_; }
int8_t MatrixEngine::GetX2() { return x2_; }
int8_t MatrixEngine::GetY2() { return y2_; }

void MatrixEngine::SetWavelistOffset(int8_t offset) { wavelist_offset_ = offset; }
int8_t MatrixEngine::GetWavelistOffset() { return wavelist_offset_; }
