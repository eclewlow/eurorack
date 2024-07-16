/*
  ==============================================================================

    Context.cpp
    Created: 7 Nov 2023 1:06:53pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include <stm32f4xx_conf.h>
#include "stmlib/system/system_clock.h"
#include "waves2/dsp/test_context.h"

namespace waves2 {

using namespace std;
using namespace stmlib;

void TestContext::Init(BufferAllocator* allocator) {

  engines_.Init();

  // mainMenuTest_.Init();
  // test_five_.Init();

  engines_.RegisterInstance(&mainMenuTest_);
  engines_.RegisterInstance(&test_five_);

  for (int i = 0; i < engines_.size(); ++i) {
    // All engines will share the same RAM space.
    allocator->Free();
    // engines_.get(i)->Init(allocator);
  }

  // engines_.get(0)->Paint();
  // mainMenuTest_.Paint();
  // Move* e = &mainMenuTest_;
  // e->Paint();

  index_ = 0;
  was_pressed_ = false;
}

void TestContext::Paint() {

  if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) > 0 && ! was_pressed_) {
    was_pressed_ = true;
    if(index_ == 0)
      index_ = 1;
    else
      index_ = 0;
  } else {
    was_pressed_ = false;
  }
  Move* e = engines_.get(1);

  e->Paint();
  // test_five_.Paint();
}

}