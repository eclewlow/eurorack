/*
  ==============================================================================

    Storage.cpp
    Created: 20 Nov 2023 5:41:08pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/wavetables.h"
#include "waves/Globals.h"

void Storage::Init() {
//    save_file_ = juce::File::getCurrentWorkingDirectory().getChildFile ("file.txt");
    
//    printf("%s\n", juce::File::getCurrentWorkingDirectory().getFullPathName().toStdString().c_str());
    save_file_ = juce::File("/Users/eclewlow/file.txt");
    if( !save_file_.existsAsFile() )
    {
        auto result = save_file_.create();
        if( !result.wasOk() )
        {
            printf("failed creating file!");
            DBG("failed creating file!");
            jassertfalse; //pause so we can see the error message
            return;  //bail
        }
        EraseAll();
        SaveAll();
    } else {
        LoadAll();
    }
//    printf("size of = %d", sizeof(SNAPSHOT));
//    printf("signed int=%d", static_cast<uint16_t>(static_cast<int16_t>(65535)));
}

bool Storage::SaveAll() {
    save_file_.replaceWithData(&persistent_storage_, sizeof(PERSISTENT_STORAGE));
    return true;
}

bool Storage::LoadAll() {
    juce::FileInputStream input_stream(save_file_);
    input_stream.read(&persistent_storage_, sizeof(PERSISTENT_STORAGE));
    return true;
}

bool Storage::ResetFactoryWavetables() {
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

    for(int table = 0; table < FACTORY_WAVETABLE_COUNT; table++) {
        strncpy(GetWavetable(table)->name, names[table], 9);
        GetWavetable(table)->factory_preset = true;
        GetWavetable(table)->is_empty = false;
        for(int frame = 0; frame < 16; frame++) {
            GetWavetable(table)->waves[frame].memory_location = table * 16 * 2048 + frame * 2048;
//            strncpy(GetWavetable(table)->waves[frame].name, names[frame], 9);
            snprintf(GetWavetable(table)->waves[frame].name, 9, "%02d", frame);
            GetWavetable(table)->waves[frame].factory_preset = true;
            GetWavetable(table)->waves[frame].is_empty = false;
        }
    }
    
    SaveAll();

    return true;
}
bool Storage::EraseAllUserWavetables() {
    for(int table = FACTORY_WAVETABLE_COUNT; table <  FACTORY_WAVETABLE_COUNT + USER_WAVETABLE_COUNT; table++) {
        strncpy(GetWavetable(table)->name, "INIT", 9);
        GetWavetable(table)->factory_preset = false;
        GetWavetable(table)->is_empty = true;
        for(int frame = 0; frame < 16; frame++) {
            GetWavetable(table)->waves[frame].memory_location = table * 16 * 2048 + frame * 2048;
            strncpy(GetWavetable(table)->waves[frame].name, "INIT", 9);
            GetWavetable(table)->waves[frame].factory_preset = false;
            GetWavetable(table)->waves[frame].is_empty = true;
            memset(&ROM[GetWavetable(table)->waves[frame].memory_location], 0, 2048 * 2);
        }
    }
    return true;
}

bool Storage::EraseAllWavetables() {
    ResetFactoryWavetables();
    EraseAllUserWavetables();
    
    SaveAll();

    return true;
}

bool Storage::EraseAllSnapshots() {
    for(int snapshot = 0; snapshot < FACTORY_SNAPSHOT_COUNT + USER_SNAPSHOT_COUNT; snapshot++) {
        SNAPSHOT * snapshot_ptr = GetSnapshot(snapshot);
        EraseSnapshot(snapshot_ptr, snapshot, false);
    }
    
    SaveAll();

    return true;
}


bool Storage::EraseAll() {
    EraseAllWavetables();
    EraseAllSnapshots();
    
    SaveAll();

    return true;
}

bool Storage::EraseSnapshot(SNAPSHOT *snapshot, uint8_t index, bool save) {
    char* lines[4] = {
        (char*)"WHOOPSIE\0",
        (char*)"TEST\0",
        (char*)"TESTICL\0",
        (char*)"RIGHTO\0",
    };

    SNAPSHOT * snapshot_ptr = snapshot;
    snapshot_ptr->factory_preset = index < FACTORY_SNAPSHOT_COUNT;
    snapshot_ptr->is_empty = !snapshot_ptr->factory_preset;
    strncpy(snapshot_ptr->name, "INIT", 9);
//    strncpy(snapshot_ptr->name, lines[index % 4], 9);
    snapshot_ptr->brightness = 100;
    snapshot_ptr->contrast = 100;
    snapshot_ptr->invert = false;
    snapshot_ptr->scope_setting = UserSettings::SETTING_SCOPE_LINE;
    snapshot_ptr->morph_setting = UserSettings::SETTING_MORPH_SMOOTH;
    // sub osc parameters
    snapshot_ptr->subosc_offset = 0;
    snapshot_ptr->subosc_detune = 0;
    snapshot_ptr->subosc_mix = 100;
    snapshot_ptr->subosc_wave = 0;
    
    snapshot_ptr->fx_depth = 1.0f;
    snapshot_ptr->fx_sync = false;
    snapshot_ptr->fx_scale = 0;
    snapshot_ptr->fx_range = 1;
    snapshot_ptr->fx_oscillator_shape = EffectManager::SINE_SHAPE;
    snapshot_ptr->fx_control_type = EffectManager::INTERNAL_MODULATOR;
    snapshot_ptr->fx_effect = EffectManager::EFFECT_TYPE_BYPASS;

    snapshot_ptr->engine = Context::ENGINE_TYPE_AB;
    // ab engine parameters
    snapshot_ptr->ab_engine_left_wavetable = 0;
    snapshot_ptr->ab_engine_left_frame = 0;
    snapshot_ptr->ab_engine_right_wavetable = 0;
    snapshot_ptr->ab_engine_right_frame = 0;
    snapshot_ptr->ab_engine_is_editing_left = false;
    snapshot_ptr->ab_engine_is_editing_right = false;

    // wavetable engine parameters
    snapshot_ptr->wavetable_engine_wavetable = 0;
    
    // matrix engine parameters
    snapshot_ptr->matrix_engine_x1 = 0;
    snapshot_ptr->matrix_engine_x2 = 7;
    snapshot_ptr->matrix_engine_y1 = 0;
    snapshot_ptr->matrix_engine_y2 = 7;
    snapshot_ptr->matrix_engine_wavelist_offset = 0;
    
    // drum engine parameters
    snapshot_ptr->drum_engine_amp_decay = 1.0f;
    snapshot_ptr->drum_engine_fm_decay = 1.0f;
    snapshot_ptr->drum_engine_fm_shape = 0.5f;
    snapshot_ptr->drum_engine_fm_depth = 0.5f;
    snapshot_ptr->drum_engine_wavetable = 0;

    // pot settings
    snapshot_ptr->pot_fx_amount = adc.getChannel(1);
    snapshot_ptr->pot_fx = adc.getChannel(2);
    snapshot_ptr->pot_morph = adc.getChannel(3);
    
    // calibration settings
    for(int i = 0; i < 4; i++) {
        snapshot_ptr->io_gain[i] = 1.0f;   // don't randomize this, but save in snapshot
        snapshot_ptr->io_bias[i] = 0.0f;   // don't randomize this, but save in snapshot
    }

//    snapshot_ptr->calibration_x = 0.029304029304029;
//    snapshot_ptr->calibration_y = 0;
    snapshot_ptr->calibration_x = 0.023619047619048;    // don't randomize this, but save in snapshot
    snapshot_ptr->calibration_y = 12.0f;    // don't randomize this, but save in snapshot
    
    if(save)
        SaveAll();
    return true;
}

bool Storage::SaveSnapshot(const char * name, uint8_t index, SNAPSHOT * snapshot)
{
    if(GetSnapshot(index)->factory_preset)
        return false;
    
    char temp_name[9];
    
    strncpy(temp_name, name, 9);

    memcpy(GetSnapshot(index), snapshot, sizeof(SNAPSHOT));
    strncpy(GetSnapshot(index)->name, temp_name, 9);
    
    GetSnapshot(index)->factory_preset = false;
    GetSnapshot(index)->is_empty = false;

    SaveAll();
    return true;
}

int16_t Storage::LoadWaveSample(int table, int frame, int index) {

    WAVETABLE * t = GetWavetable(table);
        
    // TODO: this should be a read to ROM data
    // the wavetable struct data exists in MCU flash
    // and the wave struct data exists in MCU flash
    // but the wave struct data field points to a memory location in ROM.
    return ROM[t->waves[frame].memory_location + index];
}

void Storage::LoadWaveSample(int16_t * wavedata, int16_t table, int16_t frame) {
    WAVETABLE * t = GetWavetable(table);

    const int16_t * data = &ROM[t->waves[frame].memory_location];
    memcpy(wavedata, data, 2048 * 2);
}


void Storage::LoadWaveSample(int16_t * wavedata, int16_t table, float morph) {
    
    float frequency = 23.4375;

    float phaseIncrement = frequency / 48000.0f;
    
    float phase = 0.0f;
    
    for(int i = 0; i < 2048; i++) {
        float index = phase * 2048.0f;
        uint16_t integral = floor(index);
        float fractional = index - integral;
        
        uint16_t nextIntegral = (integral + 1) % 2048;
        
        float frame = morph * 15.0f;
        uint16_t frame_integral = floor(frame);
        float frame_fractional = frame - frame_integral;
        
        uint16_t next_frame_integral = (frame_integral + 1) % 16;

        float sample;

        if(frame_fractional > 0.0f) {
            // we need to morph between frames
            float sample1 = LoadWaveSample(table, frame_integral, integral);
            float sample2 = LoadWaveSample(table, frame_integral, nextIntegral);
            
            float interpolated16 = sample1 + (sample2 - sample1) * fractional;
            
            float interpolatedFloat = interpolated16 / 32768.0f;
            
            sample = interpolatedFloat;
            
            sample1 = LoadWaveSample(table, next_frame_integral, integral);
            sample2 = LoadWaveSample(table, next_frame_integral, nextIntegral);
            
            interpolated16 = sample1 + (sample2 - sample1) * fractional;
            
            interpolatedFloat = interpolated16 / 32768.0f;
            
            sample = (1 - frame_fractional) * sample + frame_fractional * interpolatedFloat;

        } else {
            // just do the phase morph
            float sample1 = LoadWaveSample(table, frame_integral, integral);
            float sample2 = LoadWaveSample(table, frame_integral, nextIntegral);
            
            float interpolated16 = sample1 + (sample2 - sample1) * fractional;
            
            float interpolatedFloat = interpolated16 / 32768.0f;
            
            sample = interpolatedFloat;
        }
                
        phase += phaseIncrement;
        
        if(phase >= 1.0f)
            phase -= 1.0f;
        
        wavedata[i] = static_cast<int16_t>(sample * 32767.0f);
    }
}

int8_t Storage::GetNumberOfWavesInTable(int16_t table) {
    int8_t count = 0;
    
    WAVETABLE * t = GetWavetable(table);
    if(t->factory_preset)
        return 16;
    
    for(int8_t i = 0; i < 16; i++) {
        if(!t->waves[i].is_empty)
            count++;
    }
    
    return count;
}

bool Storage::SaveWavetable(char * name, int table) {
    // make sure wavetable is not in factory memory
    WAVETABLE * t = GetWavetable(table);

    if(t->factory_preset)
        return false;

    t->is_empty = GetNumberOfWavesInTable(table) == 0;
    
    strncpy(t->name, name, 9);
    
    SaveAll();

    return true;
}

bool Storage::SaveWave(const char * name, int16_t * data, int table, int frame) {
    WAVETABLE * t = GetWavetable(table);
    if(t->factory_preset)
        return false;

    std::strncpy(t->waves[frame].name, name, 9);
    
    t->waves[frame].memory_location = 2048 * 16 * table + 2048 * frame;
    t->waves[frame].is_empty = false;
    
    std::memcpy((void*)&ROM[t->waves[frame].memory_location], data, 2048 * 2);

    SaveAll();

    return true;
}

bool Storage::CopyWavetable(int table_dst, int table_src) {
    WAVETABLE * t_dst = GetWavetable(table_dst);
    WAVETABLE * t_src = GetWavetable(table_src);
    
    if(t_dst->factory_preset) return false;
    
    t_dst->factory_preset = false;
    strncpy(t_dst->name, t_src->name, 9);
    for(int i = 0; i < 16; i++) {
        strncpy(t_dst->waves[i].name, t_src->waves[i].name, 9);
        t_dst->waves[i].is_empty = t_src->waves[i].is_empty;
        std::memcpy((void*)&ROM[t_dst->waves[i].memory_location], (void*)&ROM[t_src->waves[i].memory_location], 2048 * 2);
    }

    SaveAll();
    return true;
}

bool Storage::CopyWave(int table_dst, int frame_dst, int table_src, int frame_src) {
    WAVETABLE * t_dst = GetWavetable(table_dst);
    WAVETABLE * t_src = GetWavetable(table_src);
    
    if(t_dst->factory_preset) return false;
    
    t_dst->factory_preset = false;

    strncpy(t_dst->waves[frame_dst].name, t_src->waves[frame_src].name, 9);
    std::memcpy((void*)&ROM[t_dst->waves[frame_dst].memory_location], (void*)&ROM[t_src->waves[frame_src].memory_location], 2048 * 2);

    t_dst->waves[frame_dst].is_empty = t_src->waves[frame_src].is_empty;
    
    SaveAll();
    return true;
}


bool Storage::DeleteWavetable(int table) {
    WAVETABLE * t = GetWavetable(table);
    if(t->factory_preset)
        return false;

    for (int8_t i = 0; i < 16; i++) {
        strncpy(t->waves[i].name, "INIT", 9);
        memset(&ROM[t->waves[i].memory_location], 0, 2048 * 2);
        t->waves[i].is_empty = true;

    }
    strncpy(t->name, "INIT", 9);
    t->is_empty = true;

    SaveAll();
    return true;
}

bool Storage::DeleteWave(int table, int frame) {
    WAVETABLE * t = GetWavetable(table);
    if(t->factory_preset)
        return false;
    
    strncpy(t->waves[frame].name, "INIT", 9);
    memset(&ROM[t->waves[frame].memory_location], 0, 2048 * 2);
    t->waves[frame].is_empty = true;
    
    SaveAll();
    return true;
}

bool Storage::SwapWaves(int table, int frame1, int frame2) {
    WAVETABLE * wt = GetWavetable(table);
    
    if(wt->factory_preset)
        return false;
    
    WAVE w = GetWavetable(table)->waves[frame1];
    
    GetWavetable(table)->waves[frame1] = GetWavetable(table)->waves[frame2];
    GetWavetable(table)->waves[frame2] = w;
    return true;
}

bool Storage::SwapWavetables(int table1, int table2) {
        // easy swap
    WAVETABLE wt = persistent_storage_.wavetables[table1];
    persistent_storage_.wavetables[table1] = persistent_storage_.wavetables[table2];
    persistent_storage_.wavetables[table2] = wt;
    return true;
    /*
     // swapping wavetables requires swapping of all wave data memory locations.
    char name_buffer1[9];
    char name_buffer2[9];

    for(int frame = 0; frame < 16; frame++) {
        // TODO: if the wave doesn't exist, we have to update the memory_location before swapping
        if(!WaveDoesExist(table1, frame)) {
            WaveTables[table1].waves[frame].memory_location = 2048 * 16 * table1 + 2048 * frame;
        }
        if(!WaveDoesExist(table2, frame)) {
            WaveTables[table2].waves[frame].memory_location = 2048 * 16 * table2 + 2048 * frame;
        }

        // swap frames between tables
        uint16_t swap_buffer1[2048];
        uint16_t swap_buffer2[2048];

        std::memcpy(swap_buffer1, &ROM[WaveTables[table1].waves[frame].memory_location], 2048 * 2);
        std::memcpy(swap_buffer2, &ROM[WaveTables[table2].waves[frame].memory_location], 2048 * 2);
        strncpy(name_buffer1, WaveTables[table1].waves[frame].name, 9);
        strncpy(name_buffer2, WaveTables[table2].waves[frame].name, 9);

        std::memcpy(&ROM[WaveTables[table1].waves[frame].memory_location], swap_buffer2, 2048 * 2);
        std::memcpy(&ROM[WaveTables[table2].waves[frame].memory_location], swap_buffer1, 2048 * 2);
        strncpy(WaveTables[table1].waves[frame].name, name_buffer2, 9);
        strncpy(WaveTables[table2].waves[frame].name, name_buffer1, 9);
    }

    strncpy(name_buffer1, WaveTables[table1].name, 9);
    strncpy(name_buffer2, WaveTables[table2].name, 9);
    strncpy(WaveTables[table1].name, name_buffer2, 9);
    strncpy(WaveTables[table2].name, name_buffer1, 9);

    return false;
     //*/
}
//
//int16_t Storage::NumAvailableWaveSlots() {
//    int16_t count = 0;
//    for(int i = 0; i < FACTORY_WAVE_COUNT + USER_WAVE_COUNT; i ++) {
//        if(Waves[i].name[0] != '\0')
//            count ++;
//    }
//    return count;
//}
//
//int16_t Storage::NextAvailableWaveSlot() {
//    for(int i = 0; i < FACTORY_WAVE_COUNT + USER_WAVE_COUNT; i ++) {
//        if(Waves[i].name[0] != '\0')
//            return i;
//    }
//    return -1;
//}

