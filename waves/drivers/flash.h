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

#ifndef WAVES_DRIVERS_FLASH_H_
#define WAVES_DRIVERS_FLASH_H_

#include "stmlib/stmlib.h"
#include "waves/drivers/globals.h"
#include "waves/drivers/wavetables.h"
#include "stmlib/system/system_clock.h"

#include <stm32f4xx_conf.h>
#include <cstring>
#include <stdio.h>
#include "math.h"

namespace waves {

using namespace std;
using namespace stmlib;

#define WriteEnable             0x06
#define WtiteDisable            0x04
#define Read_JedecID            0x9f
#define Read_UniqueID           0x4b
#define Read_DeviceID           0xab
#define Read_StatusRegister_1   0x05
#define Read_StatusRegister_2   0x35
#define Read_StatusRegister_3   0x15
#define Write_StatusRegister    0x01
#define Write_StatusRegister_1  0x01
#define Write_StatusRegister_2  0x31
#define Write_StatusRegister_3  0x11
#define PageProgram             0x02
#define ReadData                0x03
#define FastRead                0x0b
#define Erase_Chip              0xc7
#define Erase_Block_64          0xd8
#define Erase_Block_32          0x52
#define Erase_Sector            0x20
#define Erase_Suspend           0x75
// #define Erase_Resume            0x7h
#define PowerDown               0xb9
#define PowerDown_Release       0xab

// #define _BUSY_                  0x01 // Erase or write in progress
// #define _WEL_                   0x02 // Write enable Latch
// #define _BP0_                   0x04 // Block protect bit 0 (non-volatile)
// #define _BP1_                   0x08 // Block protect bit 1 (non-volatile)
// #define _BP2_                   0x10 // Block protect bit 2 (non-volatile)
// #define _TB_                    0x20 // Top/Bottom write protect (non-volatile)
// #define _SEC_                   0x40 // Sector protect (non-volatile)
// #define _SRP0_                  0x80 // Status register protect bit 0 (non-volatile)

// #define _SRP1_                  0x01 // Status register protect bit 1 (non-volatile)
// #define _SREQ_                  0x02 // Quad enable

typedef struct
{
  uint32_t  ID;
  uint32_t  UniqID[2];
  uint16_t  BlockCount;
    uint32_t  Capacity;
  uint8_t   Lock;   
} w25qxx_t;

const uint16_t w25q[10] = { 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400 };
const uint16_t PageSize = 0x0100; // 256
const uint16_t SectorSize = 0x1000; // 4096
const uint16_t Block32KSize = 0x8000; // 32768
const uint32_t BlockSize = 0x00010000; // 65536


class Flash {
 public:
  Flash() { }
  ~Flash() { }
  
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


  void Init();
  
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

      typedef struct {
        char name[9];
        uint32_t memory_location;
        bool factory_preset;
        bool is_empty;
    } WAVE;
    
    typedef struct {
        char name[9];
        WAVE waves[16];
        bool factory_preset;
        bool is_empty;
    } WAVETABLE;
    
    typedef struct {
        char name[9];
        bool factory_preset;
        bool is_empty;

        // misc settings
        int8_t brightness;
        int8_t contrast;
        bool invert;
        int8_t scope_setting;                  // (0, 1)
        int8_t morph_setting;                  // (0, 1)

        // sub osc parameters
        int8_t subosc_offset;                    // (-24, 24)
        int8_t subosc_detune;                    // (-50, 50)
        int8_t subosc_mix;                       // (0, 100)
        int8_t subosc_wave;                      // (0, 5)
        
        // fx parameters
        float fx_depth;                        // (0.0f, 1.0f)
        bool fx_sync;                           // (false, true)
        int8_t fx_scale;                        // (0, 100)
        int8_t fx_range;                        // (1, 10)
        int8_t fx_oscillator_shape;             // (0, 5)
        int8_t fx_control_type;                 // (0, 2)
        int8_t fx_effect;                       // (0, 8)

        int8_t engine;                          // (0, 3)
        // ab engine parameters
        int8_t ab_engine_left_wavetable;        // (0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1)
        int8_t ab_engine_left_frame;            // (0, 15)
        int8_t ab_engine_right_wavetable;       // (0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1)
        int8_t ab_engine_right_frame;           // (0, 15)
        bool ab_engine_is_editing_left;
        bool ab_engine_is_editing_right;
//        bool ab_engine_is_editing; set to false for both left and right

        // wavetable engine parameters
        int8_t wavetable_engine_wavetable;      // (0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1)

        // matrix engine parameters
        int8_t matrix_engine_x1;                // (0, 15)
        int8_t matrix_engine_x2;                // (x1, 15)
        int8_t matrix_engine_y1;                // (0, 15)
        int8_t matrix_engine_y2;                // (y1, 15)
        int8_t matrix_engine_wavelist_offset;   // (0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 16 - 1)

        // drum engine parameters
        float drum_engine_amp_decay;            // (0.0, 1.0)
        float drum_engine_fm_decay;             // (0.0, 1.0)
        float drum_engine_fm_shape;             // (0.0, 1.0)
        float drum_engine_fm_depth;             // (0.0, 1.0)
        int8_t drum_engine_wavetable;           // (0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1)

        // pot positions
        uint16_t pot_fx_amount;                 // (0, 4095)
        uint16_t pot_fx;                        // (0, 4095)
        uint16_t pot_morph;                     // (0, 4095)
        
        // calibration data
        float io_gain[4];                      // (1, 10.0)    // don't randomize this, but save in snapshot
        float io_bias[4];                      // (-1.0, 1.0)  // don't randomize this, but save in snapshot

        float calibration_x;                   // ()  // don't randomize this, but save in snapshot
        float calibration_y;                   // ()  // don't randomize this, but save in snapshot
    } SNAPSHOT;

      typedef struct {
        WAVETABLE wavetables[USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT];
        SNAPSHOT snapshots[USER_SNAPSHOT_COUNT + FACTORY_SNAPSHOT_COUNT];
    } PERSISTENT_STORAGE;


    typedef union
      {
      uint8_t
        as_bytes[2];
      uint16_t
        as_word;
      } WORD_UNION;


    WAVETABLE GetWavetable(uint8_t table) {
      // read memory at sector table * 4096.
      // read 4096 bytes into buffer
      uint16_t size = sizeof(WAVETABLE);
      uint8_t buffer[sizeof(WAVETABLE)];
      uint8_t * data = &buffer[0];
      uint32_t address = table * 4096;

      LOW(EEPROM_PERSISTENT_SS);

      SPI_I2S_SendData(SPI1, 0x03); // Read Instruction

      while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
      SPI_I2S_SendData(SPI1, (address >> 16) & 0xFF); // Send Address

      while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
      SPI_I2S_SendData(SPI1, (address >> 8) & 0xFF); // Send Address

      while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
      SPI_I2S_SendData(SPI1, (address) & 0xFF); // Send Address


    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

      while (size)
      {
          // while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
          while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
          SPI_I2S_SendData(SPI1, 0x00);
          // SPI1->DR = 0;  // send dummy data
          while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
          // while (!((SPI1->SR) &(1<<0))){};  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
          *data++ = SPI_I2S_ReceiveData(SPI1);
          size--;
      }

      HIGH(EEPROM_PERSISTENT_SS);

      WAVETABLE wt;

      memcpy(&wt, buffer, sizeof(WAVETABLE));

      return wt;
    }

    // inline WAVETABLE* GetWavetables() { return persistent_storage_.wavetables; }
    // inline SNAPSHOT* GetSnapshot(int8_t snapshot) { return &persistent_storage_.snapshots[snapshot]; }

  int16_t LoadWaveSample(uint8_t table, uint8_t frame, uint16_t index) {
    int16_t value = 0;
  
    // TODO: this shouldn't have to look up from persistent storage.

    // WAVETABLE wt = GetWavetable(table);
    
    // read 2 bytes from memory location + offset
    uint8_t size = 2;
    uint8_t buffer[2];
    uint8_t * data = &buffer[0];

    uint32_t address = 2048 * 16 * 2 * (table % FACTORY_WAVETABLE_COUNT) + 2048 * 2 * frame + index * 2;

    LOW(table < FACTORY_WAVETABLE_COUNT ? EEPROM_FACTORY_SS : EEPROM_USER_SS );

    SPI_I2S_SendData(SPI1, 0x0B); // Read Instruction High speed 0x0B.  normal speed is 0x03

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address >> 16) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address >> 8) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, 0x00);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

    while (size)
    {
        // while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
        SPI_I2S_SendData(SPI1, 0x00);
        // SPI1->DR = 0;  // send dummy data
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
        // while (!((SPI1->SR) &(1<<0))){};  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
        *data++ = SPI_I2S_ReceiveData(SPI1);
        size--;
    }

    HIGH(table < FACTORY_WAVETABLE_COUNT ? EEPROM_FACTORY_SS : EEPROM_USER_SS );

    memcpy(&value, buffer, 2);
    // value = (buffer[0] << 8) | buffer[1];
    return value;

  }

  void LoadWaveSample(int16_t * wavedata, uint8_t table, uint8_t frame, int16_t index) {
    // TODO: this shouldn't have to look up from persistent storage.

    // read 2 bytes from memory location + offset
    uint16_t size = 2048 * 2;
    uint8_t buffer[2048 * 2];
    uint8_t * data = &buffer[0];
    // uint32_t address = wt.waves[frame].memory_location;
    // uint8_t pin = wt.waves[frame].factory_preset ? kPinFactorySS : kPinUserSS;

    uint32_t address = 2048 * 16 * 2 * (table % FACTORY_WAVETABLE_COUNT) + 2048 * 2 * frame + index * 2;

    LOW(table < FACTORY_WAVETABLE_COUNT ? EEPROM_FACTORY_SS : EEPROM_USER_SS );

    SPI_I2S_SendData(SPI1, 0x0B); // Read Instruction High speed 0x0B.  normal speed is 0x03

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address >> 16) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address >> 8) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, 0x00);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

    while (size)
    {
        // while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
        SPI_I2S_SendData(SPI1, 0x00);
        // SPI1->DR = 0;  // send dummy data
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
        // while (!((SPI1->SR) &(1<<0))){};  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
        *data++ = SPI_I2S_ReceiveData(SPI1);
        size--;
    }

    HIGH(table < FACTORY_WAVETABLE_COUNT ? EEPROM_FACTORY_SS : EEPROM_USER_SS );

    // const int16_t * data = &ROM[t->waves[frame].memory_location];
    memcpy(wavedata, data, 2048 * 2);
}

bool ResetFactoryWavetables() {
    char *names[16] = {
        (char*)"SQUISH",
        (char*)"HARMONIX",
        (char*)"CLICKY",
        (char*)"PHASER",
        (char*)"TOILET",
        (char*)"SOLAR",
        (char*)"JUICE",
        (char*)"SQROWL",
        (char*)"GRANULAR",
        (char*)"H SERIES",
        (char*)"FOLD",
        (char*)"SOLRPOWR",
        (char*)"FLANGE",
        (char*)"NOISE",
        (char*)"VITAL 1",
        (char*)"VITAL 2",
    };


    for(uint8_t table = 0; table < FACTORY_WAVETABLE_COUNT; table++) {
        WAVETABLE wt = GetWavetable(table);
        strncpy(wt.name, names[table], 9);
        wt.factory_preset = true;
        wt.is_empty = false;
        for(uint8_t frame = 0; frame < 16; frame++) {
            wt.waves[frame].memory_location = table * 16 * 2048 * 2 + frame * 2048 * 2;
//            strncpy(GetWavetable(table)->waves[frame].name, names[frame], 9);
            snprintf(wt.waves[frame].name, 9, "%02d", frame);
            wt.waves[frame].factory_preset = true;
            wt.waves[frame].is_empty = false;
        }

        // SAVE THE WAVETABLE 
        SaveWavetable(wt, table);
    }
    


    return true;
}

#define READ_STATUS_REGISTER            (0x05)
#define ENABLE_WRITE_STATUS_REGISTER    (0x50)
#define WRITE_STATUS_REGISTER           (0x01)
#define WRITE_ENABLE                    (0x06)
#define WRITE_DISABLE                   (0x04)
#define ENABLE_BSY                      (0x70)
#define DISABLE_BSY                     (0x80)
#define JEDEC_ID_READ                   (0x9f)
#define SECTOR_ERASE_4K                 (0x20)
#define READ_25MHZ                      (0x03)
#define READ_66MHZ                      (0x0B)
#define AAI_WORD_PROGRAM                (0xad)
#define BYTE_PROGRAM                    (0x02)


bool SaveWavetable(WAVETABLE wt, uint8_t table) {
    // write wavetable to sector
    uint8_t buffer[sizeof(WAVETABLE)];
    uint16_t size = sizeof(WAVETABLE);

    memcpy(buffer, &wt, sizeof(WAVETABLE));
    
    uint32_t address = table * 4096;

    uint8_t pin = EEPROM_PERSISTENT_SS;

    SectorErase4K(address, pin);

    AAI_Word_Program(buffer, size, address, pin);

    return true;
}

bool InitMemory() {
    WriteStatusRegister(0, EEPROM_FACTORY_SS);
    for(uint8_t table = 0; table < 1; table++) {
        for(uint8_t frame = 0; frame < 16; frame++) {
              uint32_t address = table * 2048 * 16 * 2 + frame * 2048 * 2;
              SectorErase4K(address, EEPROM_FACTORY_SS);
              // AAI_Word_Program((uint8_t *)&ROM[table * 2048 * 16 + frame * 2048], 2048 * 2, address, EEPROM_FACTORY_SS);
        }

    }

    // AAI_Word_Program((uint8_t *)&ROM[0 * 2048 * 16 + 0 * 2048], 2048 * 2, 0, EEPROM_FACTORY_SS);
  return true;
}


// -------------------------------------------------------------  
void W25Qxx_TransferDMASPI (uint32_t __bytes, DataDirectionTypeDef __dir, uint32_t __offset){
  // SetFlag(&_EREG_, _DBLF_, FLAG_SET);
 
  uint8_t pump = 0;
  if (__dir == DIR_WRITE)
  {
    DMA_SetCurrDataCounter(DMA2_Stream3, __bytes);
    DMA2_Stream3->M0AR = ((int)&dataBuffer) + (__offset * 256);
    DMA_Cmd(DMA2_Stream3, ENABLE);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
  }
  else
  {
    DMA_SetCurrDataCounter(DMA2_Stream3, __bytes);
    DMA_SetCurrDataCounter(DMA2_Stream0, __bytes);
    DMA2_Stream3->M0AR = (int)&pump;
    DMA2_Stream0->M0AR = (int)&dataBuffer;
    DMA_Cmd(DMA2_Stream3, ENABLE);
    DMA_Cmd(DMA2_Stream0, ENABLE);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
  }

  while (!DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3));
  DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3);

  while (!DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0));
  DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0);
  // Wait<500>();
  
  if (__dir == DIR_WRITE)
  {
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    DMA_Cmd(DMA2_Stream3, DISABLE);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
  }
  else
  {
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
    DMA_Cmd(DMA2_Stream3, DISABLE);
    DMA_Cmd(DMA2_Stream0, DISABLE);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, DISABLE);
  }

  // SetFlag(&_EREG_, _DBLF_, FLAG_SET);
}




// -------------------------------------------------------------  
bool W25Qxx_TransferSPI (uint8_t __command, int32_t __address, uint16_t __bytes, DataDirectionTypeDef __dir, uint32_t __offset)
{
  GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
  
  // SPI_Cmd(SPI1, ENABLE);

  SPI_I2S_SendData(SPI1, __command);
  while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
  SPI_I2S_ReceiveData(SPI1);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));

  if (__address >= 0)
  {
    int i = 4;
    while (--i)
    {
      SPI_I2S_SendData(SPI1, __address >> 8 * i);
      while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
      SPI_I2S_ReceiveData(SPI1);
      while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
    }
  }
  
  if (__command == FastRead)
  {
    SPI_I2S_SendData(SPI1, 0);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
  }

  if(__command==Read_JedecID) {
    // SPI_I2S_SendData(SPI1, 0);
    // while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    // SPI_I2S_ReceiveData(SPI1);
    // while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));    
  }

  if (__command == 0xab)
  {
    SPI_I2S_SendData(SPI1, 0);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));

    SPI_I2S_SendData(SPI1, 0);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));

    SPI_I2S_SendData(SPI1, 0);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));

    SPI_I2S_SendData(SPI1, 0);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
  }
  
    if (__command == WRITE_STATUS_REGISTER)
  {
    SPI_I2S_SendData(SPI1, 0x0c);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));

    // SPI_I2S_SendData(SPI1, 1);
    // while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    // SPI_I2S_ReceiveData(SPI1);
    // while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
  }


    if (__command == READ_STATUS_REGISTER)
  {
    SPI_I2S_SendData(SPI1, 0);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
  }

  if (__bytes)
  {
    W25Qxx_TransferDMASPI(__bytes, __dir, __offset);
  }

  // SPI_Cmd(SPI1, ENABLE);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));
    
  GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);

  return true;
}

bool ReadWaveframe(int32_t __address, uint8_t pin) {

    LOW(pin);

    CMD(READ_66MHZ, EEPROM_FACTORY_SS);

    if (__address >= 0)
    {
        int i = 4;
        while (--i)
        {
            SPI_I2S_SendData(SPI1, __address >> 8 * i);
            while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
            SPI_I2S_ReceiveData(SPI1);
            while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
        }
    }

    SPI_I2S_SendData(SPI1, 0);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));    

    StartDMARead(4096);

    return true;
}

void CMD(uint8_t code, uint8_t pin) {

    LOW(pin);

    uint8_t buf[1] = { code };

    Write(buf, 1);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));

    HIGH(pin);
}

inline void Write(uint8_t * buf, uint32_t size) {
    for(uint32_t i = 0; i < size; i++) {
        SPI_I2S_SendData(SPI1, buf[i]);
        while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
        SPI_I2S_ReceiveData(SPI1);
        // while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
          while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)) {
            SPI_I2S_ReceiveData(SPI1);    
          }
    }
}


inline void ReadFast(uint8_t * buf, uint32_t size) {
    for(uint32_t i = 0; i < size; i++) {
        SPI_I2S_SendData(SPI1, 0);
        while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
        buf[i] = SPI_I2S_ReceiveData(SPI1);
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
    }
}

inline void Read(uint8_t * buf, uint32_t size) {
    for(uint32_t i = 0; i < size; i++) {
        SPI_I2S_SendData(SPI1, 0);
        while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
        buf[i] = SPI_I2S_ReceiveData(SPI1);
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
    }
}

uint32_t Jedec_ID_Read() {

    LOW(EEPROM_FACTORY_SS);

    uint8_t buf[1] = { JEDEC_ID_READ };

    Write(buf, 1);

    uint32_t result = 0;

    Read((uint8_t*)&result, 4);

    HIGH(EEPROM_FACTORY_SS);

    return result;
}

bool WriteStatusRegister(uint8_t byte, uint8_t pin) {
    CMD(WRITE_ENABLE, pin);

    CMD(ENABLE_WRITE_STATUS_REGISTER, pin);

    LOW(pin);

    uint8_t buf[2] = { WRITE_STATUS_REGISTER, byte };

    Write(buf, 2);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));

    HIGH(pin);

    CMD(WRITE_DISABLE, pin);

    return true;
}

uint8_t ReadStatusRegister(uint8_t pin) {

    LOW(pin);

    uint8_t buf[1] = { READ_STATUS_REGISTER };

    Write(buf, 1);

    SPI_I2S_SendData(SPI1, 0);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));

    uint8_t byte = 0;

    Read(&byte, 1);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));

    HIGH(pin);

    return byte;
}

bool SectorErase4K(uint32_t address, uint8_t pin) {

    // SET WREN first
    CMD(WRITE_ENABLE, pin);

    LOW(pin);

    uint8_t buf[4];

    buf[0] = SECTOR_ERASE_4K;
    buf[1] = ((address >> 16) & 0xFF);
    buf[2] = ((address >> 8) & 0xFF);
    buf[3] = ((address) & 0xFF);

    Write(buf, 4);

    // SPI_I2S_SendData(SPI1, 0);
    // while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    // SPI_I2S_ReceiveData(SPI1);
    // while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));

    HIGH(pin);

    // while(ReadStatusRegister(pin) & 0x01);  // BUSY
    system_clock.Delay(25);


    CMD(WRITE_DISABLE, pin);

    return true;
}

void Read25Mhz(uint8_t * buf, uint32_t size, uint32_t address, uint8_t pin) {

    LOW(pin);

    uint8_t send_buf[4];

    send_buf[0] = READ_25MHZ;
    send_buf[1] = ((address >> 16) & 0xFF);
    send_buf[2] = ((address >> 8) & 0xFF);
    send_buf[3] = ((address) & 0xFF);
    // memcpy(send_buf, &address, 4);

    send_buf[0] = READ_25MHZ;

    Write(send_buf, 4);

    SPI_I2S_SendData(SPI1, 0);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));

    Read(buf, size);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));

    HIGH(pin);
}

void Read66Mhz(uint8_t * buf, uint32_t size, uint32_t address, uint8_t pin) {

    LOW(pin);

    uint8_t send_buf[5];
    send_buf[0] = READ_66MHZ;
    send_buf[1] = ((address >> 16) & 0xFF);
    send_buf[2] = ((address >> 8) & 0xFF);
    send_buf[3] = ((address) & 0xFF);
    send_buf[4] = 0x00;

    Write(send_buf, 5);

    ReadFast(buf, size);

    HIGH(pin);
}

void Read66MhzDMA(uint32_t size, uint32_t address) {

    LOW(EEPROM_FACTORY_SS);

    uint8_t send_buf[5];
    send_buf[0] = READ_66MHZ;
    send_buf[1] = ((address >> 16) & 0xFF);
    send_buf[2] = ((address >> 8) & 0xFF);
    send_buf[3] = ((address) & 0xFF);
    send_buf[4] = 0x00;

    Write(send_buf, 5);

    SPI_I2S_SendData(SPI1, 0);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));

    StartDMARead(size);
}

void Read25MhzDMA(uint32_t size, uint32_t address) {

    LOW(EEPROM_FACTORY_SS);

    uint8_t send_buf[4];
    send_buf[0] = READ_25MHZ;
    send_buf[1] = ((address >> 16) & 0xFF);
    send_buf[2] = ((address >> 8) & 0xFF);
    send_buf[3] = ((address) & 0xFF);

    Write(send_buf, 4);

    SPI_I2S_SendData(SPI1, 0);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));

    StartDMARead(size);
}

bool Program(uint8_t byte, uint32_t address, uint8_t pin) {
    // wren
    CMD(WRITE_ENABLE, pin);

    LOW(pin);

    uint8_t buf[5];
    buf[0] = BYTE_PROGRAM;
    buf[1] = ((address >> 16) & 0xFF);
    buf[2] = ((address >> 8) & 0xFF);
    buf[3] = ((address) & 0xFF);
    buf[4] = byte;

    Write(buf, 5);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));

    HIGH(pin);

    // while(ReadStatusRegister(pin) & 0x01);  // BUSY
    system_clock.Delay(25);

    // wrdi
    CMD(WRITE_DISABLE, pin);

    return true;

}

bool AAI_Word_Program(uint8_t * buffer, uint32_t size, uint32_t address, uint8_t pin) {
    if(size < 2) return false;
    if(size % 2 != 0) return false;

    // TODO: USE HARDWARE BSY FOR FASTER WRITE
    // wren
    CMD(WRITE_ENABLE, pin);

    // ebsy
    // CMD(ENABLE_BSY, pin);
    CMD(DISABLE_BSY, pin);

    LOW(pin);

    uint32_t i = 0;

    uint8_t send_buf[6];
    send_buf[0] = AAI_WORD_PROGRAM;
    send_buf[1] = ((address >> 16) & 0xFF);
    send_buf[2] = ((address >> 8) & 0xFF);
    send_buf[3] = ((address) & 0xFF);
    send_buf[4] = buffer[i++];
    send_buf[5] = buffer[i++];

    Write(send_buf, 6);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));

    HIGH(pin);

    // while(ReadStatusRegister(pin) & 0x01);  // BUSY
    system_clock.Delay(10);

    while(i < size) {

        LOW(pin);

        uint8_t send_buf2[3] = {
            AAI_WORD_PROGRAM,
            buffer[i++],
            buffer[i++],
        };

        Write(send_buf2, 3);

        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));

        HIGH(pin);

        // while(ReadStatusRegister(pin) & 0x01);  // BUSY
        system_clock.Delay(1);
    }

    // wrdi
    CMD(WRITE_DISABLE, pin);

    // dbsy
    // CMD(DISABLE_BSY, pin);

    return true;
}

// -------------------------------------------------------------  
void W25qxx_Init (void)
{
    // system_clock.Delay(1000);

    w25qxx.ID = __REV(Jedec_ID_Read());

    WriteStatusRegister(0x00, EEPROM_FACTORY_SS);

    w25qxx.Lock = ReadStatusRegister(EEPROM_FACTORY_SS);

    // Program(0xef, 0, EEPROM_FACTORY_SS);
    // Program(0xaa, 1, EEPROM_FACTORY_SS);

    while(GetFlag(&_EREG_, _BUSY_));
    SetFlag(&_EREG_, _BUSY_, FLAG_SET);
    for(int i = 0; i < 16; i++) {
    // loading++;
        // system_clock.Delay(10);
    // int i = 0;
    // SectorErase4K(4096*i, EEPROM_FACTORY_SS);
    // system_clock.Delay(i == 0 ? 25 : 25);
    // AAI_Word_Program((uint8_t *)&ROM[2048*i], 4096, 4096*i, EEPROM_FACTORY_SS);
    }


    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
    // Program(0x1, 0, EEPROM_FACTORY_SS);
    // system_clock.Delay(100);
    // Program(0x1, 1, EEPROM_FACTORY_SS);
    // system_clock.Delay(100);
    // SectorErase4K(0, EEPROM_FACTORY_SS);
    // system_clock.Delay(100);
    // AAI_Word_Program((uint8_t *)ROM, 4096, 0, EEPROM_FACTORY_SS);


    // system_clock.Delay(100);
    // SectorErase4K(4096, EEPROM_FACTORY_SS);
    // system_clock.Delay(100);
    // AAI_Word_Program((uint8_t *)&ROM[2048], 4096, 4096, EEPROM_FACTORY_SS);
    // system_clock.Delay(100);

    // if(!GetFlag(&_EREG_, _BUSY_)) {
        // Read66MhzDMA(4096, 0);
    // }
    // front_buffer = double_waveframe_buffer_1;
    // back_buffer = double_waveframe_buffer_2;

    // SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    // StartFrameDMARead((uint32_t*)front_buffer, 8192, 0);

    // SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
    // SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    // if(!GetFlag(&_EREG_, _BUSY_)) {
        // StartFrameDMARead((uint32_t*)double_waveframe_buffer_1, 8192, 4096 * 0);
    // }
    // uint32_t buf;
    // Read25Mhz((uint8_t*)&buf, 4, 2, EEPROM_FACTORY_SS);
    // int16_t * test = (int16_t*)&buf;
    // dataBuffer[0] = test[1];
    // memset(dataBuffer, 0, 4);
    // memcpy(dataBuffer, &buf, 2);
  // W25Qxx_TransferSPI(WRITE_ENABLE, -1, 0, DIR_NEUTRAL, 0);

  // W25Qxx_TransferSPI(ENABLE_WRITE_STATUS_REGISTER, -1, 0, DIR_NEUTRAL, 0);

  // W25Qxx_TransferSPI(WRITE_STATUS_REGISTER, -1, 0, DIR_NEUTRAL, 0);

  // W25Qxx_TransferSPI(WRITE_DISABLE, -1, 0, DIR_NEUTRAL, 0);

  // W25Qxx_TransferSPI(READ_STATUS_REGISTER, -1, 4, DIR_READ, 0);
  // w25qxx.Capacity = __REV(dataBuffer[0]);

    // w25qxx.ID = ReadStatusRegister(EEPROM_FACTORY_SS);

    // while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));

    // ReadWaveframe(0, EEPROM_FACTORY_SS);

  // W25Qxx_TransferSPI(0xab, -1, 4, DIR_READ, 0);
  // w25qxx.ID = __REV(dataBuffer[0]);

  // W25Qxx_TransferSPI(Read_JedecID, -1, 4, DIR_READ, 0);
  // w25qxx.ID = __REV(dataBuffer[0]);

  // W25Qxx_TransferSPI(WRITE_ENABLE, -1, 0, DIR_NEUTRAL, 0);

  // W25Qxx_TransferSPI(ENABLE_WRITE_STATUS_REGISTER, -1, 0, DIR_NEUTRAL, 0);

  // W25Qxx_TransferSPI(WRITE_STATUS_REGISTER, -1, 0, DIR_NEUTRAL, 0);

  // W25Qxx_TransferSPI(WRITE_DISABLE, -1, 0, DIR_NEUTRAL, 0);

  // W25Qxx_TransferSPI(READ_STATUS_REGISTER, -1, 4, DIR_READ, 0);
  // w25qxx.ID = __REV(dataBuffer[0]);

  // W25Qxx_TransferSPI(Read_JedecID, -1, 0, DIR_WRITE, 0);
  // w25qxx.ID = __REV(dataBuffer[0]);


  // W25Qxx_TransferSPI(Read_JedecID, -1, 4, DIR_READ, 0);
  // w25qxx.ID = __REV(dataBuffer[0]);
  // W25Qxx_TransferSPI(Read_JedecID, -1, 4, DIR_READ, 0);
  // w25qxx.ID = __REV(dataBuffer[0]);

  
  // W25Qxx_TransferSPI(Read_UniqueID, -1, 12, DIR_READ, 0);
  // w25qxx.UniqID[0] = __REV(dataBuffer[1]);
  // w25qxx.UniqID[1] = __REV(dataBuffer[2]);

  // W25Qxx_TransferSPI(Read_DeviceID, -1, 4, DIR_READ, 0);
  // w25qxx.BlockCount = w25q[(__REV(dataBuffer[0]) & 0x0f)];  
  // w25qxx.Capacity = w25qxx.BlockCount * BlockSize;
  
  // SetFlag(&_EREG_, _DBLF_, FLAG_CLEAR);
}
    w25qxx_t    w25qxx;
void StartFrameDMARead(uint32_t * buffer, uint32_t size, uint32_t address);
void StartDMARead(uint16_t __bytes);
void StartDMAWrite(uint16_t __bytes);
void StopDMA();

static Flash* GetInstance() { return instance_; }

 private:

  static Flash* instance_;
  // NextSampleFn next_sample_fn_;
  // static FirmwareUpdateDac* instance_;
  
  DISALLOW_COPY_AND_ASSIGN(Flash);
};
template<> inline void Flash::Wait<0>() { }

}  // namespace stages

#endif  // WAVES_DRIVERS_FLASH_H_
