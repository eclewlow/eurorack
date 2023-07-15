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

#include "test/drivers/system.h"
#include "test/drivers/dac.h"
#include "stmlib/stmlib.h"

#include <stm32f4xx_conf.h>

#include <cstring>
#include <math.h>

using namespace test;
using namespace stmlib;

const uint32_t kStartAddress = 0x08008000;

int __errno;
Dac dac;

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

void SysTick_Handler() {
  system_clock.Tick();
}

uint32_t sample_rate = 48000;
// uint32_t sample_rate = 1;

float freq = 440.0 / 48000.0;
// float freq = 0.25;
float phase = 0.0;

void TIM2_IRQHandler() {
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    // tides::FirmwareUpdateDac::GetInstance()->NextSample();

    // dac.Write(4095);
    // dac.Write(0);
    phase += freq;
    if (phase >= 1.0) {
      phase -= 1.0;
    }

    float sample = sinf(M_TWOPI*phase);

    // sample += 1.0;
    // sample += 1.0; // now 0 - 2.0
    // sample /= 2.0; // now 0 - 1.0;
    sample = 4095.0 * (5.0+sample/3.0)/10.0;
    // sample = 4095.0/2.0;
    // dac.Write( static_cast<uint16_t>(sample));
    // sample ++;
    dac.Write(static_cast<uint16_t>(sample));

    // uint32_t ms = system_clock.milliseconds() % 2000;
    // if(ms > 1000) {
    //   // dac.Write(static_cast<uint16_t>(sample));
    // } else {
    //   // dac.Write(0);
    // }
  }
}



}


void Init() {
  System sys;
  
  sys.Init((F_CPU / 2) / sample_rate - 1, true);
  dac.Init();

  sys.StartTimers();

}

int main(void) {
  Init();
  while (1) { 
    // GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
    // system_clock.Delay(2000);
    //   dac.Write(static_cast<uint16_t>(4095.0 * (5.0+2.0)/10.0));
    // system_clock.Delay(2000);
    //   dac.Write(0);
    // uint32_t ms = system_clock.milliseconds() % 2000;
    // if(ms > 1000) {
    //   dac.Write(static_cast<uint16_t>(4095.0 * (5.0+2.0)/10.0));
    //   // dac.Write(0);
    // } else {
    //   // dac.Write(static_cast<uint16_t>(4095.0 * (5.0+2.0)/10.0));
    //   dac.Write(0);
    // }

  }
}
