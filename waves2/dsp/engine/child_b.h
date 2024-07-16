/*
  ==============================================================================

    MainMenu.h
    Created: 7 Nov 2023 1:30:39pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#ifndef WAVES_2_DSP_ENGINE_CHILD_B_H
#define WAVES_2_DSP_ENGINE_CHILD_B_H

#include "waves2/dsp/engine/engine.h"

namespace waves2 {

class ChildB: public Engine
{
public:

    ChildB() { }
    ~ChildB() { }

    virtual void Init(stmlib::BufferAllocator* allocator);
    virtual void Paint();

private:
    DISALLOW_COPY_AND_ASSIGN(ChildB);
};

}

#endif