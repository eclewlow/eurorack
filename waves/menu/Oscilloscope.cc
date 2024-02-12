/*
  ==============================================================================

    Oscilloscope.cpp
    Created: 13 Nov 2023 6:13:45pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

namespace waves {

Oscilloscope::Oscilloscope() {
    setState(OSCILLOSCOPE_STATE_NONE);
}

Oscilloscope::~Oscilloscope() {
    
}

bool Oscilloscope::handleKeyRelease(int key) {
    if(key == LEFT_ENCODER_CCW) {
        switch(currentState) {
            default:
                break;
        }
    }
    if(key == LEFT_ENCODER_CW) {
        switch(currentState) {
            default:
                break;
        }
    }
    if(key == LEFT_ENCODER_CLICK) {
        switch(currentState) {
            case OSCILLOSCOPE_STATE_NONE:
                context.setState(&controlStatusMenu);
//                context.setState(&modeMenu);
                break;
            default:
                break;
        }
    }
    if(key == BACK_BUTTON) {
        switch(currentState) {
            case OSCILLOSCOPE_STATE_NONE:
                if(back_menu_)
                    context.setState(back_menu_, true);
                else
                    context.setState(&mainMenu);
                break;
            default:
                break;
        }
    }

    return true;
}

void Oscilloscope::paint() {
    Display::clear_screen();
    
    int y_offset;
    int x_offset;
    
    uint16_t tune = adc.getChannelProcessed(0);
    uint16_t fx_amount = adc.getChannelProcessed(1);
    uint16_t fx = adc.getChannelProcessed(2);
    uint16_t morph = adc.getChannelProcessed(3);
    
    context.getEngine()->FillWaveform(BUF1, tune,  fx_amount,  fx,  morph);

//    uint8_t note = static_cast<uint8_t>((120.0f * tune)/4095.0);
    float note = tune * user_settings.getCalibrationX() + user_settings.getCalibrationY();
    
    note = quantizer.Quantize(note);
    
    float a = 440; //frequency of A (coomon value is 440Hz)
    float frequency = (a / 32) * pow(2, ((note - 9) / 12.0));
    
    int x = 0;
    int width = 128;
    int y = 2;
    int height = 40;
    Display::outline_rectangle(x, y, width, height);
    
    Display::Draw_Wave(x+1, y+1, width-2, height-2, BUF1, user_settings.getScopeSetting() == UserSettings::SETTING_SCOPE_FILL);
    
    std::string note_letter = "C C#D D#E F F#G G#A A#B ";
    int note_index = static_cast<int16_t>(floor(note + 0.5)) % 12;

    char first_line[20];
    snprintf(first_line, 20, "TUNE;%d%s, %5.0fHZ", (static_cast<int16_t>(floor(note + 0.5)) / 12 - 1), note_letter.substr(note_index * 2, 2).c_str(), frequency);
    
    x_offset = 5;
    y_offset = y + height + 3;
    Display::put_string_5x5(x_offset, y_offset, strlen(first_line), first_line);

    x_offset = 5;
    y_offset += 7;

    char second_line[20];
    uint32_t currentTime = system_clock.milliseconds();
    unsigned int hours = (currentTime / (1000*60*60)) % 24;
    unsigned int minutes = (currentTime / (1000*60)) % 60;
    unsigned int seconds = (currentTime / (1000)) % 60;
    unsigned int milliseconds = (currentTime / 100) % 10;
        
    snprintf(second_line, 20, "TIME;%02d;%02d;%02d.%01d", hours, minutes, seconds, milliseconds);

    Display::put_string_5x5(x_offset, y_offset, strlen(second_line), second_line);
}

}
