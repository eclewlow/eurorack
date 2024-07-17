
// #include "stmlib/system/bootloader_utils.h"
#include "stmlib/system/system_clock.h"
#include "waves2/drivers/system.h"
#include "stmlib/system/bootloader_utils.h"
#include "waves2/dsp/context.h"

using namespace waves2;
using namespace stmlib;

const uint32_t kStartAddress = 0x08008000;
Context context;
ChildA childA;
char shared_buffer[16384];


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
void __cxa_pure_virtual() { while (1); }

void SysTick_Handler() {
  // IWDG_ReloadCounter();
  system_clock.Tick();
}

}

void Init() {
  System sys;

  sys.Init(false);

  sys.StartTimers();

  GPIO_ResetBits(GPIOA, GPIO_Pin_5);

  BufferAllocator allocator(shared_buffer, 16384);
  context.Init(&allocator);

  childA.Paint();
  // Engine* e = &childA;

  volatile size_t counter = 1000000;
  while (counter--);

  // e->Paint();

  GPIO_SetBits(GPIOA, GPIO_Pin_5);

}

int main(void) {
  Init();
  JumpTo(kStartAddress);
  // while (1) { }
  while (1) {
    // context.Paint();
    // if((system_clock.milliseconds() / 1000) % 2 < 1) {
    //   GPIO_SetBits(GPIOA, GPIO_Pin_5);
    // } else {
    //   GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    // }
  }
}
