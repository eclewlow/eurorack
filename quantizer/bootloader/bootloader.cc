// Copyright 2014 Emilie Gillet.
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

#include "stmlib/system/bootloader_utils.h"
#include "stmlib/system/system_clock.h"

#include "quantizer/drivers/system.h"
#include "quantizer/drivers/dac.h"
#include "quantizer/drivers/adc.h"
#include "quantizer/drivers/internal_adc.h"
#include "quantizer/drivers/quantizer.h"
#include "stmlib/stmlib.h"

#include <stm32f10x_conf.h>


using namespace quantizer;
using namespace std;
using namespace stmlib;

const uint32_t kStartAddress = 0x08008000;

int __errno;
Dac dac;
Adc adc;
InternalAdc internal_adc;
Quantizer Q;

#define SAMPLE_RATE (96000)

uint16_t last_chord_type = 0;
uint32_t chord_pot_width = 455;//4096 / kChordTableLength;
uint32_t chord_pot_width_2 = 227;//chord_pot_width / 2;
uint32_t pot_normalization = 247;
uint16_t midi_test;
uint32_t midi_test_timer;
uint16_t chord_type;
uint16_t transpose;
int32_t chaos;
uint16_t slew;
uint16_t v_oct_input;
int16_t chaos_cv_input;


// Default interrupt handlers.
extern "C" {

  void NMI_Handler() { }
  void HardFault_Handler() { while (1); }
  void MemManage_Handler() { while (1); }
  void BusFault_Handler() { while (1); }
  void UsageFault_Handler() { while (1); }
  void SVC_Handler() { }
  void DebugMon_Handler() { }
  void PendSV_Handler() { }
}

extern "C" {

  void SysTick_Handler() {
    system_clock.Tick();
  }


  void TIM1_UP_IRQHandler() {
    if (!(TIM1->SR & TIM_IT_Update)) {
      return;
    }
    TIM1->SR = (uint16_t)~TIM_IT_Update;


    chord_type = internal_adc.value(0);
    transpose = internal_adc.value(1);
    chaos = internal_adc.value(2);
    slew = internal_adc.value(3);


    bool adc_scan_cycle_complete = adc.PipelinedScan();

    if (adc_scan_cycle_complete) {
      v_oct_input = adc.channel(0);
      chaos_cv_input = adc.channel(1); // 0 - 4095
    }


    switch (2) {
    case 1:
    // case 1: +/- 5 v cv -5v => 0 chaos
    // chaos = (chaos - 2047) * chaos_cv_input / 4095 + 2047;
    case 2:
    // case 2: -5 v is -chaos , +5 is +chaos
    // chaos = (chaos - 2047) * (chaos_cv_input - 2047) / 2048 + 2047;
    default:
      break;
    }


    bool apply_pot_hysterisis = true;

    if (apply_pot_hysterisis) {

      uint32_t last_chord_center = last_chord_type * chord_pot_width + chord_pot_width_2;

      if (last_chord_type == 0 && chord_type < chord_pot_width_2 + pot_normalization) {
        chord_type = last_chord_type;
      }
      else if (last_chord_type == (kChordTableLength - 1) && chord_type > (kChordTableLength - 1) * chord_pot_width + chord_pot_width_2 - pot_normalization ) {
        chord_type = last_chord_type;
      }
      else if (chord_type < last_chord_center + pot_normalization && chord_type > last_chord_center - pot_normalization) {
        chord_type = last_chord_type;
      }
      else {
        chord_type = kChordTableLength * chord_type / 4096;
        last_chord_type = chord_type;
      }
    } else {
      chord_type = kChordTableLength * chord_type / 4096;
    }

    Q.set_scale(chord_type);

    int16_t q;

    q = Q.Quantize(v_oct_input, transpose, slew, chaos);

    uint16_t output = static_cast<uint16_t>(q);

    dac.Write(output, 0);
    // dac.Write(midi_test, 0);

    midi_test_timer += 1;
    if (midi_test_timer > 6000) {
      midi_test += 69 ;
      midi_test_timer -= 6000;
    }

  }

}


void Init() {
  System sys;

  system_clock.Init();
  sys.Init(F_CPU / 96000 - 1, true);

  dac.Init();
  adc.Init();
  internal_adc.Init();

  Q.Init();

  sys.StartTimers();
}

int main(void) {
  Init();
  while (1) {

    // uint32_t ms = system_clock.milliseconds() % 2000;
    // if(ms > 1000) {
    //   GPIOC->BRR = GPIO_Pin_13;
    //   // dac.Write(static_cast<uint16_t>(4095.0 * (5.0+2.0)/10.0));
    //   // dac.Write(2048,0);
    //   midi_test = 0;
    // } else {
    //   GPIOC->BSRR = GPIO_Pin_13;
    //   // dac.Write(static_cast<uint16_t>(4095.0 * (5.0+2.0)/10.0));
    //   // dac.Write(0);
    //   midi_test = 2047;
    // }

  }
}
