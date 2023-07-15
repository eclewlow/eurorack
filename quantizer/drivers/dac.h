// Copyright 2012 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Driver for DAC.

#ifndef QUANTIZER_DRIVERS_DAC_H_
#define QUANTIZER_DRIVERS_DAC_H_

#include <stm32f10x_conf.h>
#include "stmlib/stmlib.h"

namespace quantizer {

  class Dac {
  public:
    Dac() { }
    ~Dac() { }

    void Init();

    template<int N>
    inline void Wait() {
      Wait<N-1>();
      __asm__("nop");
    }

    void Write(uint16_t value, uint8_t channel) {
      channel = channel & 0x01;
      value = value & 0x0fff;
      value = 4095 - value; // flip 0...2.5 to 2.5...0 because of inverted opamp

      GPIOB->BSRR = GPIO_Pin_1;
      // GPIOB->BRR = GPIO_Pin_6;
      GPIOA->BRR = GPIO_Pin_4;

      while (!((SPI1->SR)&(1<<1))) {};

      // SPI1->DR = (channel << 15) | value | (0x3000);
      SPI1->DR = value | (0x3000);
      // Wait<64>();

      // GPIO_SetBits(GPIOB, kPinSS);
      // GPIO_ResetBits(GPIOB, kPinSS);
      // SPI_I2S_SendData(SPI1, 0x3000 | (value >> 8));
      // SPI_I2S_SendData(SPI1, value << 8);


      while (!((SPI1->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
      while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication 
  
      //  Clear the Overrun flag by reading DR and SR
      uint8_t temp = SPI1->DR;
      temp = SPI1->SR;
      temp++;

      // GPIOB->BSRR = GPIO_Pin_6;
      GPIOA->BSRR = GPIO_Pin_4;
      GPIOB->BRR = GPIO_Pin_1;
   }

 private:

  DISALLOW_COPY_AND_ASSIGN(Dac);
};

template<> inline void Dac::Wait<0>() { }


}  // namespace quantizer

#endif  // QUANTIZER_DRIVERS_DAC_H_
