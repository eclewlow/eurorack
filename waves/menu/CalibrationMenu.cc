/*
  ==============================================================================

    CalibrationMenu.cpp
    Created: 14 Nov 2023 4:42:55pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"
#include <string>

namespace waves {


CalibrationMenu::CalibrationMenu() {
//    setState(IO_CONFIG_MENU_INPUT);
    setState(CALIBRATION_MENU_STATE_1V);
    message_displayed_ = false;
    c1_value_ = 2457;
    c5_value_ = 4095;
}

CalibrationMenu::~CalibrationMenu() {
    
}

bool CalibrationMenu::handleKeyRelease(int key) {
//    DISPLAY_MENU_BRIGHTNESS,
//    DISPLAY_MENU_CONTRAST,
//    DISPLAY_MENU_INVERT,
//    DISPLAY_MENU_SCOPE,

    if(key == BACK_BUTTON) {
        context.setState(&deviceConfigurationMenu);
    }
    else if(key == LEFT_ENCODER_CCW) {
    }
    else if(key == LEFT_ENCODER_CW) {
    }
    else if(key == RIGHT_ENCODER_CLICK) {
        if(state_ == CALIBRATION_MENU_STATE_1V && message_displayed_ == false) {
            message_displayed_ = true;
            timer_ = system_clock.milliseconds();
            c1_value_ = adc.getChannel(Adc::ADC_CHANNEL_PITCH_CV);
        } else if(state_ == CALIBRATION_MENU_STATE_5V && message_displayed_ == false) {
            message_displayed_ = true;
            timer_ = system_clock.milliseconds();
            c5_value_ = adc.getChannel(Adc::ADC_CHANNEL_PITCH_CV);
            // user_settings.Calibrate(c1_value_, c5_value_);
            abEngine.Calibrate(c1_value_, c5_value_);
        } else if(state_ == CALIBRATION_MENU_STATE_DONE) {
            message_displayed_ = false;
            timer_ = system_clock.milliseconds();
            setState(CALIBRATION_MENU_STATE_1V);
        }
    }
    else if(key == LEFT_ENCODER_CLICK) {
    }
    return true;
}

void CalibrationMenu::paint() {
    Display::clear_screen();

    int x_offset;
    int y_offset;

    uint16_t tune = adc.getChannel(Adc::ADC_CHANNEL_PITCH_CV);
    
    x_offset = 128 - 6 * 6;
    y_offset = 10;
    Display::put_string_5x5(x_offset + (128 - x_offset) / 2 - strlen("INPUT") * 6 / 2, y_offset, strlen("INPUT"), "INPUT");
    
    y_offset += 6;
    char line[20];
    snprintf(line, 20, "%1.3fV", (tune / 65535.0f) * 8.0f);
    Display::put_string_5x5(128 - strlen(line) * 6, y_offset, strlen(line), line);

    
    // float note = tune * user_settings.getCalibrationX() + user_settings.getCalibrationY();
    float note = tune * abEngine.getCalibrationX() + abEngine.getCalibrationY();
    
    note = clamp(note, 0.0f, 120.0f);

    // float a = 440; //frequency of A (coomon value is 440Hz)
    // float frequency = (a / 32) * pow(2, ((note - 9) / 12.0));
    
    std::string note_letter = "C C#D D#E F F#G G#A A#B ";
    int note_index = static_cast<int16_t>(floor(note + 0.5)) % 12;

    int octave = static_cast<int16_t>(floor(note + 0.5)) / 12 - 1;
    const char * note_text = note_letter.substr(note_index * 2, 2).c_str();
    
    y_offset = 10;
    if(message_displayed_) {
        note_text = "C ";
        if(state_ == CALIBRATION_MENU_STATE_1V)
            octave = 1;
        else if(state_ == CALIBRATION_MENU_STATE_5V)
            octave = 5;
    }
    Display::put_string_9x9(64 - Display::get_string_9x9_width(note_text, 2) / 2, y_offset, strlen(note_text), note_text, false, 2);
    snprintf(line, 20, "OCT:%d", octave);
    y_offset += 10 + 2;
    Display::put_string_5x5(64 - strlen(line) * 6 / 2, y_offset, strlen(line), line);
    /****/
    // draw graph
    /****/
//    x_offset = 12;
    // int line_height = 56;
    // int line_spacing = 24;
    int line_width = 100;
//    Display::LCD_Line(x_offset, 32 - line_height / 2, x_offset, 32 + line_height / 2, true);
//
    y_offset = 42;
    Display::LCD_Line(63 - line_width / 2, y_offset, 63 + line_width / 2, y_offset, true);
    
    for(int i = 0; i < 11; i++) {
        x_offset = 63 - line_width / 2 + line_width * (i / 10.0f);
        if(i == 0 || i == 10 || i == 5)
            Display::LCD_Line(x_offset, y_offset - 2, x_offset, y_offset + 2, true);
        else
            Display::LCD_Line(x_offset, y_offset - 1, x_offset, y_offset + 1, true);
    }
    
    int note_integral = floor(note + 0.5); // round to nearest int
    float note_fractional = note - note_integral; // -0.5 to 0.4999

    // draw note cursor
    if(message_displayed_) {
        x_offset = 63 - line_width / 2 + line_width * (0.5);
    } else {
        x_offset = 63 - line_width / 2 + line_width * (note_fractional + 0.5);
    }
    Display::LCD_Line(x_offset-1, y_offset - 3, x_offset-1, y_offset + 3, true);
    Display::LCD_Line(x_offset, y_offset - 3, x_offset, y_offset + 3, true);
    Display::LCD_Line(x_offset+1, y_offset - 3, x_offset+1, y_offset + 3, true);

    x_offset = 63 - line_width / 2 + line_width * (0 / 10.0f);
    y_offset = y_offset - 3 - 8;
    strncpy(line, "-50", 20);
    Display::put_string_5x5(x_offset - strlen(line) * 6 / 2 + 1, y_offset, strlen(line), line);

    x_offset = 63 - line_width / 2 + line_width * (10.0f / 10.0f);
    strncpy(line, "+50", 20);
    Display::put_string_5x5(x_offset - strlen(line) * 6 / 2 + 1, y_offset, strlen(line), line);

    x_offset = 63 - line_width / 2 + line_width * (5.0f / 10.0f);
    strncpy(line, "0", 20);
    Display::put_string_5x5(x_offset - strlen(line) * 6 / 2 + 1, y_offset, strlen(line), line);
    
    y_offset += 8 + 6 + 3 + 4;
    uint32_t elapsed_time = system_clock.milliseconds() - timer_;
    if(elapsed_time > 2000 && message_displayed_ == true) {
        message_displayed_ = false;
        if(state_ == CALIBRATION_MENU_STATE_1V)
            state_ = CALIBRATION_MENU_STATE_5V;
        else if(state_ == CALIBRATION_MENU_STATE_5V)
            state_ = CALIBRATION_MENU_STATE_DONE;
    }
    if(message_displayed_) {
        if(state_ == CALIBRATION_MENU_STATE_1V)
            strncpy(line, "1.000V", 20);
        else
            strncpy(line, "5.000V", 20);
        Display::put_string_9x9(63 - Display::get_string_9x9_width(line, 2) / 2, y_offset, strlen(line), line, false, 2);
    } else {
        if(state_ == CALIBRATION_MENU_STATE_1V)
            strncpy(line, "PRESS R ENC FOR C1", 20);
        else if(state_ == CALIBRATION_MENU_STATE_5V)
            strncpy(line, "PRESS R ENC FOR C5", 20);
        else
            strncpy(line, "DONE CALIBRATING!", 20);
        Display::put_string_5x5(63 - strlen(line) * 6 / 2, y_offset + 3, strlen(line), line);
    }
}

void CalibrationMenu::triggerUpdate(bool back_pressed) {
}

}
