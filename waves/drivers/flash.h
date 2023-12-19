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
#include "math.h"

namespace waves {

using namespace std;
using namespace stmlib;


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


  void HIGH(uint8_t index) {
    GPIO_SetBits(eeprom[index].gpio, eeprom[index].pin);
  }

  void LOW(uint8_t index) {
    GPIO_ResetBits(eeprom[index].gpio, eeprom[index].pin);
  }

  uint8_t READ(uint8_t index) {
    return GPIO_ReadInputDataBit(eeprom[index].gpio, eeprom[index].pin);
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


bool WriteStatusRegister(uint8_t byte, uint8_t pin) {
    CMD(WRITE_ENABLE, pin);

    CMD(ENABLE_WRITE_STATUS_REGISTER, pin);

    //clock low
    LOW(EEPROM_CLOCK);

    HIGH(pin);

    LOW(pin);

    uint8_t buf[2] = { WRITE_STATUS_REGISTER, byte };

    Write(buf, 2);

    HIGH(pin);

    //clock low
    LOW(EEPROM_CLOCK);

    return true;
}


void Write(uint8_t * buf, uint32_t size) {
    for(uint32_t i = 0; i < size; i++) {

        uint8_t bsize = 8;

        while(bsize) {
            //clock low
            LOW(EEPROM_CLOCK);
        
            // Wait<1>();

            bool set = (buf[i] >> (bsize - 1)) & 0x1;
            if(set)
                HIGH(EEPROM_MOSI);
            else
                LOW(EEPROM_MOSI);

            //clock high
            HIGH(EEPROM_CLOCK);

            // Wait<1>();

            bsize--;
        }
    }

}


void ReadFast(uint8_t * buf, uint32_t size) {
    for(uint32_t i = 0; i < size; i++) {

        uint8_t bsize = 8;

        while(bsize) {
            //clock low
            LOW(EEPROM_CLOCK);

            bool set = READ(EEPROM_MISO);
            if(set)
                buf[i] |= (0x1 << (bsize - 1));
            else
                buf[i] |= (0x0 << (bsize - 1));


            //clock high
            HIGH(EEPROM_CLOCK);

            bsize--;
        }
    }
}

void Read(uint8_t * buf, uint32_t size) {
    for(uint32_t i = 0; i < size; i++) {

        uint8_t bsize = 8;

        while(bsize) {
            //clock low
            LOW(EEPROM_CLOCK);
        
            // Wait<1>();

            bool set = READ(EEPROM_MISO);
            if(set)
                buf[i] |= (0x1 << (bsize - 1));
            else
                buf[i] |= (0x0 << (bsize - 1));


            //clock high
            HIGH(EEPROM_CLOCK);

            // Wait<1>();

            bsize--;
        }
    }
}

void CMD(uint8_t code, uint8_t pin) {
    //clock low
    LOW(EEPROM_CLOCK);

    HIGH(pin);

    LOW(pin);

    uint8_t buf[1] = { code };

    Write(buf, 1);

    HIGH(pin);

    //clock low
    LOW(EEPROM_CLOCK);
}

uint8_t Jedec_ID_Read() {

    //clock low
    LOW(EEPROM_CLOCK);

    HIGH(EEPROM_FACTORY_SS);

    LOW(EEPROM_FACTORY_SS);

    uint8_t buf[1] = { JEDEC_ID_READ };

    Write(buf, 1);

    uint8_t byte = 0;

    Read(&byte, 1);

    HIGH(EEPROM_FACTORY_SS);

    //clock low
    LOW(EEPROM_CLOCK);

    return byte;
}


uint8_t ReadStatusRegister(uint8_t pin) {

    //clock low
    LOW(EEPROM_CLOCK);

    HIGH(pin);

    LOW(pin);

    uint8_t buf[1] = { READ_STATUS_REGISTER };

    Write(buf, 1);

    uint8_t byte = 0;

    Read(&byte, 1);

    HIGH(pin);

    //clock low
    LOW(EEPROM_CLOCK);

    return byte;
}

bool SectorErase4K(uint32_t address, uint8_t pin) {

    // SET WREN first
    CMD(WRITE_ENABLE, pin);

    //clock low
    LOW(EEPROM_CLOCK);

    HIGH(pin);

    LOW(pin);

    uint8_t buf[4];

    buf[0] = SECTOR_ERASE_4K;
    buf[1] = ((address >> 16) & 0xFF);
    buf[2] = ((address >> 8) & 0xFF);
    buf[3] = ((address) & 0xFF);

    // memcpy(buf, &address, 4);

    buf[0] = SECTOR_ERASE_4K;

    Write(buf, 4);

    HIGH(pin);

    //clock low
    LOW(EEPROM_CLOCK);

    while(ReadStatusRegister(pin) & 0x01);  // BUSY

    CMD(WRITE_DISABLE, pin);

    return true;
}

void Read25Mhz(uint8_t * buf, uint32_t size, uint32_t address, uint8_t pin) {

    //clock low
    LOW(EEPROM_CLOCK);

    HIGH(pin);

    LOW(pin);

    uint8_t send_buf[4];

    send_buf[0] = READ_25MHZ;
    send_buf[1] = ((address >> 16) & 0xFF);
    send_buf[2] = ((address >> 8) & 0xFF);
    send_buf[3] = ((address) & 0xFF);
    // memcpy(send_buf, &address, 4);

    send_buf[0] = READ_25MHZ;

    Write(send_buf, 4);

    Read(buf, size);

    HIGH(pin);

    //clock low
    LOW(EEPROM_CLOCK);
}

void Read66Mhz(uint8_t * buf, uint32_t size, uint32_t address, uint8_t pin) {

    //clock low
    LOW(EEPROM_CLOCK);

    HIGH(pin);

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

    //clock low
    LOW(EEPROM_CLOCK);
}

bool AAI_Word_Program(uint8_t * buffer, uint32_t size, uint32_t address, uint8_t pin) {
    if(size < 2) return false;
    if(size % 2 != 0) return false;

    // TODO: USE HARDWARE BSY FOR FASTER WRITE

    // ebsy
    // CMD(ENABLE_BSY, pin);
    // CMD(DISABLE_BSY, pin);

    // wren
    CMD(WRITE_ENABLE, pin);

    //clock low
    LOW(EEPROM_CLOCK);

    HIGH(pin);

    LOW(pin);

    uint8_t send_buf[6];
    send_buf[0] = AAI_WORD_PROGRAM;
    send_buf[1] = ((address >> 16) & 0xFF);
    send_buf[2] = ((address >> 8) & 0xFF);
    send_buf[3] = ((address) & 0xFF);
    send_buf[4] = buffer[0];
    send_buf[5] = buffer[1];

    Write(send_buf, 6);

    HIGH(pin);

    //clock low
    LOW(EEPROM_CLOCK);

    while(ReadStatusRegister(pin) & 0x01);  // BUSY

    uint32_t i = 2;

    while(i < size) {
        HIGH(pin);
        LOW(pin);

        uint8_t send_buf[3] = {
            AAI_WORD_PROGRAM,
            buffer[i++],
            buffer[i++],
        };

        Write(send_buf, 3);

        HIGH(pin);
        //clock low
        LOW(EEPROM_CLOCK);

        while(ReadStatusRegister(pin) & 0x01);  // BUSY
    }

    // wrdi
    CMD(WRITE_DISABLE, pin);

    // dbsy
    // CMD(DISABLE_BSY, pin);

    return true;
}

bool Program(uint8_t byte, uint32_t address, uint8_t pin) {
    // wren
    CMD(WRITE_ENABLE, pin);

    //clock low
    LOW(EEPROM_CLOCK);

    HIGH(pin);

    LOW(pin);

    uint8_t buf[5];
    buf[0] = BYTE_PROGRAM;
    buf[1] = ((address >> 16) & 0xFF);
    buf[2] = ((address >> 8) & 0xFF);
    buf[3] = ((address) & 0xFF);
    buf[4] = byte;

    Write(buf, 5);

    HIGH(pin);

    //clock low
    LOW(EEPROM_CLOCK);

    while(ReadStatusRegister(pin) & 0x01);  // BUSY

    // wrdi
    CMD(WRITE_DISABLE, pin);

    return true;

}

bool InitMemory() {
    WriteStatusRegister(0, EEPROM_FACTORY_SS);
    for(uint8_t table = 0; table < 1; table++) {
        for(uint8_t frame = 0; frame < 16; frame++) {
              // uint32_t address = table * 2048 * 16 * 2 + frame * 2048 * 2;
              // SectorErase4K(address, EEPROM_FACTORY_SS);
              // AAI_Word_Program((uint8_t *)&ROM[table * 2048 * 16 + frame * 2048], 2048 * 2, address, EEPROM_FACTORY_SS);
        }

    }

    Read66Mhz((uint8_t * )buffer0, 4096, 0, EEPROM_FACTORY_SS);
    // Program(0, EEPROM_FACTORY_SS);
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
    void Swap_Buffers() {
        if(wavetable_engine_front_buffer_1 == buffer0) {
            wavetable_engine_back_buffer_1  = buffer0;
            wavetable_engine_back_buffer_2  = buffer1;
            wavetable_engine_front_buffer_1 = buffer2;
            wavetable_engine_front_buffer_2 = buffer3;
        } else {
            wavetable_engine_front_buffer_1 = buffer0;
            wavetable_engine_front_buffer_2 = buffer1;
            wavetable_engine_back_buffer_1  = buffer2;
            wavetable_engine_back_buffer_2  = buffer3;            
        }
        current_frame = target_frame;
    }

    void Load_BackBuffer() {
        Read66Mhz((uint8_t *)wavetable_engine_back_buffer_1, 4096, target_frame * 4096, EEPROM_FACTORY_SS);
        if(target_frame < 15)
            Read66Mhz((uint8_t *)wavetable_engine_back_buffer_2, 4096, (target_frame + 1) * 4096, EEPROM_FACTORY_SS);
        else
            Read66Mhz((uint8_t *)wavetable_engine_back_buffer_2, 4096, (target_frame) * 4096, EEPROM_FACTORY_SS);
    }

    float LoadWaveSample(int table, float morph, float phase) {

      float index = phase * 2048.0;
      uint16_t integral = floor(index);
      float fractional = index - integral;
      
      uint16_t nextIntegral = (integral + 1) % 2048;
      
      int16_t s1 = 0;
      int16_t s2 = 0;

      int16_t t1 = 0;
      int16_t t2 = 0;

        // morph is 0 to 15
      float frame = morph * 15.0f;

        uint16_t frame_integral = floor(frame);
        float frame_fractional = frame - frame_integral;

        if (static_cast<uint8_t>(frame) != current_frame) {
            // trigger load
            trigger_load = true;
            target_frame = static_cast<uint8_t>(frame);
        }

       s1 = wavetable_engine_front_buffer_1[integral];
       s2 = wavetable_engine_front_buffer_1[nextIntegral];

       t1 = wavetable_engine_front_buffer_2[integral];
       t2 = wavetable_engine_front_buffer_2[nextIntegral];

      float s_interpolated = (s1 + (s2 - s1) * fractional) / 32768.0f;
      float t_interpolated = (t1 + (t2 - t1) * fractional) / 32768.0f;

        float sample = s_interpolated * (1.0f - frame_fractional) + t_interpolated * frame_fractional;

        // if(morph)
        // WAVETABLE * t = GetWavetable(table);
            
        // TODO: this should be a read to ROM data
        // the wavetable struct data exists in MCU flash
        // and the wave struct data exists in MCU flash
        // but the wave struct data field points to a memory location in ROM.
        // return ROM[t->waves[frame].memory_location + index];
            return sample;
    }
/*

    if target 2 wavetable / frame != current 2 wavetable / frame-morph
    -   start loading target 2 table/frames into 2 back buffer    
    -   once loading is finished, swap 2 back with 2 front buffer.
*/
    int16_t *wavetable_engine_front_buffer_1;
    int16_t *wavetable_engine_front_buffer_2;
    int16_t *wavetable_engine_back_buffer_1;
    int16_t *wavetable_engine_back_buffer_2;
    uint8_t current_table;
    uint8_t current_frame;
    uint8_t target_table;
    uint8_t target_frame;
    bool trigger_load;
    bool loading;
  int16_t buffer0[2048];
  int16_t buffer1[2048];
  int16_t buffer2[2048];
  int16_t buffer3[2048];
 private:
  // NextSampleFn next_sample_fn_;
  // static FirmwareUpdateDac* instance_;
  
  DISALLOW_COPY_AND_ASSIGN(Flash);
};

template<> inline void Flash::Wait<0>() { }

}  // namespace stages

#endif  // WAVES_DRIVERS_FLASH_H_
