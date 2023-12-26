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

#include <stm32f4xx_conf.h>
// #include <cstdio>
// #include <cstring>
// #include <stdio.h>
// #include <cstdlib>
#include "math.h"
// #include "waves/drivers/clock_inputs.h"
// #include "waves/drivers/dac.h"
// #include "waves/drivers/debug_pin.h"
// #include "waves/drivers/debug_port.h"
// #include "waves/drivers/gate_outputs.h"
// #include "waves/drivers/rng.h"
#include "waves/drivers/system.h"
#include "waves/drivers/adc.h"
#include "stmlib/system/system_clock.h"
#include "waves/drivers/audio_dac.h"
#include "waves/drivers/flash.h"
#include "waves/drivers/4x_downsampler.h"
#include "waves/drivers/ParameterInterpolator.h"
// #include "waves/drivers/uart_logger.h"
// #include "waves/ramp/ramp_extractor.h"
// #include "waves/random/random_generator.h"
// #include "waves/random/random_stream.h"
// #include "waves/random/t_generator.h"
// #include "waves/random/x_y_generator.h"

// #include "waves/clock_self_patching_detector.h"
// #include "waves/cv_reader.h"
// #include "waves/io_buffer.h"
// #include "waves/note_filter.h"
// #include "waves/resources.h"
// #include "waves/scale_recorder.h"
// #include "waves/settings.h"
// #include "waves/ui.h"

// #include "stmlib/dsp/dsp.h"
// #include "stmlib/dsp/hysteresis_quantizer.h"
// #include "stmlib/dsp/units.h"

#define PROFILE_INTERRUPT 0
#define PROFILE_RENDER 0

using namespace waves;
using namespace std;
using namespace stmlib;

const bool test_adc_noise = false;

const int kSampleRate = 48000;
const int kGateDelay = 2;

const size_t kMaxBlockSize = 24;
const size_t kBlockSize = 12;

AudioDac audio_dac;

Adc adc;

// ClockInputs clock_inputs;
// ClockSelfPatchingDetector self_patching_detector[kNumGateOutputs];
// CvReader cv_reader;
// Dac dac;
// DebugPort debug_port;
// GateOutputs gate_outputs;
// HysteresisQuantizer2 deja_vu_length_quantizer;
// IOBuffer io_buffer;
// NoteFilter note_filter;
// Rng rng;
// ScaleRecorder scale_recorder;
// Settings settings;
// Ui ui;
// UartLogger logger;
Flash flash;

// RandomGenerator random_generator;
// RandomStream random_stream;
// TGenerator t_generator;
// XYGenerator xy_generator;

// Default interrupt handlers.


/* Debug Exception and Monitor Control Register base address */
// #define DEMCR                 *((volatile uint32_t*) 0xE000EDFCu)

/* ITM register addresses */
// #define ITM_STIMULUS_PORT0    *((volatile uint32_t*) 0xE0000000u)
// #define ITM_TRACE_EN          *((volatile uint32_t*) 0xE0000E00u)


/* Override low-level _write system call */
int _write(int file, char *ptr, int len) {
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++) {
        ITM_SendChar(*ptr++);
    }
    return len;
}

uint8_t current_frame_ = 0;
uint8_t target_frame_ = 0;

extern "C" {

int __errno;

void NMI_Handler() { }
void HardFault_Handler() { while (1); }
void MemManage_Handler() { while (1); }
void BusFault_Handler() { while (1); }
void UsageFault_Handler() { while (1); }
void SVC_Handler() { }
void DebugMon_Handler() { }
void PendSV_Handler() { }

void SysTick_Handler() {
  // IWDG_ReloadCounter();
  system_clock.Tick();
  // ui.Poll();
  // if (settings.freshly_baked()) {
  //   if (debug_port.readable()) {
  //     uint8_t command = debug_port.Read();
  //     uint8_t response = ui.HandleFactoryTestingRequest(command);
  //     debug_port.Write(response);
  //   }
  // }
  adc.Convert();

  float morph_target = adc.float_value(0);
  float clamped_morph = CLAMP<float>(morph_target, 0.0, 0.9999);

  if(!GetFlag(&_EREG_, _BUSY_) && !GetFlag(&_EREG_, _RXNE_)) {
    float frame = clamped_morph * 15.0f;

    uint16_t frame_integral = floor(frame);
    if(frame_integral != current_frame_) {
      target_frame_ = frame_integral;
      flash.StartFrameDMARead((uint32_t*)back_buffer, 8192, target_frame_ * 4096);
    }
  }
}

int counter = 0;
float phase = 0.0f;


void TIM1_UP_TIM10_IRQHandler(void) {
  if (!(TIM1->SR & TIM_IT_Update)) {
    return;
  }
  TIM1->SR = (uint16_t)~TIM_IT_Update;
  
  counter++;

  // if(counter > 48000)
    // counter++;
  // if(counter % 2 == 0)
  //   GPIO_SetBits(GPIOA, kPinFactorySS);
  // else {
  //   GPIO_ResetBits(GPIOA, kPinFactorySS);
  // }

  // printf("test");
  // logger.Trace('h');
  // int16_t sample = flash.LoadWaveSample(counter % (2048 * 16 * 1), counter % (2048 * 16), counter % (2048));
  // int16_t sample = flash.LoadWaveSample(0, 0, 0);
  // ITM_SendChar('h');

  char value[80];



    // if(counter == 9) {
    //   flash.SectorErase4K(0, EEPROM_FACTORY_SS);
    //   // system_clock.Delay(25);
    //   flash.Program(0xec, 0, EEPROM_FACTORY_SS);
    //   // system_clock.Delay(25);
    //   flash.Program(0x41, 1, EEPROM_FACTORY_SS);
    //   // system_clock.Delay(25);
    //   int16_t buf = 0;
    //   flash.Read25Mhz((uint8_t*)&buf, 2, 0, EEPROM_FACTORY_SS);
    //   flash.w25qxx.Capacity = buf;
    // }
    // if(counter == 10) {
    //   flash.SectorErase4K(0, EEPROM_FACTORY_SS);
    // }
    // if(counter == 11) {
    //   flash.Program(0xec, 0, EEPROM_FACTORY_SS);
    // }
    // if(counter == 12) {
    //   flash.Program(0x41, 1, EEPROM_FACTORY_SS);
    // }
    // if(counter == 13) {
    //   int16_t buf = 0;
    //   flash.Read25Mhz((uint8_t*)&buf, 2, 0, EEPROM_FACTORY_SS);
    //   flash.w25qxx.Capacity = buf;
    // }

  uint32_t  sample1 = 0;
  // int16_t sample2 = 0;

  uint8_t status = 0;
  // status = flash.ReadStatusRegister(EEPROM_FACTORY_SS);

  sample1 = flash.w25qxx.ID;
  // flash.Read66Mhz((uint8_t *)&sample1, 2, 0, EEPROM_FACTORY_SS);
  // flash.Read25Mhz((uint8_t *)&sample2, 2, 2, EEPROM_FACTORY_SS);

  // sample1 = flash.Jedec_ID_Read();
  // flash.Read66Mhz((uint8_t *)&sample2, 1, 1, EEPROM_FACTORY_SS);

  // flash.Read25Mhz(&status, 1, 0, EEPROM_FACTORY_SS);
  // float test = 20.0f;
  // sample = adc.value(0);
  // int16_t * buffer = (int16_t *)dataBuffer;

  sample1 += 0;
  status += 0;
  // snprintf(value, 80, "c=%d, m=%lu, s=%08lx, %08lx\n", counter, system_clock.milliseconds(), sample1, __REV(dataBuffer[0]));
  // _write(0, (char*)value, 80);
// union fourbyte {
//   uint32_t dword;
//   uint16_t word[2];
//   uint8_t  byte[4];
// };

// union fourbyte val;

// val.dword = __REV(dataBuffer[0]);
  // int16_t * result = (int16_t *)dataBuffer;
  // if(DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0)) {
  //   loading++;
  //   DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0);
  // }
  // if(DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3)) {
  //   loading++;
  //   DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3);
  // }



// if(!GetFlag(&_EREG_, _BUSY_) && !GetFlag(&_EREG_, _RXNE_)) {

  // float morph_target = adc.float_value(0);
  // float clamped_morph = CLAMP<float>(morph_target, 0.0, 0.9999);
  // float frame = clamped_morph * 15.0f;
  // uint16_t frame_integral = floor(frame);

  // memcpy(&result, dataBuffer, 2);
  // snprintf(value, 80, "c=%d, s=%d, frame=%d, %d\n", counter, loading, frame_integral, front_buffer[(2048 - 20) + counter%20]);
  snprintf(value, 80, "l=%d, busy=%d, rxne=%d, %d\n", loading, GetFlag(&_EREG_, _BUSY_), GetFlag(&_EREG_, _RXNE_), front_buffer[(2048 - 20) + counter%20]);
  // snprintf(value, 80, "%08lx\n", dataBuffer[0]);
  // snprintf(value, 80, "buf=%04x, db=%08lx\n, %08lx", buffer[0], dataBuffer[0], flash.w25qxx.ID);
  // snprintf(value, 40, "f=%d\n", static_cast<int>(phase*100.0f));
  // sample += 1;
  // _write(0, (char*)value, 80);


      // GPIO_ResetBits(GPIOA, kPinFactorySS);

  // if(flash.trigger_load && !flash.loading) {
  //   flash.trigger_load = false;
  //   flash.loading = true;
  //   flash.Load_BackBuffer();
  //   flash.Swap_Buffers();
  // }

  // GPIOB->BSRR = GPIO_Pin_1;
  // dac.Write(-audio_samples[playback_block][current_sample] + 32768);

  // bool trigger_detected = gate_input.raised();
  // sync_samples[playback_block][current_sample] = trigger_detected;
  // trigger_detected_flag = trigger_detected_flag | trigger_detected;
  
  // current_sample = current_sample + 1;
  // if (current_sample >= kBlockSize) {
  //   current_sample = 0;
  //   playback_block = (playback_block + 1) % kNumBlocks;
  // }
  
  // bool adc_scan_cycle_complete = adc.PipelinedScan();
  // if (adc_scan_cycle_complete) {
  //   ui.UpdateCv(adc.channel(0), adc.channel(1), adc.channel(2), adc.channel(3));
  //   if (trigger_detected_flag) {
  //     trigger_delay = settings.trig_delay()
  //         ? (1 << settings.trig_delay()) : 0;
  //     ++trigger_delay;
  //     trigger_detected_flag = false;
  //   }
  //   if (trigger_delay) {
  //     --trigger_delay;
  //     if (trigger_delay == 0) {
  //       trigger_flag = true;
  //     }
  //   }
  // }
  // HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, var);
}

}

uint32_t test_ramp;

int16_t GetSample(int16_t wavetable, int16_t frame, float phase) {
    // float index = phase * 2048.0;
    // uint16_t integral = floor(index);
    // float fractional = index - integral;
    
    // uint16_t nextIntegral = (integral + 1) % 2048;
    
    // int16_t sample1 = 0;
    // int16_t sample2 = 0;

    // // flash.Read66Mhz((uint8_t *)&sample1, 2, 4096 * frame + integral * 2, EEPROM_FACTORY_SS);
    // // flash.Read66Mhz((uint8_t *)&sample2, 2, 4096 * frame + nextIntegral * 2, EEPROM_FACTORY_SS);
    // index += 0;
    // integral += 0;
    // nextIntegral += 0;
    // fractional += 0;
    // sample1 += 0;
    // sample2 += 0;
    // float interpolated16 = sample1 + (sample2 - sample1) * fractional;
    
    // float interpolatedFloat = interpolated16 / 32768.0f;
    
    // return interpolatedFloat;
    return 0;
}

float carrier_fir_;

float morph_;

float swap_counter = 0.0f;
float swap_increment = 1.0f / 10000.0f;

void FillBuffer(AudioDac::Frame* output, size_t size) {
// #ifdef PROFILE_INTERRUPT
//   TIC
// #endif  // PROFILE_INTERRUPT

//   IWDG_ReloadCounter();
  
//   ui.Poll();
  float phase_increment = (110.0f / 4.0f) / 47992.0f;
//   if (test_adc_noise) {
//     static float note_lp = 0.0f;
//     float note = modulations.note;
//     ONE_POLE(note_lp, note, 0.0001f);
//     float cents = (note - note_lp) * 100.0f;
//     CONSTRAIN(cents, -8.0f, +8.0f);
//     while (size--) {
//       output->r = output->l = static_cast<int16_t>(cents * 4040.0f);
//       ++output;
//     }
//   } else if (ui.test_mode()) {
//     // 100 Hz ascending and descending ramps.
    Downsampler carrier_downsampler(&carrier_fir_);

    /*
  168 mhz
    48000
    3500 instructions per frame

    */

    float morphTarget = adc.float_value(0);

    ParameterInterpolator morph_interpolator(&morph_, morphTarget, size);

    bool swap = false;

    if(GetFlag(&_EREG_, _RXNE_)) {
      // swap buffers
      // do transition this half
      swap = true;
    }
    // swap = false;

    while (size--) {

      // if(1) {//counter < 20) {
      //   output->l = 0;
      //   output->r = 0;
      //   ++output;
      //   return;
      // }
      // float sample = sinf(2 * M_PI * phase);
      // float test = 0;//sinf(2 * M_PI * phase);

      // sample += 0.0f;
      // int16_t shortsample = static_cast<int16_t>(32767.0f * sample);
      // shortsample = 0xffff;
      // if(phase < 0.5)
        // shortsample = 0x0000;

      // output->l = shortsample;
      // output->r = shortsample;
      // ++output;

      // +/- 2.74V, 2.69, 2.79, 2.84 , 2.54 fluctuates

      // takes 480 to increment once.
      // if rate is 48000, it can output 100hz

      // float morph = 0;//adc.float_value(0);
      // float interpolated16 = flash.LoadWaveSample(0, morph, phase);

      // frame = 0 to 15, morph between two frames

      // float frame = 0;//adc.float_value(0) * 15.0f;
      // uint16_t frame_integral = floor(frame);
      // float frame_fractional = frame - frame_integral;      
      // uint16_t next_frame_integral = (frame_integral + 1) % 16;

    // float index = phase * 4096.0;
    // uint16_t integral = static_cast<uint16_t>(index) & ~0x1;
    // float fractional = index - integral;
    
    // uint16_t nextIntegral = (integral + 1) % 2048;
    
    // int16_t sample2 = 0;
    // int16_t samples[32];

    /*
      one read is:
          write - 8*5*(4 + 4 + 1 + 1) = 400
          read - 2 * 8 * (2 + 1 + 4 + 4) = 176
    */
    // flash.Read66Mhz((uint8_t *)&sample1, 2, integral, EEPROM_FACTORY_SS);
    // sample1 = 0;
    // flash.Read66Mhz((uint8_t *)&sample1, 2, integral, EEPROM_FACTORY_SS);
    // flash.Read66Mhz((uint8_t *)&sample1, 2, integral, EEPROM_FACTORY_SS);
    // flash.Read66Mhz((uint8_t *)&sample1, 2, integral, EEPROM_FACTORY_SS);
    // flash.Read66Mhz((uint8_t *)samples, 64, 0, EEPROM_FACTORY_SS);

    // flash.Read66Mhz((uint8_t *)&sample1, 2, index, EEPROM_FACTORY_SS);
    // flash.Read66Mhz((uint8_t *)&sample1, 2, index, EEPROM_FACTORY_SS);

    // flash.Read66Mhz((uint8_t *)&sample1, 2, 4096 * frame_integral + integral * 2, EEPROM_FACTORY_SS);
    // flash.Read66Mhz((uint8_t *)&sample1, 2, 4096 * frame + nextIntegral * 2, EEPROM_FACTORY_SS);

    // flash.Read66Mhz((uint8_t *)&sample2, 2, 4096 * next_frame_integral + integral * 2, EEPROM_FACTORY_SS);
    // flash.Read66Mhz((uint8_t *)&sample2, 2, 4096 * frame + nextIntegral * 2, EEPROM_FACTORY_SS);

      // sample1 = sample1 * (1.0f - frame_fractional) + sample2 * frame_fractional;
      // sample = sample1;

    // index += 0;
    // integral += 0;
    // nextIntegral += 0;
    // fractional += 0;
    // sample1 += 0;
    // sample2 += 0;

      float interpolated_morph = morph_interpolator.Next();
      interpolated_morph = CLAMP<float>(interpolated_morph, 0.0, 0.9999f);

      float sample = 0;
      for (size_t j = 0; j < kOversampling; ++j) {
          
          float frame = interpolated_morph * 15.0f;
          uint16_t frame_integral = floor(frame);
          float frame_fractional = frame - frame_integral;
          // frame_fractional = 0.0f;
          // uint16_t next_frame_integral = (frame_integral + 1) % 16;

          float index = phase * 2048.0;
          uint16_t integral = floor(index);
          float fractional = index - integral;
          uint16_t nextIntegral = (integral + 1) % 2048;
          // swap = false;

          if(!swap) {
            int16_t sample1 = 0;
            int16_t sample2 = 0;

            int16_t * buf = front_buffer;
            sample1 = buf[integral] + (buf[nextIntegral] - buf[integral]) * fractional;
            sample2 = buf[2048 + integral] + (buf[2048 + nextIntegral] - buf[2048 + integral]) * fractional;

            if(frame_integral > current_frame_)
              sample = sample2;
            else if(frame_integral < current_frame_)
              sample = sample1;
            else
              sample = sample1 * (1.0f - frame_fractional) + sample2 * frame_fractional;
          }
          else {
            int16_t sample1 = 0;
            int16_t sample2 = 0;

            int16_t * buf = front_buffer;
            sample1 = buf[integral] + (buf[nextIntegral] - buf[integral]) * fractional;
            sample2 = buf[2048 + integral] + (buf[2048 + nextIntegral] - buf[2048 + integral]) * fractional;

            /*

            0 to 2.  0... morph ... 2.0 front buffer.. morph is from 0 to 1.0.
            when morph approaches 1.0, a new frame is loaded.

            */
            if(target_frame_ > current_frame_)
              sample1 = sample2;
            else if(current_frame_ > target_frame_)
              sample1 = sample1;
            else
              sample1 = sample1 * (1.0f - frame_fractional) + sample2 * frame_fractional;

            int16_t sample3 = 0;
            int16_t sample4 = 0;

            buf = back_buffer;
            sample3 = buf[integral] + (buf[nextIntegral] - buf[integral]) * fractional;
            sample4 = buf[2048 + integral] + (buf[2048 + nextIntegral] - buf[2048 + integral]) * fractional;

            // if(frame_integral > target_frame_)
            //   sample3 = sample4;
            // else if(frame_integral < target_frame_)
            //   sample3 = sample3;
            // else
              sample3 = sample3 * (1.0f - frame_fractional) + sample4 * frame_fractional;
            // sample3 += 0;
            // sample4 += 0;
            sample = sample1 * (1.0f - swap_counter) + sample3 * swap_counter;
            // sample = sample1;
            // sample = sample1;
          }

          phase += phase_increment;
          
          if(phase >= 1.0f)
              phase -= 1.0f;
          
          if(swap) {
            swap_counter += swap_increment;
            if(swap_counter >= 1.0f) {
              swap = false;
              if(front_buffer == double_waveframe_buffer_1) {
                front_buffer = double_waveframe_buffer_2;
                back_buffer = double_waveframe_buffer_1;
              } else {
                front_buffer = double_waveframe_buffer_1;
                back_buffer = double_waveframe_buffer_2;      
              }
              current_frame_ = target_frame_;
              swap_counter = 0.0f;

              SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
              SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
            }
          }


          carrier_downsampler.Accumulate(j, sample / 32768.0f);
      }
      
      sample = carrier_downsampler.Read();
      

      // int16_t sample1 = 0;
      // uint8_t sample2[2];
      // int16_t sample2 = 0;
      // int16_t sample3 = 0;
      // interpolated16 = flash.LoadWaveSample(0, morph, phase);
      // interpolated16 = flash.LoadWaveSample(0, morph, phase);
      // interpolated16 = flash.LoadWaveSample(0, morph, phase);
      // interpolated16 = flash.LoadWaveSample(0, morph, phase);
      // interpolated16 = flash.LoadWaveSample(0, morph, phase);

      // flash.ReadStatusRegister(EEPROM_FACTORY_SS);

      // flash.Read66Mhz((uint8_t *)&sample1, 2, integral * 2, EEPROM_FACTORY_SS);
      // flash.Read66Mhz((uint8_t *)&sample2, 2, nextIntegral * 2, EEPROM_FACTORY_SS);

      // sample1 = sample1 + (sample2 - sample1) * fractional;
      // for(uint32_t i = 0; i < 128; i++) {
      // flash.HIGH(GPIOA, GPIO_Pin_11);
      // flash.LOW(GPIOA, GPIO_Pin_11);
      // }

      // uint32_t address = integral * 2;
      // flash.LOW(EEPROM_FACTORY_SS);
      // flash.Wait<200>();
      // uint8_t buf[5];
      // buf[0] = READ_66MHZ;
      // buf[1] = ((address >> 16) & 0xFF);
      // buf[2] = ((address >> 8) & 0xFF);
      // buf[3] = ((address) & 0xFF);
      // buf[4] = 0x00;

      // buf[4] += 0;
      // flash.HIGH(EEPROM_FACTORY_SS);
      // flash.LOW(EEPROM_MOSI);
      // flash.HIGH(EEPROM_MOSI);

      // for(uint32_t i = 0; i < 10; i++) {

      //     uint8_t bsize = 8;

      //     while(bsize) {
      //         // bool set = (buf[i] >> (bsize - 1)) & 0x1;
      //         // if(set)
      //             // flash.HIGH(EEPROM_FACTORY_SS);
      //         // else
      //             // flash.LOW(EEPROM_MOSI);
      //         // sample += 0.0f;
      //         //clock high
      //         flash.HIGH(EEPROM_CLOCK);

      //         flash.Wait<1>();

      //         test += 1.1233124124124f;
      //         sample += 0.0f;
      //         // clock low
      //         // flash.LOW(EEPROM_CLOCK);

      //         bsize--;
      //     }
      // }
      // // flash.Write(send_buf, 5);

      // flash.HIGH(EEPROM_FACTORY_SS);
      // test += 0.0f;
      // while(flash.ReadStatusRegister(EEPROM_FACTORY_SS) & 0x01);  // BUSY
      // flash.Wait<200>();
      // sample1 = 0;
      // flash.Read66Mhz((uint8_t *)&sample1, 2, integral * 2, EEPROM_FACTORY_SS);
      // flash.Read66Mhz((uint8_t *)&sample3, 2, integral * 2, EEPROM_FACTORY_SS);
      // flash.Read66Mhz((uint8_t *)&sample3, 2, integral * 2, EEPROM_FACTORY_SS);
      // flash.Read66Mhz((uint8_t *)&sample3, 2, integral * 2, EEPROM_FACTORY_SS);
      // flash.Read66Mhz((uint8_t *)&sample3, 2, integral * 2, EEPROM_FACTORY_SS);
      // flash.Read66Mhz((uint8_t *)&sample3, 2, integral * 2, EEPROM_FACTORY_SS);

      // test_ramp = static_cast<uint32_t>(4294967296 * (sample + 1.0f) / 2.0f);
      // output->l = static_cast<int32_t>(32767.0f * (sample + 1.0f) / 2.0f);
      // output->l = static_cast<int32_t>(20000.0f * sample);
      // output->l = static_cast<int16_t>(interpolated16 * 26000.0f);
      output->l = static_cast<int32_t>(26000.0f * sample);
      // output->l = sample1;
      // output->l = static_cast<int16_t>(sample / 1.5f);
      // output->l = static_cast<int16_t>(sample1 / 1.5f);
      // output->l = ~test_ramp >> 16;
      // output->l = 0;
      output->r = 0;//test_ramp >> 16;
      // test_ramp += 8947848;  // 480.. 48000
      /*
      480 cycles per one wavelength
      12 hz.  12 * 480 cycles per second
      5760 cycles per second



      //*/
      ++output;
      // phase += phase_increment;
      // if(phase >= 1.0f)
      //   phase -= 1.0f;

    }
}

// IOBuffer::Slice FillBuffer(size_t size) {
//   if (PROFILE_INTERRUPT) {
//     TIC;
//   }
//   IOBuffer::Slice s = io_buffer.NextSlice(size);
  
//   gate_outputs.Write(s);
//   clock_inputs.Read(s, size);

//   if (io_buffer.new_block()) {
//     cv_reader.Copy(&s.block->adc_value[0]);
//     clock_inputs.ReadNormalization(s.block);
//   }

//   if (rng.readable()) {
//     random_stream.Write(rng.data());
//   }

//   if (PROFILE_INTERRUPT) {
//     TOC;
//   }
  
//   return s;
// }

// inline uint16_t DacCode(int index, float voltage) {
//   CONSTRAIN(voltage, -5.0f, 5.0f);
//   const float scale = settings.calibration_data().dac_scale[index];
//   const float offset = settings.calibration_data().dac_offset[index];
//   return ClipU16(static_cast<int32_t>(voltage * scale + offset));
// }

// void ProcessTest(IOBuffer::Block* block, size_t size) {
//   float parameters[kNumParameters];
//   GateFlags hidden_gates[kNumParameters];
//   static float phase;
//   cv_reader.Process(false, &block->adc_value[0], parameters, hidden_gates);
//   for (size_t i = 0; i < size; ++i) {
//     phase += 100.0f / static_cast<float>(kSampleRate);
//     if (phase >= 1.0f) {
//       phase -= 1.0f;
//     }
//     block->cv_output[0][i] = DacCode(
//         0, 4.0 * Interpolate(lut_sine, phase, 256.0f));
//     block->cv_output[1][i] = DacCode(
//         1, -8.0f * phase + 4.0f);
//     block->cv_output[2][i] = DacCode(
//         2, (phase < 0.5f ? phase : 1.0f - phase) * 16.0f - 4.0f);
//     block->cv_output[3][i] = DacCode(
//         3, phase < 0.5f ? -4.0f : 4.0f);

//     for (int j = 0; j < 4; ++j) {
//       uint16_t dac_code = ui.output_test_forced_dac_code(j);
//       if (dac_code) {
//         block->cv_output[j][i] = dac_code;
//       }
//     }
    
//     block->gate_output[0][i] = block->input_patched[0]
//         ? block->input[0][i]
//         : phase < 0.2f;
//     block->gate_output[1][i] = phase < 0.5f;
//     block->gate_output[2][i] = block->input_patched[1]
//         ? block->input[1][i]
//         : phase < 0.8f;
//   }
// }

// Ratio y_divider_ratios[] = {
//   { 1, 64 },
//   { 1, 48 },
//   { 1, 32 },
//   { 1, 24 },
//   { 1, 16 },
//   { 1, 12 },
//   { 1, 8 },
//   { 1, 6 },
//   { 1, 4 },
//   { 1, 3 },
//   { 1, 2 },
//   { 1, 1 },
// };

// int loop_length[] = {
//   1,
//   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
//   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
//   4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
//   5, 5, 5, 5,
//   6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
//   7, 7,
//   8, 8, 8, 8, 8, 8, 8, 8, 8,
//   10, 10, 10,
//   12, 12, 12, 12, 12, 12, 12,
//   14, 14,
//   16
// };
// GateFlags hidden_gates[kNumParameters];
// float parameters[kNumParameters];
// float ramp_buffer[kBlockSize * 4];
// bool gates[kBlockSize * 2];
// float voltages[kBlockSize * 4];
// Ramps ramps;
// GroupSettings x, y;
// bool gate_delay_tail[kNumGateOutputs][kGateDelay];

// float SineOscillator(float voltage) {
//   static float phase = 0.0f;
//   CONSTRAIN(voltage, -5.0f, 5.0f);
//   float frequency = stmlib::SemitonesToRatio(voltage * 12.0f) * 220.0f / kSampleRate;
//   phase += frequency;
//   if (phase >= 1.0f) {
//     phase -= 1.0f;
//   }
//   return 5.0f * Interpolate(lut_sine, phase, 256.0f);
// }

// void Process(IOBuffer::Block* block, size_t size) {
//   if (PROFILE_RENDER) {
//     TIC;
//   }

//   // Filter CV values (3.5%)
//   cv_reader.Process(
//       settings.explicit_reset(),
//       &block->adc_value[0],
//       parameters,
//       hidden_gates);
  
//   float deja_vu = parameters[ADC_CHANNEL_DEJA_VU_AMOUNT];
  
//   //  Deadband near 12 o'clock for the deja vu parameter.
//   const float d = fabsf(deja_vu - 0.5f);
//   if (d > 0.03f) {
//     ui.set_deja_vu_lock(false);
//   } else if (d < 0.02f) {
//     ui.set_deja_vu_lock(true);
//   }
//   if (deja_vu < 0.47f) {
//     deja_vu *= 1.06382978723f;
//   } else if (deja_vu > 0.53f) {
//     deja_vu = 0.5f + (deja_vu - 0.53f) * 1.06382978723f;
//   } else {
//     deja_vu = 0.5f;
//   }
  
//   GateFlags* t_clock = block->input[0];
//   GateFlags* xy_clock = block->input[1];
  
//   // Determine the clock source for the XY section (2%)
//   ClockSource xy_clock_source = CLOCK_SOURCE_INTERNAL_T1_T2_T3;
//   if (block->input_patched[1]) {
//     xy_clock_source = CLOCK_SOURCE_EXTERNAL;
//     size_t best_score = 8;
//     for (size_t i = 0; i < kNumGateOutputs; ++i) {
//       size_t score = self_patching_detector[i].Process(block, size);
//       if (score >= best_score) {
//         xy_clock_source = ClockSource(CLOCK_SOURCE_INTERNAL_T1 + i);
//         best_score = score;
//       }
//     }
//   }
  
//   // Generate gates for T-section (16%).
//   ramps.master = &ramp_buffer[0];
//   ramps.external = &ramp_buffer[kBlockSize];
//   ramps.slave[0] = &ramp_buffer[kBlockSize * 2];
//   ramps.slave[1] = &ramp_buffer[kBlockSize * 3];
  
//   const State& state = settings.state();
//   int deja_vu_length = deja_vu_length_quantizer.Lookup(
//       loop_length,
//       parameters[ADC_CHANNEL_DEJA_VU_LENGTH]);
  
//   bool t_section_reset = settings.explicit_reset() && hidden_gates[ADC_CHANNEL_T_JITTER] & GATE_FLAG_RISING;
  
//   t_generator.set_model(TGeneratorModel(state.t_model));
//   t_generator.set_range(TGeneratorRange(state.t_range));
//   t_generator.set_rate(parameters[ADC_CHANNEL_T_RATE]);
//   t_generator.set_bias(parameters[ADC_CHANNEL_T_BIAS]);
//   t_generator.set_jitter(parameters[ADC_CHANNEL_T_JITTER]);
//   t_generator.set_deja_vu(
//       state.t_deja_vu == DEJA_VU_LOCKED
//           ? 0.5f
//           : (state.t_deja_vu == DEJA_VU_ON ? deja_vu : 0.0f));
//   t_generator.set_length(deja_vu_length);
//   t_generator.set_pulse_width_mean(float(state.t_pulse_width_mean) / 256.0f);
//   t_generator.set_pulse_width_std(float(state.t_pulse_width_std) / 256.0f);
//   t_generator.Process(
//       block->input_patched[0],
//       &t_section_reset,
//       t_clock,
//       ramps,
//       gates,
//       size);

//   // Generate voltages for X-section (40%).
//   float note_cv_1 = cv_reader.channel(ADC_CHANNEL_X_SPREAD).scaled_raw_cv();
//   float note_cv_2 = cv_reader.channel(ADC_CHANNEL_X_SPREAD_2).scaled_raw_cv();
//   float note_cv = 0.5f * (note_cv_1 + note_cv_2);
//   float u = note_filter.Process(0.5f * (note_cv + 1.0f));
  
//   if (test_adc_noise) {
//     static float note_lp = 0.0f;
//     float note = note_cv_1;
//     ONE_POLE(note_lp, note, 0.0001f);
//     float cents = (note - note_lp) * 1200.0f * 5.0f;
//     fill(&voltages[0], &voltages[4 * size], cents);
//   } else if (ui.recording_scale()) {
//     float voltage = (u - 0.5f) * 10.0f;
//     for (size_t i = 0; i < size; ++i) {
//       GateFlags gate = block->input_patched[1]
//           ? block->input[1][i]
//           : GATE_FLAG_LOW;
//       if (gate & GATE_FLAG_RISING) {
//         scale_recorder.NewNote(voltage);
//       }
//       if (gate & GATE_FLAG_HIGH) {
//         scale_recorder.UpdateVoltage(voltage);
//       }
//       if (gate & GATE_FLAG_FALLING) {
//         scale_recorder.AcceptNote();
//       }
//     }
//     fill(&voltages[0], &voltages[4 * size], voltage);
//   } else {
//     x.control_mode = ControlMode(state.x_control_mode);
//     x.voltage_range = VoltageRange(state.x_range % 3);
//     x.register_mode = state.x_register_mode;
//     x.register_value = u;
//     cv_reader.set_attenuverter(
//         ADC_CHANNEL_X_SPREAD, state.x_register_mode ? 0.5f : 1.0f);
  
//     x.spread = parameters[ADC_CHANNEL_X_SPREAD];
//     x.bias = parameters[ADC_CHANNEL_X_BIAS];
//     x.steps = parameters[ADC_CHANNEL_X_STEPS];
//     x.deja_vu = state.x_deja_vu == DEJA_VU_LOCKED
//         ? 0.5f
//         : (state.x_deja_vu == DEJA_VU_ON ? deja_vu : 0.0f);
//     x.length = deja_vu_length;
//     x.ratio.p = 1;
//     x.ratio.q = 1;
  
//     y.control_mode = CONTROL_MODE_IDENTICAL;
//     y.voltage_range = VoltageRange(state.y_range);
//     y.register_mode = false;
//     y.register_value = 0.0f;
//     y.spread = float(state.y_spread) / 256.0f;
//     y.bias = float(state.y_bias) / 256.0f;
//     y.steps = float(state.y_steps) / 256.0f;
//     y.deja_vu = 0.0f;
//     y.length = 1;
//     y.ratio = y_divider_ratios[
//         static_cast<uint16_t>(state.y_divider) * 12 >> 8];
    
//     if (settings.dirty_scale_index() != -1) {
//       int i = settings.dirty_scale_index();
//       xy_generator.LoadScale(i, settings.persistent_data().scale[i]);
//       settings.set_dirty_scale_index(-1);
//     }
    
//     y.scale_index = x.scale_index = state.x_scale;
    
//     bool x_section_reset = settings.explicit_reset() && hidden_gates[ADC_CHANNEL_X_STEPS] & GATE_FLAG_RISING;
//     if (xy_clock_source != CLOCK_SOURCE_EXTERNAL) {
//       x_section_reset |= t_section_reset;
//     }

//     xy_generator.Process(
//         xy_clock_source,
//         x,
//         y,
//         &x_section_reset,
//         xy_clock,
//         ramps,
//         voltages,
//         size);
//   }
  
//   const float* v = voltages;
//   const bool* g = gates;
//   for (size_t i = 0; i < size; ++i) {
//     //block->cv_output[1][i] = DacCode(1, SineOscillator(*v++));
//     block->cv_output[1][i] = DacCode(1, *v++);
//     block->cv_output[2][i] = DacCode(2, *v++);
//     block->cv_output[3][i] = DacCode(3, *v++);
//     block->cv_output[0][i] = DacCode(0, *v++);
//     block->gate_output[0][i + kGateDelay] = *g++;
//     block->gate_output[1][i + kGateDelay] = ramps.master[i] < 0.5f;
//     block->gate_output[2][i + kGateDelay] = *g++;
//   }
  
//   for (size_t i = 0; i < kNumGateOutputs; ++i) {
//     for (size_t j = 0; j < kGateDelay; ++j) {
//       block->gate_output[i][j] = gate_delay_tail[i][j];
//       gate_delay_tail[i][j] = block->gate_output[i][size + j];
//     }
//   }
  
//   if (PROFILE_RENDER) {
//     TOC;
//   }
// }


void Init() {
  System sys;
  sys.Init(true);

  // printf("hi\n");

  // Enable TRCENA
  // DEMCR |= ( 1 << 24);
  // Enable stimulus port 0
  // ITM_TRACE_EN |= ( 1 << 0);

  // settings.Init();
  
  // clock_inputs.Init();
  // dac.Init(kSampleRate, 1);
  // rng.Init();
  // note_filter.Init();
  // gate_outputs.Init();
  // io_buffer.Init();
    
  // deja_vu_length_quantizer.Init(
  //     sizeof(loop_length) / sizeof(int), 0.25f, false);
  // cv_reader.Init(settings.mutable_calibration_data());
  // scale_recorder.Init();
  // ui.Init(&settings, &cv_reader, &scale_recorder, &clock_inputs);
  
  // if (settings.freshly_baked()) {
  //   settings.ProgramOptionBytes();
  //   if (PROFILE_INTERRUPT || PROFILE_RENDER) {
  //     DebugPin::Init();
  //   } else {
  //     debug_port.Init();
  //   }
  // }
  
  // random_generator.Init(1);
  // random_stream.Init(&random_generator);
  // t_generator.Init(&random_stream, static_cast<float>(kSampleRate));
  // xy_generator.Init(&random_stream, static_cast<float>(kSampleRate));

  // for (size_t i = 0; i < kNumScales; ++i) {
  //   xy_generator.LoadScale(i, settings.persistent_data().scale[i]);
  // }
  
  // for (size_t i = 0; i < kNumGateOutputs; ++i) {
  //   self_patching_detector[i].Init(i);
  // }
  adc.Init(false);
  flash.Init();
  // flash.InitMemory();

  
  sys.StartTimers();

  audio_dac.Init(48000, kBlockSize);
  audio_dac.Start(&FillBuffer);


  // logger.Init(9600);
  // dac.Start(&FillBuffer);
}

int main(void) {
  Init();
  bool fresh_start = true;
  while (1) {
    if(fresh_start) {
      fresh_start = false;
      flash.W25qxx_Init();
    }
    // ui.DoEvents();
    // io_buffer.Process(ui.output_test_mode() ? &ProcessTest : &Process);
  }
}
