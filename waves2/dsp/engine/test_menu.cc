/*
 ==============================================================================
 
 MainMenu.cpp
 Created: 7 Nov 2023 1:30:39pm
 Author:  Eugene Clewlow
 
 ==============================================================================
 */

#include <stm32f4xx_conf.h>
#include "stmlib/system/system_clock.h"
#include "waves2/dsp/engine/test_menu.h"

namespace waves2 {

using namespace stmlib;

void TestMenu::Init(BufferAllocator* allocator) {
}

void TestMenu::Paint() {

	// GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
	// toggle every 5 seconds
	// if((system_clock.milliseconds() / 1000) % 4 < 2) {
	// 	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	// } else {
	// 	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	// }

}

}