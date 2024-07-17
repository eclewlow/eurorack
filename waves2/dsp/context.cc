/*
  ==============================================================================

    context.cpp
    Created: 7 Nov 2023 1:06:53pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include <stm32f4xx_conf.h>
#include "stmlib/system/system_clock.h"
#include "waves2/dsp/context.h"

namespace waves2 {

using namespace std;
using namespace stmlib;

void Context::Init(BufferAllocator* allocator) {

  engines_.Init();

  engines_.RegisterInstance(&childA_);
  engines_.RegisterInstance(&childB_);

  for (int i = 0; i < engines_.size(); ++i) {
    // All engines will share the same RAM space.
    allocator->Free();
    engines_.get(i)->Init(allocator);
  }

  index_ = 0;
  was_pressed_ = false;
}

void Context::Paint() {

  if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) > 0 && ! was_pressed_) {
    was_pressed_ = true;
    if(index_ == 0)
      index_ = 1;
    else
      index_ = 0;
  } else {
    was_pressed_ = false;
  }
  Engine* e = engines_.get(index_);

  e->Paint();
}

}