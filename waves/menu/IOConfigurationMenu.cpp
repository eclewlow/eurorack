/*
  ==============================================================================

    IOConfiguration.cpp
    Created: 14 Nov 2023 4:42:37pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

IOConfigurationMenu::IOConfigurationMenu() {
    setState(IO_CONFIG_MENU_INPUT);
}

IOConfigurationMenu::~IOConfigurationMenu() {
    
}

bool IOConfigurationMenu::handleKeyRelease(int key) {
//    DISPLAY_MENU_BRIGHTNESS,
//    DISPLAY_MENU_CONTRAST,
//    DISPLAY_MENU_INVERT,
//    DISPLAY_MENU_SCOPE,

    if(key == BACK_BUTTON) {
        context.setState(&deviceConfigurationMenu);
    }
    else if(key == LEFT_ENCODER_CCW) {
        state_ = std::clamp<int8_t>(state_ - 1, IO_CONFIG_MENU_INPUT, IO_CONFIG_MENU_BIAS);
    }
    else if(key == LEFT_ENCODER_CW) {
        state_ = std::clamp<int8_t>(state_ + 1, IO_CONFIG_MENU_INPUT, IO_CONFIG_MENU_BIAS);
    }
    else if(key == RIGHT_ENCODER_CCW) {
        switch(state_) {
            case IO_CONFIG_MENU_INPUT: {
                input_ = (input_ + IO_CONFIG_LAST - 1) % IO_CONFIG_LAST;
                break;
            }
            case IO_CONFIG_MENU_GAIN: {
                user_settings.setIOGain(input_, user_settings.getIOGain(input_) - 0.1f);
                break;
            }
            case IO_CONFIG_MENU_BIAS: {
                user_settings.setIOBias(input_, user_settings.getIOBias(input_) - 0.1f);
                break;
            }
            default:
                break;
        }
    }
    else if(key == RIGHT_ENCODER_CW) {
        switch(state_) {
            case IO_CONFIG_MENU_INPUT: {
                input_ = (input_ + 1) % IO_CONFIG_LAST;
                break;
            }
            case IO_CONFIG_MENU_GAIN: {
                user_settings.setIOGain(input_, user_settings.getIOGain(input_) + 0.1f);
                break;
            }
            case IO_CONFIG_MENU_BIAS: {
                user_settings.setIOBias(input_, user_settings.getIOBias(input_) + 0.1f);
                break;
            }
            default:
                break;
        }
    }
    else if(key == LEFT_ENCODER_CLICK) {
    }
    return true;
}

void IOConfigurationMenu::paint(juce::Graphics& g) {
    Display::clear_screen();

    UpdateWaveform();
    /****/
    // draw IO
    /****/
    int x_offset = (64) / 2;
    int y_offset = 10;
    Display::put_string_9x9(x_offset - Display::get_string_9x9_width("IO", 2) / 2, y_offset, strlen("IO"), "IO", false, 2);
    
    /****/
    // draw left side items
    /****/
    x_offset = 0;
    y_offset += 16;
    Display::put_string_5x5(x_offset, y_offset, strlen("INPUT:"), "INPUT:");
    char *line;
    switch(input_) {
        case IO_CONFIG_PITCH:
            line = (char* ) "PITCH";
            break;
        case IO_CONFIG_FX_AMOUNT:
            line = (char* ) "FX AMT";
            break;
        case IO_CONFIG_FX:
            line = (char* ) "FX";
            break;
        case IO_CONFIG_MORPH:
            line = (char* ) "MORPH";
            break;
        default:
            break;
    }
    Display::put_string_5x5(x_offset + strlen("INPUT:") * 6, y_offset, strlen(line), line, state_ == IO_CONFIG_MENU_INPUT);

    y_offset += 7;
    Display::put_string_5x5(x_offset+6, y_offset, strlen("GAIN:"), "GAIN:");
    char line2[20];
    snprintf(line2, 20, "%.1fX", user_settings.getIOGain(input_));
    Display::put_string_5x5(x_offset + strlen("GAIN:") * 6 + 6, y_offset, strlen(line2), line2, state_ == IO_CONFIG_MENU_GAIN);

    
    y_offset += 7;
    Display::put_string_5x5(x_offset+6, y_offset, strlen("BIAS:"), "BIAS:");
    snprintf(line2, 20, "%.2f", user_settings.getIOBias(input_));
    Display::put_string_5x5(x_offset + strlen("BIAS:") * 6 + 6, y_offset, strlen(line2), line2, state_ == IO_CONFIG_MENU_BIAS);

    /****/
    // draw graph
    /****/
    x_offset = 64 + 14;
    int line_height = 56;
    int line_spacing = 24;
    Display::LCD_Line(x_offset, 32 - line_height / 2, x_offset, 32 + line_height / 2, true);

    Display::LCD_Line(x_offset, 32, x_offset - 2, 32, true);
    Display::LCD_DottedLine(x_offset, 32, 127, 32, 1, 1, true);

    Display::LCD_Line(x_offset, 32 - line_spacing, x_offset - 2, 32 - line_spacing, true);
    Display::LCD_DottedLine(x_offset, 32 - line_spacing, 127, 32 - line_spacing, 1, 1, true);
    Display::put_string_5x5(x_offset - 3 - strlen("+10") * 6, 32 - line_spacing - 5 / 2, strlen("+10"), "+10");
    
    Display::LCD_Line(x_offset, 32 + line_spacing, x_offset - 2, 32 + line_spacing, true);
    Display::LCD_DottedLine(x_offset, 32 + line_spacing, 127, 32 + line_spacing, 2, 2, true);
    Display::put_string_5x5(x_offset - 3 - strlen("-10") * 6, 32 + line_spacing - 5 / 2, strlen("-10"), "-10");

    Display::LCD_Line(x_offset + 2, 32 - line_spacing / 2, x_offset - 2, 32 - line_spacing / 2, true);
    Display::LCD_Line(x_offset + 2, 32 + line_spacing / 2, x_offset - 2, 32 + line_spacing / 2, true);

    int graph_width = 128 - x_offset;
    Display::Draw_NWave(x_offset, 32 - line_spacing + 1, 50, 2 * line_spacing, wavedata_, 4095);
}

void IOConfigurationMenu::triggerUpdate(bool back_pressed) {
    ClearWaveform();
}


void IOConfigurationMenu::ClearWaveform() {
    for(int i = 0; i < 50; i++)
        wavedata_[i] = 2048;
}

// called 48000x per second
void IOConfigurationMenu::UpdateWaveform() {
    for(int i = 0; i < 49; i++) {
        wavedata_[i] = wavedata_[i+1];
    }
    uint16_t channel;
    channel = adc.getChannelProcessed(input_ + 4);
    wavedata_[49] = channel;
}
