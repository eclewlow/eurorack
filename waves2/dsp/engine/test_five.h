/*
  ==============================================================================

    MainMenu.h
    Created: 7 Nov 2023 1:30:39pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#ifndef WAVES_2_DSP_ENGINE_TEST_FIVE_H
#define WAVES_2_DSP_ENGINE_TEST_FIVE_H

#include "waves2/dsp/engine/test.h"

namespace waves2 {

class TestFive: public Move
{
public:

    TestFive() { }
    ~TestFive() { }

    virtual void Init(stmlib::BufferAllocator* allocator);
    virtual void Paint();

private:
    DISALLOW_COPY_AND_ASSIGN(TestFive);
};

}

#endif