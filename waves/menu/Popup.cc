/*
  ==============================================================================

    Popup.cpp
    Created: 28 Nov 2023 9:20:46pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

Popup::Popup() {
    is_showing_ = false;
    timer_length_ = 2000;
    timer_ = system_clock.milliseconds();
}

Popup::~Popup() {
    
}

bool Popup::handleKeyRelease(int key) {
    if(!is_showing_) {
        return false;
    }

    if(key == LEFT_ENCODER_CCW) {
    }
    if(key == LEFT_ENCODER_CW) {
    }
    if(key == LEFT_ENCODER_CLICK) {
    }
    if(key == BACK_BUTTON) {
    }
    return true;
}

void Popup::show(int32_t timer_length) {
    timer_length_ = timer_length;
    timer_ = system_clock.milliseconds();
    is_showing_ = true;
}
void Popup::hide() {
    is_showing_ = false;
    timer_length_ = 0;
    timer_ = 0;
}

void Popup::SetLine(int line_no, char* str) {
    char* line = lines_[line_no];
    memset(line, 0, 20);
    strncpy(line, str, strlen(str));
}

void Popup::paint(juce::Graphics& g) {
    int width = 112;
    int height = 32;
    int x = 64 - width / 2;
    int y = 32 - height / 2;
    
    if(!is_showing_) {
        return;
    }

    uint32_t elapsed_time = system_clock.milliseconds() - timer_;
    
    if(elapsed_time > timer_length_) {
        hide();
        return;
    }
    
    Display::clear_rectangle_simple(x, y, width, height);
    Display::outline_rectangle(x, y, width, height);
    
    int line_count = 0;
    for(int i = 0; i < 3; i++) {
        if(strlen(lines_[i]) > 0)
            line_count++;
    }
    
    int y_offset = 32 - line_count * 3;
    
    for(int i = 0; i < 3; i++) {
        if(strlen(lines_[i]) > 0) {
            int x_offset = 64 - 3 * strlen(lines_[i]);
            Display::put_string_5x5(x_offset, y_offset, strlen(lines_[i]), lines_[i]);
        }
        y_offset += 6;
    }
}
