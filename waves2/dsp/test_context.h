/*
  ==============================================================================

    Context.h
    Created: 7 Nov 2023 1:06:53pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#ifndef WAVES_2_DSP_TEST_CONTEXT_H
#define WAVES_2_DSP_TEST_CONTEXT_H

#include "stmlib/stmlib.h"

#include "stmlib/utils/buffer_allocator.h"

#include "waves2/dsp/engine/test.h"
#include "waves2/dsp/engine/test_menu.h"
#include "waves2/dsp/engine/test_five.h"

namespace waves2 {

const int kMaxEngines = 24;

class TestContext {
public:
    TestContext() {}
    ~TestContext() {}
        
    void Init(stmlib::BufferAllocator* allocator);
    void Paint();


private:
    uint8_t index_;
    bool was_pressed_;
    EngineRegistry<kMaxEngines> engines_;
    TestMenu mainMenuTest_;
    TestFive test_five_;
    DISALLOW_COPY_AND_ASSIGN(TestContext);
};

}

#endif