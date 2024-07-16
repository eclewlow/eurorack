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

#include "stmlib/system/system_clock.h"
#include "waves2/drivers/system.h"
#include "waves2/dsp/context.h"

using namespace waves2;
using namespace std;
using namespace stmlib;

Context context;
ChildA childA;
char shared_buffer[16384];

extern "C" {

void NMI_Handler() { }
void HardFault_Handler() { while (1); }
void MemManage_Handler() { while (1); }
void BusFault_Handler() { while (1); }
void UsageFault_Handler() { while (1); }
void SVC_Handler() { }
void DebugMon_Handler() { }
void PendSV_Handler() { }
void __cxa_pure_virtual() { while (1); }

void SysTick_Handler() {
  // IWDG_ReloadCounter();
  system_clock.Tick();
}

}

void Init() {
  System sys;

  sys.Init(true);

  sys.StartTimers();

  GPIO_ResetBits(GPIOA, GPIO_Pin_5);

  BufferAllocator allocator(shared_buffer, 16384);
  context.Init(&allocator);

  childA.Paint();
  Engine* e = &childA;

  volatile size_t counter = 1000000;
  while (counter--);

  e->Paint();

  GPIO_SetBits(GPIOA, GPIO_Pin_5);

}

int main(void) {
  Init();
  while (1) {
    // context.Paint();
    if((system_clock.milliseconds() / 1000) % 2 < 1) {
      GPIO_SetBits(GPIOA, GPIO_Pin_5);
    } else {
      GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    }
  }
}
