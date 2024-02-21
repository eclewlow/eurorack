/*
  ==============================================================================

    DrumMode.cpp
    Created: 30 Nov 2023 5:44:53pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

namespace waves {

DrumMode::DrumMode() {
    edit_state_ = DRUM_MODE_EDIT_WAVETABLE;
    setEditing(false);
}

DrumMode::~DrumMode() {
    
}

void DrumMode::setEditing(bool editing) {
    is_editing_ = editing;
    timer_ = system_clock.milliseconds();
}


void DrumMode::triggerUpdate(bool back_pressed) {
    setEditing(false);
    edit_state_ = DRUM_MODE_EDIT_WAVETABLE;
}


void DrumMode::on_load_wavetable_names_finished() {
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}

bool DrumMode::handleKeyRelease(int key) {
//    DRUM_MODE_EDIT_WAVETABLE = 0,
//    DRUM_MODE_EDIT_AMP_DECAY = 1,
//    DRUM_MODE_EDIT_FM_DECAY = 2,
//    DRUM_MODE_EDIT_FM_SHAPE = 3,
//    DRUM_MODE_EDIT_FM_DEPTH = 4,
    float dx = 0.01f;

    if(key == LEFT_ENCODER_CCW) {
        edit_state_ = CLAMP<int8_t>(edit_state_ - 1, DRUM_MODE_EDIT_WAVETABLE, DRUM_MODE_EDIT_FM_DEPTH);
        setEditing(false);
    }
    if(key == LEFT_ENCODER_CW) {
        edit_state_ = CLAMP<int8_t>(edit_state_ + 1, DRUM_MODE_EDIT_WAVETABLE, DRUM_MODE_EDIT_FM_DEPTH);
        setEditing(false);
    }
    if(key == RIGHT_ENCODER_CCW) {
        switch(edit_state_) {
            case DRUM_MODE_EDIT_WAVETABLE:
                flash.StartFrameDMARead((uint32_t*)wavetable_names_, 16 * 9, 0, DrumMode::on_load_wavetable_names_finished, EEPROM_PERSISTENT_SS);
                drumEngine.SetWavetable(drumEngine.GetWavetable() - 1);
                break;
            case DRUM_MODE_EDIT_AMP_DECAY:
                drumEngine.SetAmpDecay(drumEngine.GetAmpDecay() - dx);
                setEditing(true);
                break;
            case DRUM_MODE_EDIT_FM_DECAY:
                drumEngine.SetFMDecay(drumEngine.GetFMDecay() - dx);
                setEditing(true);
                break;
            case DRUM_MODE_EDIT_FM_SHAPE:
                drumEngine.SetFMShape(drumEngine.GetFMShape() - dx);
                setEditing(true);
                break;
            case DRUM_MODE_EDIT_FM_DEPTH:
                drumEngine.SetFMDepth(drumEngine.GetFMDepth() - dx);
                setEditing(true);
                break;
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CW) {
        switch(edit_state_) {
            case DRUM_MODE_EDIT_WAVETABLE:
                flash.StartFrameDMARead((uint32_t*)wavetable_names_, 16 * 9, 0, DrumMode::on_load_wavetable_names_finished, EEPROM_PERSISTENT_SS);
                drumEngine.SetWavetable(drumEngine.GetWavetable() + 1);
                break;
            case DRUM_MODE_EDIT_AMP_DECAY:
                drumEngine.SetAmpDecay(drumEngine.GetAmpDecay() + dx);
                setEditing(true);
                break;
            case DRUM_MODE_EDIT_FM_DECAY:
                drumEngine.SetFMDecay(drumEngine.GetFMDecay() + dx);
                setEditing(true);
                break;
            case DRUM_MODE_EDIT_FM_SHAPE:
                drumEngine.SetFMShape(drumEngine.GetFMShape() + dx);
                setEditing(true);
                break;
            case DRUM_MODE_EDIT_FM_DEPTH:
                drumEngine.SetFMDepth(drumEngine.GetFMDepth() + dx);
                setEditing(true);
                break;
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CLICK) {
    }
    if(key == LEFT_ENCODER_CLICK) {
    }
    if(key == BACK_BUTTON) {
        if(back_menu_)
            context.setState(back_menu_, true);
        else
            context.setState(&mainMenu);
    }

    return true;
}

void DrumMode::paint() {
    Display::clear_screen();
    
    // uint16_t morph = adc.getChannelProcessed(3);
    
    uint32_t elapsed_time = system_clock.milliseconds() - timer_;
    if(is_editing_ && elapsed_time > 1000) {
        setEditing(false);
    }

    /****/
    // draw wave
    /****/
    int wave_width = 108;
    int wave_height = 22;
    int x_offset = 64 - wave_width / 2;
    int y_offset = 0;//32 - 10 - wave_height;
    Display::outline_rectangle(x_offset, y_offset, wave_width, wave_height);
    
    int16_t wave_buffer[2048];
    // drumEngine.FillWaveform(wave_buffer, morph * 1.0f / 4095.0f);

    float morph = adc.getChannelProcessed(3) / 65536.0f;

    uint8_t frame = morph * 15.0f;

    if(wavetable_gui_ != drumEngine.GetWavetable() && frame_gui_ != frame) {
        // load double frame. draw double frame
        flash.StartFrameDMARead((uint32_t*)front_buffer_4, 8192, drumEngine.GetWavetable() * 65536 + frame * 4096, NULL, EEPROM_FACTORY_SS);
        wavetable_gui_ = drumEngine.GetWavetable();
        frame_gui_ = frame;
    }
    // storage.LoadWaveSample(BUF1, wavetable_, morph_);

    abEngine.FillWaveform(wave_buffer, morph);

    // storage.LoadWaveSample(BUF1, wavetable_, morph * 1.0f / 4095.0f);



    Display::Draw_Wave(x_offset + 1, y_offset + 1, wave_width - 2, wave_height - 2, wave_buffer);

    /****/
    // draw cursor
    /****/
    int x = x_offset + 1 + (wave_width - 2 - 4) * morph;
    Display::invert_rectangle(x, y_offset + 1, 4, wave_height - 2);

    
    /****/
    // draw wave name
    /****/
    char * line;
    // if(storage.GetWavetable(drumEngine.GetWavetable())->name[0] == '\0')
    //     line = (char*)"--------";
    // else
    //     line = storage.GetWavetable(drumEngine.GetWavetable())->name;
    line = wavetable_names_[drumEngine.GetWavetable()];

    y_offset += wave_height + 2;
    x_offset = 64 - strlen("WAVETABLE:") * 6 + 6;
    Display::put_string_5x5(x_offset, y_offset, strlen("WAVETABLE:"), "WAVETABLE:");
    Display::put_string_5x5(x_offset + strlen("WAVETABLE:") * 6, y_offset, strlen(line), line, edit_state_ == DRUM_MODE_EDIT_WAVETABLE);

    /****/
    // draw squares and circles
    /****/
    int shape_width = 18;
    int gap = (128 - shape_width * 4) / 5;
    
    x_offset = gap;
    y_offset = 32;
    Display::outline_rectangle(x_offset, y_offset, shape_width, shape_width);
    Display::LCD_Line(x_offset + 1, y_offset + 1, x_offset + 1 + (shape_width - 2) * drumEngine.GetAmpDecay(), y_offset + 1 + shape_width - 2, true);
    if(edit_state_ == DRUM_MODE_EDIT_AMP_DECAY)
        Display::invert_rectangle(x_offset + 1, y_offset + 1, shape_width - 2, shape_width - 2);

    x_offset += gap + shape_width;
    Display::outline_rectangle(x_offset, y_offset, shape_width, shape_width);
    Display::LCD_Line(x_offset + 1, y_offset + 1, x_offset + 1 + (shape_width - 2) * drumEngine.GetFMDecay(), y_offset + 1 + shape_width - 2, true);
    if(edit_state_ == DRUM_MODE_EDIT_FM_DECAY)
        Display::invert_rectangle(x_offset + 1, y_offset + 1, shape_width - 2, shape_width - 2);

    x_offset += gap + shape_width;
    Display::outline_rectangle(x_offset, y_offset, shape_width, shape_width);
    
    int last_x = -1;
    for (float i = 0.0f; i < 1.0; i += 0.01f) {
        // float curve = drumEngine.GetFMShape();
        float x = i;
        float y = drumEngine.GetY(x);
        
        int ix = x_offset + 1 + x * (shape_width - 2);

        if(last_x != ix)
            Display::Put_Pixel(x_offset + 1 + x * (shape_width - 2), (y_offset + 1 + shape_width - 2) - y * (shape_width - 2), true);
        last_x = ix;
    }

    if(edit_state_ == DRUM_MODE_EDIT_FM_SHAPE)
        Display::invert_rectangle(x_offset + 1, y_offset + 1, shape_width - 2, shape_width - 2);

    x_offset += gap + shape_width;

    x_offset += shape_width / 2;
    y_offset += shape_width / 2;
    int radius = shape_width / 2;
    float angle = 2 * M_PI * ((1.0f - drumEngine.GetFMDepth()) * 300.0f - 60.0f) / 360.0f;

    if(edit_state_ == DRUM_MODE_EDIT_FM_DEPTH) {
        Display::LCD_FillCircle(x_offset, y_offset, radius, true);
        Display::LCD_Line(x_offset, y_offset, radius * cos(angle) + x_offset, y_offset - radius * sin(angle), true, true);
    } else {
        Display::LCD_Line(x_offset, y_offset, radius * cos(angle) + x_offset, y_offset - radius * sin(angle), true);
        Display::LCD_Circle(x_offset, y_offset, radius, true);
    }

    /****/
    // draw text
    /****/
    x_offset = gap + shape_width / 2;
    y_offset += shape_width / 2 + 2;
    
    if(edit_state_ == DRUM_MODE_EDIT_AMP_DECAY && is_editing_) {
        char line[20];
        snprintf(line, 20, "%d", static_cast<int8_t>(drumEngine.GetAmpDecay() * 100.0f));
        Display::put_string_5x5(x_offset - strlen(line) * 6 / 2, y_offset + 6 / 2 - 1, strlen(line), line);
    } else {
        Display::put_string_5x5(x_offset - strlen("AMP") * 6 / 2, y_offset, strlen("AMP"), "AMP");
        Display::put_string_5x5(x_offset - strlen("DECY") * 6 / 2, y_offset + 6, strlen("DECY"), "DECY");
    }

    x_offset += gap + shape_width;

    if(edit_state_ == DRUM_MODE_EDIT_FM_DECAY && is_editing_) {
        char line[20];
        snprintf(line, 20, "%d", static_cast<int8_t>(drumEngine.GetFMDecay() * 100.0f));
        Display::put_string_5x5(x_offset - strlen(line) * 6 / 2, y_offset + 6 / 2 - 1, strlen(line), line);
    } else {
        Display::put_string_5x5(x_offset - strlen("FM") * 6 / 2, y_offset, strlen("FM"), "FM");
        Display::put_string_5x5(x_offset - strlen("DECY") * 6 / 2, y_offset + 6, strlen("DECY"), "DECY");
    }

    // depth -50 to 50
    // decays 0 to 100
    // shape -50 to 50

    x_offset += gap + shape_width;

    if(edit_state_ == DRUM_MODE_EDIT_FM_SHAPE && is_editing_) {
        char line[20];
        snprintf(line, 20, "%d", static_cast<int8_t>(drumEngine.GetFMShape() * 100.0f - 50.0f));
        Display::put_string_5x5(x_offset - strlen(line) * 6 / 2, y_offset + 6 / 2 - 1, strlen(line), line);
    } else {
        Display::put_string_5x5(x_offset - strlen("FM") * 6 / 2, y_offset, strlen("FM"), "FM");
        Display::put_string_5x5(x_offset - strlen("SHPE") * 6 / 2, y_offset + 6, strlen("SHPE"), "SHPE");
    }

    
    x_offset += gap + shape_width;

    if(edit_state_ == DRUM_MODE_EDIT_FM_DEPTH && is_editing_) {
        char line[20];
        snprintf(line, 20, "%d", static_cast<int8_t>(drumEngine.GetFMDepth() * 100.0f - 50.0f));
        Display::put_string_5x5(x_offset - strlen(line) * 6 / 2, y_offset + 6 / 2 - 1, strlen(line), line);
    } else {
        Display::put_string_5x5(x_offset - strlen("FM") * 6 / 2, y_offset, strlen("FM"), "FM");
        Display::put_string_5x5(x_offset - strlen("DPTH") * 6 / 2, y_offset + 6, strlen("DPTH"), "DPTH");
    }
}

}