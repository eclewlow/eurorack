// /*
//  ==============================================================================
 
//  Storage.h
//  Created: 20 Nov 2023 5:41:08pm
//  Author:  Eugene Clewlow
 
//  ==============================================================================
//  */

// #pragma once

// #include "waves/wavetables.h"
// #include "waves/Globals.h"

// class Storage {
// public:
//     Storage() {};
//     ~Storage() {};
//     void Init();
//     int16_t LoadWaveSample(int table, int frame, int index);
//     void LoadWaveSample(int16_t * wavedata, int16_t table, float morph);
//     void LoadWaveSample(int16_t * wavedata, int16_t table, int16_t frame);
//     bool SaveWavetable(char * name, int table);
//     bool SaveWave(const char * name, int16_t * data, int table, int frame);
//     bool SwapWavetables(int table1, int table2);
//     bool SwapWaves(int table, int frame1, int frame2);
//     bool DeleteWavetable(int table);
//     bool DeleteWave(int table, int frame);
//     bool CopyWavetable(int table_dst, int table_src);
//     bool CopyWave(int table_dst, int frame_dst, int table_src, int frame_src);
//     bool EraseAll();
//     bool EraseAllWavetables();
//     bool EraseAllSnapshots();
//     bool ResetFactoryWavetables();
//     bool EraseAllUserWavetables();
//     bool EraseSnapshot(SNAPSHOT * snapshot, uint8_t index, bool save);
//     bool SaveSnapshot(const char * name, uint8_t index, SNAPSHOT * snapshot);
//     bool SaveAll();
//     bool LoadAll();
// //    bool SaveAllData();
// //    bool RestoreAllData();
//     int8_t GetNumberOfWavesInTable(int16_t table);
//     inline WAVETABLE* GetWavetable(int8_t table) { return &persistent_storage_.wavetables[table]; }
//     inline WAVETABLE* GetWavetables() { return persistent_storage_.wavetables; }
//     inline SNAPSHOT* GetSnapshot(int8_t snapshot) { return &persistent_storage_.snapshots[snapshot]; }
//     //    int16_t NumAvailableWaveSlots();
//     //    int16_t NextAvailableWaveSlot();
// private:
//     // 64 wavetables.  32 Factory, 32 User
//     // 1024 waves.  512 Facotry, 512 User
//     // 64 snapshots.  64 user.
// //    WAVETABLE WaveTables[FACTORY_WAVETABLE_COUNT + USER_WAVETABLE_COUNT];
//     PERSISTENT_STORAGE persistent_storage_;
    
//     // juce::File save_file_;

//     DISALLOW_COPY_AND_ASSIGN (Storage);
// };
