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

#include "test/drivers/system.h"

// #define PROFILE_INTERRUPT 1

using namespace test;
using namespace stmlib;


int __errno;

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
}

}

void Init() {
  System sys;
  
  sys.Init(true);
  sys.StartTimers();
}

int main(void) {
  Init();
  while (1) {
    // ui.DoEvents();
    // processor.Prepare();
  }
}
