/*
  ==============================================================================

    Context.h
    Created: 7 Nov 2023 1:06:53pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#ifndef WAVES_2_DSP_CONTEXT_H
#define WAVES_2_DSP_CONTEXT_H

#include "stmlib/stmlib.h"

#include "stmlib/utils/buffer_allocator.h"

#include "waves2/dsp/engine/engine.h"
#include "waves2/dsp/engine/child_a.h"
#include "waves2/dsp/engine/child_b.h"

namespace waves2 {

const int kMaxEngines = 24;

class Context {
public:
    Context() {}
    ~Context() {}
        
    void Init(stmlib::BufferAllocator* allocator);
    void Paint();


private:
    uint8_t index_;
    bool was_pressed_;
    EngineRegistry<kMaxEngines> engines_;
    ChildA childA_;
    ChildA childB_;
    DISALLOW_COPY_AND_ASSIGN(Context);
};

}

#endif