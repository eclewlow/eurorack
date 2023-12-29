/*
  ==============================================================================

    ControlStatusMenu.cpp
    Created: 14 Nov 2023 4:39:09pm
    Author:  Eugene Clewlow

  ==============================================================================
*/


#include "Display.h"
#include "graphics.h"
#include "Globals.h"

ControlStatusMenu::ControlStatusMenu() {
    setState(CONTROL_STATUS_STATE_NONE);
}

ControlStatusMenu::~ControlStatusMenu() {
    
}

bool ControlStatusMenu::handleKeyRelease(int key) {
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
            case CONTROL_STATUS_STATE_NONE:
                context.setState(&oscilloscope);
                break;
            default:
                break;
        }
    }
    if(key == BACK_BUTTON) {
        switch(currentState) {
            case CONTROL_STATUS_STATE_NONE:
                context.setState(&oscilloscope);
                break;
            default:
                break;
        }
    }

    return true;
}

void ControlStatusMenu::paint(juce::Graphics& g) {
    Display::clear_screen();
    UpdateWaveform();
    
    uint16_t tune = adc.getChannel(0);
    uint16_t fx_amount = adc.getChannel(1);
    uint16_t fx = adc.getChannel(2);
    uint16_t morph = adc.getChannel(3);

    int y_offset;
    int x_offset;
    // 5 gaps + 4 circules
    // 5*g + 4*d = 128
    int radius = 11;
    int gap = (128 - 4 * radius * 2) / 5;

    float angle;

    x_offset = gap + radius;
    y_offset = 22;
    int text_gap = 7;
    Display::LCD_Circle(x_offset, y_offset, radius, true);
    Display::put_string_5x5(x_offset - strlen("TUNE") * 6 / 2, y_offset - radius - text_gap, strlen("TUNE"), "TUNE");
    angle = 2 * M_PI * ((1.0f - tune / 4095.0f) * 300.0f - 60.0f) / 360.0f;
    Display::LCD_Line(x_offset, y_offset, radius * cos(angle) + x_offset, y_offset - radius * sin(angle), true);
    
    x_offset += gap + radius * 2;
    Display::LCD_Circle(x_offset, y_offset, radius, true);
    Display::put_string_5x5(x_offset - strlen("AMT") * 6 / 2, y_offset - radius - text_gap, strlen("AMT"), "AMT");
    angle = 2 * M_PI * ((1.0f - fx_amount / 4095.0f) * 300.0f - 60.0f) / 360.0f;
    Display::LCD_Line(x_offset, y_offset, radius * cos(angle) + x_offset, y_offset - radius * sin(angle), true);

    x_offset += gap + radius * 2;
    Display::LCD_Circle(x_offset, y_offset, radius, true);
    Display::put_string_5x5(x_offset - strlen("FX") * 6 / 2, y_offset - radius - text_gap, strlen("FX"), "FX");
    angle = 2 * M_PI * ((1.0f - fx / 4095.0f) * 300.0f - 60.0f) / 360.0f;
    Display::LCD_Line(x_offset, y_offset, radius * cos(angle) + x_offset, y_offset - radius * sin(angle), true);

    x_offset += gap + radius * 2;
    Display::LCD_Circle(x_offset, y_offset, radius, true);
    Display::put_string_5x5(x_offset - strlen("MORPH") * 6 / 2, y_offset - radius - text_gap, strlen("MORPH"), "MORPH");
    angle = 2 * M_PI * ((1.0f - morph / 4095.0f) * 300.0f - 60.0f) / 360.0f;
    Display::LCD_Line(x_offset, y_offset, radius * cos(angle) + x_offset, y_offset - radius * sin(angle), true);

    int row_gap = 4;
    x_offset = gap;
    Display::outline_rectangle(x_offset, y_offset + radius + row_gap, radius * 2, radius * 2);
    Display::Draw_NWave(x_offset+1, y_offset + radius + row_gap + 1, radius * 2 - 2, radius * 2 - 2, pitch_wavedata_, radius * 2 - 2);

    x_offset += gap + radius * 2;
    Display::outline_rectangle(x_offset, y_offset + radius + row_gap, radius * 2, radius * 2);
    Display::Draw_NWave(x_offset+1, y_offset + radius + row_gap + 1, radius * 2 - 2, radius * 2 - 2, fx_amount_wavedata_, radius * 2 - 2);

    x_offset += gap + radius * 2;
    Display::outline_rectangle(x_offset, y_offset + radius + row_gap, radius * 2, radius * 2);
    Display::Draw_NWave(x_offset+1, y_offset + radius + row_gap + 1, radius * 2 - 2, radius * 2 - 2, fx_wavedata_, radius * 2 - 2);

    x_offset += gap + radius * 2;
    Display::outline_rectangle(x_offset, y_offset + radius + row_gap, radius * 2, radius * 2);
    Display::Draw_NWave(x_offset+1, y_offset + radius + row_gap + 1, radius * 2 - 2, radius * 2 - 2, morph_wavedata_, radius * 2 - 2);

}

void ControlStatusMenu::triggerUpdate(bool back_pressed) {
    ClearWaveform();
}

void ControlStatusMenu::ClearWaveform() {
    for(int i = 0; i < 20; i++) {
        pitch_wavedata_[i] = 2048;
        fx_amount_wavedata_[i] = 2048;
        fx_wavedata_[i] = 2048;
        morph_wavedata_[i] = 2048;
    }
}

// called 48000x per second
void ControlStatusMenu::UpdateWaveform() {
    for(int i = 0; i < 19; i++) {
        pitch_wavedata_[i] = pitch_wavedata_[i+1];
        fx_amount_wavedata_[i] = fx_amount_wavedata_[i+1];
        fx_wavedata_[i] = fx_wavedata_[i+1];
        morph_wavedata_[i] = morph_wavedata_[i+1];
    }
    pitch_wavedata_[19] = adc.getChannelProcessed(Adc::ADC_CHANNEL_PITCH_CV);
    fx_amount_wavedata_[19] = adc.getChannelProcessed(Adc::ADC_CHANNEL_FX_AMOUNT_CV);
    fx_wavedata_[19] = adc.getChannelProcessed(Adc::ADC_CHANNEL_FX_CV);
    morph_wavedata_[19] = adc.getChannelProcessed(Adc::ADC_CHANNEL_MORPH_CV);
}
