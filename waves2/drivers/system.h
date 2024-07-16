
// System-level initialization.

#ifndef WAVES_2_DRIVERS_SYSTEM_H_
#define WAVES_2_DRIVERS_SYSTEM_H_

#include "stmlib/stmlib.h"
#include <stm32f4xx_conf.h>

namespace waves2 {

class System {
 public:
  System() { }
  ~System() { }
  
  void Init(bool application);
  void StartTimers();
 
 private:
  DISALLOW_COPY_AND_ASSIGN(System);
};

}  // namespace waves2

#endif  // WAVES_2_DRIVERS_SYSTEM_H_
