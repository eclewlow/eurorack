/*
  ==============================================================================

    WavetableMenu.cpp
    Created: 18 Nov 2023 8:27:59pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

WavetableModeMenu::WavetableModeMenu() {
    wavetable_ = 0;
    wavetable_offset_ = 0;
}

WavetableModeMenu::~WavetableModeMenu() {
    
}

void WavetableModeMenu::triggerUpdate(bool back_pressed) {
    if(wavetable_ < wavetable_offset_) {
        wavetable_offset_ = wavetable_;
    }

    if(wavetable_ > wavetable_offset_ + 2) {
        wavetable_offset_ = wavetable_ - 2;
    }
}

bool WavetableModeMenu::handleKeyRelease(int key) {
    if(key == LEFT_ENCODER_CCW) {
        wavetable_ = std::clamp<int16_t>(wavetable_ - 1, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
        
        if(wavetable_ < wavetable_offset_) {
            wavetable_offset_ = wavetable_;
        }
    }
    if(key == LEFT_ENCODER_CW) {
        wavetable_ = std::clamp<int16_t>(wavetable_ + 1, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
        
        if(wavetable_ > wavetable_offset_ + 2) {
            wavetable_offset_ = wavetable_ - 2;
        }
    }
    if(key == RIGHT_ENCODER_CCW) {
    }
    if(key == RIGHT_ENCODER_CW) {
    }
    if(key == RIGHT_ENCODER_CLICK) {
    }
    if(key == LEFT_ENCODER_CLICK) {
        wavetableEngine.SetWavetable(wavetable_);
    }
    if(key == BACK_BUTTON) {
        if(back_menu_)
            context.setState(back_menu_, true);
        else
            context.setState(&mainMenu);
    }

    return true;
}

void WavetableModeMenu::paint(juce::Graphics& g) {
    Display::clear_screen();

    int y_offset = 5;
    int x_offset = 1 + 2 * 4;

    uint16_t morph = adc.getChannelProcessed(3);

    storage.LoadWaveSample(BUF1, wavetable_, morph * 1.0f / 4095.0f);
    
    y_offset = 0;
    
    Display::outline_rectangle(0, y_offset, 128, 32);
    Display::Draw_Wave(1, y_offset+1, 128-2, 32-2, BUF1);

    
    y_offset += 31;
    
    Display::outline_rectangle(0, y_offset, 128, 4);
    Display::outline_rectangle(1 + 123 * (morph * 1.0f / 4095.0f), y_offset+1, 3, 2);

    y_offset += 7;
    
    int y_shift = 2;
    int bar_height = 3 * 7 + y_shift * 2;
    int y_cursor_offset = ((bar_height - 5) * wavetable_offset_) / (FACTORY_WAVETABLE_COUNT + USER_WAVETABLE_COUNT - 1 - 2);
    
    Display::outline_rectangle(128 - 6 + 1, y_offset + 1 - y_shift + y_cursor_offset, 1, 3);
    Display::invert_rectangle(128 - 6, y_offset - y_shift, 3, bar_height);

    y_offset += 1;
    
    for(int i = 0; i < 3; i++)
    {
        char line[20];
        snprintf(line, 20, "%*d", 2, i + wavetable_offset_ + 1);
        Display::put_string_3x5(2, y_offset + i * 8, strlen(line), line);
        
        char * name = storage.GetWavetable(i + wavetable_offset_)->name;

        char line2[20];
        memset(line2, 0, 20);
        if(name[0] == '\0')
            strncpy(line2, "--------", 8);
        else {
            strncpy(line2, name, 8);
        }

        Display::put_string_5x5(2 + 2 * 3 + 4, y_offset + i * 8, strlen(line2), line2, i+wavetable_offset_ == wavetable_);
    }
}
