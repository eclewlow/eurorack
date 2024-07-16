/*
  ==============================================================================

    engine.h
    Created: 7 Nov 2023 1:06:48pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#ifndef WAVES_2_DSP_ENGINE_ENGINE_H
#define WAVES_2_DSP_ENGINE_ENGINE_H

#include "stmlib/utils/buffer_allocator.h"

namespace waves2 {

class Engine {
public:
    Engine() {}
    ~Engine() {}
    virtual void Init(stmlib::BufferAllocator* allocator) = 0;
    virtual void Paint() = 0;
};

template<int max_size>
class EngineRegistry {
 public:
  EngineRegistry() { }
  ~EngineRegistry() { }
  
  void Init() {
    num_engines_ = 0;
  }

  inline Engine* get(int index) {
    return engine_[index];
  }
  
  void RegisterInstance(Engine* instance) {
    if (num_engines_ >= max_size) {
      return;
    }
    engine_[num_engines_] = instance;
    ++num_engines_;
  }
  
  inline int size() const { return num_engines_; }

 private:
  Engine* engine_[max_size];
  int num_engines_;
};

}

#endif