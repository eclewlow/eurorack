// Copyright 2017 Emilie Gillet.
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
// Lightweight DAC driver used for the firmware update procedure.
// Initializes the I2S port as SPI, and relies on a timer for clock generation.

#ifndef WAVES_DRIVERS_LCD_H_
#define WAVES_DRIVERS_LCD_H_

#include "stmlib/stmlib.h"
#include "waves/Globals.h"
// #include "waves/drivers/wavetables.h"
#include "stmlib/system/system_clock.h"

#include <stm32f4xx_conf.h>
#include <cstring>
#include <stdio.h>
#include "math.h"

namespace waves {

using namespace std;
using namespace stmlib;

extern uint8_t pic[];
extern uint8_t pic1[];

class LCD {
 public:
  LCD() { }
  ~LCD() { }
  
  inline void HIGH(GPIO_TypeDef* GPIOx, uint16_t pin) {
    // GPIO_SetBits(GPIOx, pin);
    GPIOx->BSRRL = pin;
  }

  inline void LOW(GPIO_TypeDef* GPIOx, uint16_t pin) {
    // GPIO_ResetBits(GPIOx, pin);
    GPIOx->BSRRH = pin;
  }

  inline uint8_t READ(GPIO_TypeDef* GPIOx, uint16_t pin) {
      uint8_t bitstatus = 0x00;

      if ((GPIOx->IDR & pin) != (uint32_t)Bit_RESET)
      {
        bitstatus = (uint8_t)Bit_SET;
      }
      else
      {
        bitstatus = (uint8_t)Bit_RESET;
      }
      return bitstatus;

    // return GPIO_ReadInputDataBit(GPIOx, pin);
  }

  static const uint16_t wait_time = 4;
  static const uint16_t wait_time_2 = 1;

  inline void HIGH(uint8_t index) {
    eeprom[index].gpio->BSRRL = eeprom[index].pin;
    if(index == EEPROM_CLOCK) {
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
    }
    // else
    //     Wait<wait_time_2>();
    // GPIO_SetBits(eeprom[index].gpio, eeprom[index].pin);
  }

  inline void LOW(uint8_t index) {
    eeprom[index].gpio->BSRRH = eeprom[index].pin;
    if(index == EEPROM_CLOCK) {
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
    }
    // else
    //     Wait<wait_time_2>();
    // GPIO_ResetBits(eeprom[index].gpio, eeprom[index].pin);
  }

  inline uint8_t READ(uint8_t index) {
    // return GPIO_ReadInputDataBit(eeprom[index].gpio, eeprom[index].pin);

      uint8_t bitstatus = 0x00;

      if ((eeprom[index].gpio->IDR & eeprom[index].pin) != (uint32_t)Bit_RESET)
      {
        bitstatus = (uint8_t)Bit_SET;
      }
      else
      {
        bitstatus = (uint8_t)Bit_RESET;
      }
    // if(index == EEPROM_CLOCK)
        // Wait<4>();
    // else
        // Wait<1>();
      return bitstatus;
  }
  
  // void Start(NextSampleFn next_sample_fn) {
  //   next_sample_fn_ = next_sample_fn;
  //   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  
  // }
  
  // void Stop() {
  //   TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);  
  // }


  template<int N>
  inline void Wait() {
    Wait<N-1>();
    __asm__("nop");
  }

    typedef union
      {
      uint8_t
        as_bytes[2];
      uint16_t
        as_word;
      } WORD_UNION;

// #define READ_STATUS_REGISTER            (0x05)
// #define ENABLE_WRITE_STATUS_REGISTER    (0x50)
// #define WRITE_STATUS_REGISTER           (0x01)
// #define WRITE_ENABLE                    (0x06)
// #define WRITE_DISABLE                   (0x04)
// #define ENABLE_BSY                      (0x70)
// #define DISABLE_BSY                     (0x80)
// #define JEDEC_ID_READ                   (0x9f)
// #define SECTOR_ERASE_4K                 (0x20)
// #define READ_25MHZ                      (0x03)
// #define READ_66MHZ                      (0x0B)
// #define AAI_WORD_PROGRAM                (0xad)
// #define BYTE_PROGRAM                    (0x02)


void CMD(uint8_t code, uint8_t pin) {

    LOW(pin);

    uint8_t buf[1] = { code };

    Write(buf, 1);

    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY));

    HIGH(pin);
}

void Write(uint8_t * buf, uint32_t size) {

    for(uint32_t i = 0; i < size; i++) {

      // int8_t j = 7;
      // while(j >= 0) {

      //   LOW(LCD_CLOCK);

      //   if((buf[i] >> j) & 0x1) {
      //     HIGH(LCD_MOSI);
      //   } else {
      //     LOW(LCD_MOSI);
      //   }

      //   __asm__("nop");
      //   __asm__("nop");
      //   __asm__("nop");
      //   __asm__("nop");
      //   __asm__("nop");

      //   HIGH(LCD_CLOCK);

      //   __asm__("nop");
      //   __asm__("nop");
      //   __asm__("nop");
      //   __asm__("nop");
      //   __asm__("nop");

      //   j--;
      // }
        SPI_I2S_SendData(SPI3, buf[i]);
        while (!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE));
        SPI_I2S_ReceiveData(SPI3);
        while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE));
        // while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE)) {
            // SPI_I2S_ReceiveData(SPI3);    
        // }
    }
}


inline void ReadFast(uint8_t * buf, uint32_t size) {
    for(uint32_t i = 0; i < size; i++) {
        SPI_I2S_SendData(SPI3, 0);
        while (!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE));
        buf[i] = SPI_I2S_ReceiveData(SPI3);
        while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE));
    }
}

inline void Read(uint8_t * buf, uint32_t size) {
    for(uint32_t i = 0; i < size; i++) {
        SPI_I2S_SendData(SPI3, 0);
        while (!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE));
        buf[i] = SPI_I2S_ReceiveData(SPI3);
        while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE));
    }
}

void Init();
// -------------------------------------------------------------  
void StartFrameDMARead(uint32_t * buffer, uint32_t size, uint32_t address);
void StartDMARead(uint16_t __bytes);
void StartDMAWrite();
void StopDMA();
void DisplayOn();
void DisplayOff();
void Draw();
void Initial();
void Write_Instruction(uint8_t byte);
void Display_Picture(uint8_t pic[]);
void Display(uint8_t pic[8][128]);
void Initial_Dispay_Line(uint8_t line);
void Set_Page_Address(uint8_t add);
void Set_Column_Address(uint8_t add);
void Write_Data(uint8_t * data, uint32_t size);

inline uint8_t get_page() { return page_; }
inline void increment_page() { page_++ ; }
inline void set_page(uint8_t page) { page_ = page; }
inline void swap_picture() {  swap = !swap; Display_Picture(swap ? pic1 : pic); }

static LCD* GetInstance() { return instance_; }
 private:
  uint8_t page_;
  bool swap;
  static LCD* instance_;
  // NextSampleFn next_sample_fn_;
  // static FirmwareUpdateDac* instance_;
  
  DISALLOW_COPY_AND_ASSIGN(LCD);
};
template<> inline void LCD::Wait<0>() { }

}  // namespace stages

#endif  // WAVES_DRIVERS_LCD_H_
