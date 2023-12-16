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
#include <stm32f4xx_conf.h>
#include <cstring>
#include <stdio.h>

namespace waves {

using namespace std;
using namespace stmlib;

const uint16_t kPinFactorySS = GPIO_Pin_4;
const uint16_t kPinUserSS = GPIO_Pin_3;
const uint16_t kPinPersistentSS = GPIO_Pin_2;

class Flash {
 public:
  Flash() { }
  ~Flash() { }
  
  void HIGH(GPIO_TypeDef* GPIOx, uint16_t pin) {
    GPIO_SetBits(GPIOx, pin);
  }

  void LOW(GPIO_TypeDef* GPIOx, uint16_t pin) {
    GPIO_ResetBits(GPIOx, pin);
  }

  uint8_t READ(GPIO_TypeDef* GPIOx, uint16_t pin) {
    return GPIO_ReadInputDataBit(GPIOx, pin);
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

      GPIO_ResetBits(GPIOA, kPinPersistentSS);

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

      GPIO_SetBits(GPIOA, kPinPersistentSS);

      WAVETABLE wt;

      memcpy(&wt, buffer, sizeof(WAVETABLE));

      return wt;
    }

    // inline WAVETABLE* GetWavetables() { return persistent_storage_.wavetables; }
    // inline SNAPSHOT* GetSnapshot(int8_t snapshot) { return &persistent_storage_.snapshots[snapshot]; }

  int16_t LoadWaveSample(uint8_t table, uint8_t frame, uint16_t index) {
    int16_t value = 0;
  
    // WAVETABLE wt = GetWavetable(table);
    
    // read 2 bytes from memory location + offset
    uint8_t size = 2;
    uint8_t buffer[2];
    uint8_t * data = &buffer[0];
    // uint32_t address = wt.waves[frame].memory_location + index * 2;
    // uint8_t pin = wt.waves[frame].factory_preset ? kPinFactorySS : kPinUserSS;

    uint32_t address = 2048 * 16 * 2 * table + 2048 * 2 * frame + index * 2;
    uint8_t pin = kPinFactorySS;

    GPIO_ResetBits(GPIOA, pin);

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

    GPIO_SetBits(GPIOA, pin);

    memcpy(&value, buffer, 2);
    // value = (buffer[0] << 8) | buffer[1];
    return value;

  }

  void LoadWaveSample(int16_t * wavedata, uint8_t table, uint8_t frame, int16_t index) {
    WAVETABLE wt = GetWavetable(table);
    
    // read 2 bytes from memory location + offset
    uint16_t size = 2048 * 2;
    uint8_t buffer[2048 * 2];
    uint8_t * data = &buffer[0];
    uint32_t address = wt.waves[frame].memory_location;
    uint8_t pin = wt.waves[frame].factory_preset ? kPinFactorySS : kPinUserSS;

    GPIO_ResetBits(GPIOA, pin);

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

    GPIO_SetBits(GPIOA, pin);

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

#define READ_STATUS_REGISTER          (0x05)
#define ENABLE_WRITE_STATUS_REGISTER  (0x50)
#define WRITE_STATUS_REGISTER         (0x01)
#define WRITE_ENABLE                  (0x06)
#define WRITE_DISABLE                 (0x04)
#define ENABLE_BSY                    (0x70)
#define DISABLE_BSY                   (0x80)


void CMD(uint8_t code, uint8_t pin) {
 //clock low
    LOW(eeprom_clock_gpio, eeprom_clock_pin);

    HIGH(eeprom_ss_gpio, eeprom_ss_pin);

    Wait<2>();

    LOW(eeprom_ss_gpio, eeprom_ss_pin);

    uint8_t size = 8;

    while(size) {

        //clock low
        LOW(eeprom_clock_gpio, eeprom_clock_pin);
    
        Wait<2>();

        bool set = (code >> (size - 1)) & 0x1;
        if(set)
            HIGH(eeprom_mosi_gpio, eeprom_mosi_pin);
        else
            LOW(eeprom_mosi_gpio, eeprom_mosi_pin);

        //clock high
        HIGH(eeprom_clock_gpio, eeprom_clock_pin);

        Wait<2>();

        size--;
    }

    HIGH(eeprom_ss_gpio, eeprom_ss_pin);

    //clock low
    LOW(eeprom_clock_gpio, eeprom_clock_pin);
}



bool Sector_Erase4K(uint32_t address, uint8_t pin) {

    // SET WREN first
    CMD(WRITE_ENABLE, pin);

    GPIO_ResetBits(GPIOA, pin);

    SPI_I2S_SendData(SPI1, 0x20); // 4k sector erase code

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address >> 16) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address >> 8) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

    GPIO_SetBits(GPIOA, pin);

    while(ReadStatusRegister() & 0x01);  // BUSY

    CMD(WRITE_DISABLE, pin);

    return true;
}


bool AAI_Word_Program(uint32_t address, uint8_t * buffer, int32_t size, uint8_t pin) {
    if(size < 1) return false;

    // ebsy
    // CMD(ENABLE_BSY, pin);

    // wren
    CMD(WRITE_ENABLE, pin);

    // uint8_t * data = &buffer[0];

    GPIO_ResetBits(GPIOA, pin);

    SPI_I2S_SendData(SPI1, 0xAD); // AAI Mode write

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address >> 16) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address >> 8) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, 0); // Send byte 1
    size--;

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, 0); // Send byte 2
    size--;

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

    GPIO_SetBits(GPIOA, pin);

    while (size > 0)
    {
        GPIO_ResetBits(GPIOA, pin);

        SPI_I2S_SendData(SPI1, 0xAD); // AAI Mode write

        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
        SPI_I2S_SendData(SPI1, 0); // Send byte 1
        size--;

        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
        SPI_I2S_SendData(SPI1, 0); // Send byte 2
        size--;

        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

        GPIO_SetBits(GPIOA, pin);
    }

    // wrdi
    CMD(WRITE_DISABLE, pin);

    // dbsy
    // CMD(DISABLE_BSY, pin);

    // rdsr
    CMD(READ_STATUS_REGISTER, pin);

    return true;
}

bool SaveWavetable(WAVETABLE wt, uint8_t table) {
    // write wavetable to sector
    uint8_t buffer[sizeof(WAVETABLE)];
    uint16_t size = sizeof(WAVETABLE);

    memcpy(buffer, &wt, sizeof(WAVETABLE));
    
    uint32_t address = table * 4096;

    uint8_t pin = kPinPersistentSS;

    Sector_Erase4K(address, pin);

    AAI_Word_Program(address, buffer, size, pin);

    return true;
}

bool SaveWave(const char * name, int16_t * data, uint8_t table, uint8_t frame) {
    WAVETABLE wt = GetWavetable(table);

    // if(t->factory_preset)
        // return false;

    uint8_t pin = wt.waves[frame].factory_preset ? kPinFactorySS : kPinUserSS;
    
    uint32_t address = wt.waves[frame].memory_location;
    
    Sector_Erase4K(address, pin);

    AAI_Word_Program(address, (uint8_t *)data, 2048 * 2, pin);

    return true;
}

bool Test(uint8_t bits) {
    CMD(WRITE_ENABLE, kPinFactorySS);

    CMD(ENABLE_WRITE_STATUS_REGISTER, kPinFactorySS);

    GPIO_ResetBits(GPIOA, kPinFactorySS);

    SPI_I2S_SendData(SPI1, WRITE_STATUS_REGISTER);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    // while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

    SPI_I2S_SendData(SPI1, bits);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

    GPIO_SetBits(GPIOA, kPinFactorySS);

    return true;
}


bool Program(uint32_t address, uint8_t pin) {
        // wren
    CMD(WRITE_ENABLE, pin);

    GPIO_ResetBits(GPIOA, pin);

    SPI_I2S_SendData(SPI1, 0x02);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address >> 16) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address >> 8) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, 10); // Send byte 1

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

    uint8_t temp = SPI1->DR;
    temp = SPI1->SR;
    temp++;

    GPIO_SetBits(GPIOA, pin);

    while(ReadStatusRegister() & 0x01);  // BUSY


    // wrdi
    CMD(WRITE_DISABLE, pin);

    return true;

}

uint8_t Read(uint32_t address) {
        // wren
    // CMD(WRITE_ENABLE, pin);

    GPIO_ResetBits(GPIOA, kPinFactorySS);

    SPI_I2S_SendData(SPI1, 0x03); // Read Instruction High speed 0x0B.  normal speed is 0x03

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address >> 16) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address >> 8) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (address) & 0xFF); // Send Address

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

    uint8_t result;

    SPI_I2S_SendData(SPI1, 0x00);
    // while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    // SPI1->DR = 0;  // send dummy data
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    // while (!((SPI1->SR) &(1<<0))){};  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
    result = SPI_I2S_ReceiveData(SPI1);

    GPIO_SetBits(GPIOA, kPinFactorySS);

    // wrdi
    // CMD(WRITE_DISABLE, pin);

    return result;

}

uint8_t ReadStatusRegister() {

    GPIOA->BSRRH = GPIO_Pin_4;

    SPI1->DR = 0x05;
    // while (!((SPI1->SR) & (1 << 1))) {}; // wait for TXE bit to set -> This will indicate that the buffer is empty
    // while (((SPI1->SR) & (1 << 7))) {}; // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication

    while (!(SPI1->SR & SPI_SR_TXE)); //Tx buffer not empty.
    // while (SPI1->SR & SPI_SR_BSY); //SPI (or I2S) is busy in communication or Tx buffer is not empty.

    //  Clear the Overrun flag by reading DR and SR
    // uint8_t temp = SPI1->DR;
    // temp = SPI1->SR;
    // temp++;

    // SPI1->DR = 0;
    uint8_t result;


    while (!(SPI1->SR & SPI_SR_RXNE));
    result = SPI1->DR;

    GPIOA->BSRRL = GPIO_Pin_4;

    return result;

}

void Write(uint8_t * buf, uint8_t size) {
    for(int i = 0; i < size; i++) {

        uint8_t bsize = 8;

        while(bsize) {
            //clock low
            LOW(eeprom_clock_gpio, eeprom_clock_pin);
        
            Wait<2>();

            bool set = (buf[i] >> (bsize - 1)) & 0x1;
            if(set)
                HIGH(eeprom_mosi_gpio, eeprom_mosi_pin);
            else
                LOW(eeprom_mosi_gpio, eeprom_mosi_pin);

            //clock high
            HIGH(eeprom_clock_gpio, eeprom_clock_pin);

            Wait<2>();

            bsize--;
        }
    }

}


void Read(uint8_t * buf, uint8_t size) {
    for(int i = 0; i < size; i++) {

        uint8_t bsize = 8;

        while(bsize) {
            //clock low
            LOW(eeprom_clock_gpio, eeprom_clock_pin);
        
            Wait<2>();

            bool set = READ(eeprom_miso_gpio, eeprom_miso_pin);
            if(set)
                buf[i] |= (0x1 << (bsize - 1));
            else
                buf[i] |= (0x0 << (bsize - 1));


            //clock high
            HIGH(eeprom_clock_gpio, eeprom_clock_pin);

            Wait<2>();

            bsize--;
        }
    }
}

uint8_t ReadID() {

    //clock low
    LOW(eeprom_clock_gpio, eeprom_clock_pin);

    HIGH(eeprom_ss_gpio, eeprom_ss_pin);

    LOW(eeprom_ss_gpio, eeprom_ss_pin);

    uint8_t buf[1] = {0x9f};

    Write(buf, 1);

    uint8_t byte = 0;

    Read(&byte, 1);

    HIGH(eeprom_ss_gpio, eeprom_ss_pin);

    //clock low
    LOW(eeprom_clock_gpio, eeprom_clock_pin);

    return byte;
}

// uint8_t ReadIDOld() {

//     GPIOA->BSRRL = GPIO_Pin_4;

//     GPIOA->BSRRH = GPIO_Pin_4;


//     uint8_t result;
//     SPI1->DR = 0x9f;
//     // Wait<128>();
//     // while (!(SPI1->SR & SPI_SR_TXE)); //Tx buffer not empty.
//     // while (SPI1->SR & SPI_SR_BSY); //SPI (or I2S) is busy in communication or Tx buffer is not empty.

//     while (!(SPI1->SR & SPI_SR_RXNE));
//     result = SPI1->DR;

//     SPI1->DR = 0;
//     while (!(SPI1->SR & SPI_SR_RXNE));
//     SPI1->DR;

//     SPI1->DR = 0;
//     while (!(SPI1->SR & SPI_SR_RXNE));
//     SPI1->DR;

//     GPIOA->BSRRL = GPIO_Pin_4;

//     return result;

// }

bool InitMemory() {
    // Test(12);
    // Sector_Erase4K(0, kPinFactorySS);
    // Program(0, kPinFactorySS);
    // for(uint8_t table = 0; table < 1; table++) {
    //     for(uint8_t frame = 0; frame < 16; frame++) {
    //           uint32_t address = table * 2048 * 16 * 2 + frame * 2048 * 2;
    //           Sector_Erase4K(address, kPinFactorySS);
    //           // AAI_Word_Program(address, (uint8_t *)&ROM[table * 2048 * 16 + frame * 2048], 2048 * 2, kPinFactorySS);
              
    //     }

    // }
  return true;
}


  // void NextSample() {
  //   GPIOA->BSRR = GPIO_Pin_15;

  //   uint16_t sample = (*next_sample_fn_)();
  //   GPIOA->BRR = GPIO_Pin_15;

  //   sample <<= 4;
  //   SPI3->DR = 0x0340 | (sample >> 12);
  //   Wait<64>();
  //   SPI3->DR = (sample << 4);
  // }
  
  // static FirmwareUpdateDac* GetInstance() { return instance_; }
  
 private:
  // NextSampleFn next_sample_fn_;
  // static FirmwareUpdateDac* instance_;
  
  DISALLOW_COPY_AND_ASSIGN(Flash);
};

template<> inline void Flash::Wait<0>() { }

}  // namespace stages

#endif  // WAVES_DRIVERS_FLASH_H_
