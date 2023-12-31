/*
  ==============================================================================

    MatrixMode.cpp
    Created: 29 Nov 2023 12:27:07am
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

MatrixMode::MatrixMode() {
    edit_state_ = MATRIX_MODE_EDIT_DEFAULT;
    editing_offset_ = false;
}

MatrixMode::~MatrixMode() {
    
}

void MatrixMode::triggerUpdate(bool back_pressed) {
    editing_offset_ = false;
    edit_state_ = MATRIX_MODE_EDIT_DEFAULT;
}

bool MatrixMode::handleKeyRelease(int key) {
    if(key == LEFT_ENCODER_CCW) {
        switch(edit_state_) {
            case MATRIX_MODE_EDIT_DEFAULT:
                if(matrixEngine.GetX1() > 0) {
                    matrixEngine.IncrementX1(-1);
                    matrixEngine.IncrementX2(-1);
                }
                break;
            case MATRIX_MODE_EDIT_TOPLEFT:
                matrixEngine.IncrementX1(-1);
                break;
            case MATRIX_MODE_EDIT_BOTTOMRIGHT:
                matrixEngine.IncrementX2(-1);
                break;
            default:
                break;
        }
    }
    if(key == LEFT_ENCODER_CW) {
        switch(edit_state_) {
            case MATRIX_MODE_EDIT_DEFAULT:
                if(matrixEngine.GetX2() < 15) {
                    matrixEngine.IncrementX2(1);
                    matrixEngine.IncrementX1(1);
                }
                break;
            case MATRIX_MODE_EDIT_TOPLEFT:
                matrixEngine.IncrementX1(1);
                break;
            case MATRIX_MODE_EDIT_BOTTOMRIGHT:
                matrixEngine.IncrementX2(1);
                break;
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CCW) {
        switch(edit_state_) {
            case MATRIX_MODE_EDIT_DEFAULT:
                if(matrixEngine.GetY1() > 0) {
                    matrixEngine.IncrementY1(-1);
                    matrixEngine.IncrementY2(-1);
                }
                break;
            case MATRIX_MODE_EDIT_TOPLEFT:
                matrixEngine.IncrementY1(-1);
                break;
            case MATRIX_MODE_EDIT_BOTTOMRIGHT:
                matrixEngine.IncrementY2(-1);
                break;
            case MATRIX_MODE_EDIT_OFFSET: {
                int8_t new_offset = matrixEngine.GetWavelistOffset() - 1;
                if (new_offset < 0)
                    new_offset = 0;
                matrixEngine.SetWavelistOffset(new_offset);
                break;
            }
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CW) {
        switch(edit_state_) {
            case MATRIX_MODE_EDIT_DEFAULT:
                if(matrixEngine.GetY2() < 15) {
                    matrixEngine.IncrementY2(1);
                    matrixEngine.IncrementY1(1);
                }
                break;
            case MATRIX_MODE_EDIT_TOPLEFT:
                matrixEngine.IncrementY1(1);
                break;
            case MATRIX_MODE_EDIT_BOTTOMRIGHT:
                matrixEngine.IncrementY2(1);
                break;
            case MATRIX_MODE_EDIT_OFFSET: {
                int8_t new_offset = matrixEngine.GetWavelistOffset() + 1;
                if (new_offset > FACTORY_WAVETABLE_COUNT + USER_WAVETABLE_COUNT - 1 - 16)
                    new_offset = FACTORY_WAVETABLE_COUNT + USER_WAVETABLE_COUNT - 1 - 16;
                matrixEngine.SetWavelistOffset(new_offset);
                break;
            }
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CLICK) {
        edit_state_ = edit_state_ == MATRIX_MODE_EDIT_OFFSET ? MATRIX_MODE_EDIT_DEFAULT : MATRIX_MODE_EDIT_OFFSET;
    }
    if(key == LEFT_ENCODER_CLICK) {
        edit_state_ = edit_state_ == MATRIX_MODE_EDIT_OFFSET ? MATRIX_MODE_EDIT_BOTTOMRIGHT : edit_state_;
        edit_state_ = (edit_state_ + 1) % (MATRIX_MODE_EDIT_LAST);
    }
    if(key == BACK_BUTTON) {
        if(back_menu_)
            context.setState(back_menu_, true);
        else
            context.setState(&mainMenu);
    }

    return true;
}

void MatrixMode::paint(juce::Graphics& g) {
    Display::clear_screen();
    
    uint16_t tune = adc.getChannelProcessed(0);
    uint16_t fx_amount = adc.getChannelProcessed(1);
    uint16_t fx = adc.getChannelProcessed(2);
    uint16_t morph = adc.getChannelProcessed(3);
    
    /****/
    // draw grid corners
    /****/
    int grid_offset_x = 2;
    int grid_offset_y = 2;
    
    for(int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            Display::Put_Pixel(grid_offset_x + i * 4, grid_offset_y + j * 4, true);
        }
    }
    
    /****/
    // draw outline
    /****/
    int16_t outline_x1 = matrixEngine.GetX1() * 4;
    int16_t outline_y1 = matrixEngine.GetY1() * 4;
    int16_t outline_x2 = matrixEngine.GetX2() * 4 + 4;
    int16_t outline_y2 = matrixEngine.GetY2() * 4 + 4;
    int16_t outline_width = outline_x2 - outline_x1 + 1;
    int16_t outline_height = outline_y2 - outline_y1 + 1;
    Display::outline_rectangle(outline_x1, outline_y1, outline_width, outline_height);

    /****/
    // draw 4 corners
    /****/
    // top left
    if(edit_state_ == MATRIX_MODE_EDIT_DEFAULT || edit_state_ == MATRIX_MODE_EDIT_TOPLEFT)
        Display::outline_rectangle(outline_x1 - 1, outline_y1 - 1, 3, 3);
    // top right
    if(edit_state_ == MATRIX_MODE_EDIT_DEFAULT)
        Display::outline_rectangle(outline_x2 - 1, outline_y1 - 1, 3, 3);
    // bottom left
    if(edit_state_ == MATRIX_MODE_EDIT_DEFAULT)
        Display::outline_rectangle(outline_x1 - 1, outline_y2 - 1, 3, 3);
    // bottom right
    if(edit_state_ == MATRIX_MODE_EDIT_DEFAULT || edit_state_ == MATRIX_MODE_EDIT_BOTTOMRIGHT)
        Display::outline_rectangle(outline_x2 - 1, outline_y2 - 1, 3, 3);

    /****/
    // draw cursor
    /****/
    int16_t cursor_x = 4 * matrixEngine.GetX1() + 4 * (matrixEngine.GetX2() - matrixEngine.GetX1()) * fx * 1.0f / 4095.0f;
    int16_t cursor_y = 4 * matrixEngine.GetY1() + 4 * (matrixEngine.GetY2() - matrixEngine.GetY1()) * morph * 1.0f / 4095.0f;
    Display::outline_rectangle(cursor_x + 1, cursor_y + 1, 3, 3);
    

    /****/
    // draw wave
    /****/
    int x_offset = grid_offset_x + 15 * 4 + 6;
    Display::outline_rectangle(x_offset, 0, 128 - x_offset, 32);
    
    matrixEngine.FillWaveform(BUF1, tune, fx_amount, fx, morph, false);
    Display::Draw_Wave(x_offset + 1, 0 + 1, 128 - x_offset - 2, 32 - 2, BUF1);

    /****/
    // draw coordinates
    /****/
    char line[20];
    memset(line, 0, 20);
    
    snprintf(line, 20, "X;%.1f", 1.0 + matrixEngine.GetX1() + (matrixEngine.GetX2() - matrixEngine.GetX1()) * fx * 1.0f / 4095.0f);
    Display::put_string_5x5(x_offset + 1, 35, strlen(line), line);

    snprintf(line, 20, "Y;%.1f", 1.0 + matrixEngine.GetY1() + (matrixEngine.GetY2() - matrixEngine.GetY1()) * morph * 1.0f / 4095.0f);
    Display::put_string_5x5(x_offset + 1, 35 + 7, strlen(line), line);

    strncpy(line, "OFS;  /16", 20);
    Display::put_string_5x5(x_offset + 1, 35 + 7 + 12, strlen(line), line);

    snprintf(line, 20, "%*d", 2, matrixEngine.GetWavelistOffset());
    Display::put_string_5x5(x_offset + 1 + 4 * 6, 35 + 7 + 12, strlen(line), line, edit_state_ == MATRIX_MODE_EDIT_OFFSET);
}
