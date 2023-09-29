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

#include "stmlib/system/bootloader_utils.h"
#include "stmlib/system/system_clock.h"

#include "quantizer2/drivers/system.h"
#include "quantizer2/drivers/dac.h"
#include "quantizer2/drivers/adc.h"
#include "quantizer2/drivers/internal_adc.h"
#include "quantizer2/drivers/quantizer.h"
#include "stmlib/stmlib.h"

#include <stm32f10x_conf.h>


using namespace quantizer2;
using namespace std;
using namespace stmlib;

const uint32_t kStartAddress = 0x08008000;

int __errno;
Dac dac;
Adc adc;
InternalAdc internal_adc;
Quantizer Q;

#define SAMPLE_RATE (96000)

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
    // transpose = internal_adc.value(1);
    chaos = internal_adc.value(1);
    slew = internal_adc.value(2);


    bool adc_scan_cycle_complete = adc.PipelinedScan();

    if (adc_scan_cycle_complete) {
      v_oct_input = adc.channel(0);
      // chaos_cv_input = adc.channel(1); // 0 - 4095
    }

    // slew = 0;
    // chaos = 2045;
    // chord_type = 4095;

    chord_type = kChordTableLength * chord_type / 4096;

    Q.set_scale(chord_type);

    int16_t q;

    q = Q.Quantize(v_oct_input, slew, chaos);

    uint16_t output = static_cast<uint16_t>(q);

    dac.Write(output, 0);
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
