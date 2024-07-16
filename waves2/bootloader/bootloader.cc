
// #include "stmlib/system/bootloader_utils.h"
#include "stmlib/system/system_clock.h"
#include "waves2/drivers/system.h"
#include "stmlib/system/bootloader_utils.h"

using namespace waves2;
using namespace stmlib;

const uint32_t kStartAddress = 0x08008000;

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
  // IWDG_ReloadCounter();
  system_clock.Tick();
}

}

void Init() {
}

int main(void) {
  Init();
  JumpTo(kStartAddress);
  while (1) { }
}
