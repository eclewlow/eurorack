/*
 ==============================================================================
 
 ModeMenu.cpp
 Created: 7 Nov 2023 4:54:56pm
 Author:  Eugene Clewlow
 
 ==============================================================================
 */

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

ModeMenu::ModeMenu() {
    setState(MODE_AB);
}

ModeMenu::~ModeMenu() {
    
}

bool ModeMenu::handleKeyRelease(int key) {
    //    MODE_AB,
    //    MODE_WAVETABLE,
    //    MODE_MATRIX,
    //    MODE_DRUM,
    if(key == LEFT_ENCODER_CCW) {
        currentState = std::clamp<int8_t>(currentState - 1, MODE_AB, MODE_DRUM);
    }
    if(key == LEFT_ENCODER_CW) {
        currentState = std::clamp<int8_t>(currentState + 1, MODE_AB, MODE_DRUM);
    }
    if(key == LEFT_ENCODER_CLICK) {
        switch(currentState) {
            case MODE_AB:
                abModeMenu.setBackMenu(&modeMenu);
                context.setState(&abModeMenu);
                context.setEngine(Context::ENGINE_TYPE_AB);
                break;
            case MODE_WAVETABLE:
                wavetableModeMenu.setBackMenu(&modeMenu);
                context.setState(&wavetableModeMenu);
                context.setEngine(Context::ENGINE_TYPE_WAVETABLE);
                break;
            case MODE_MATRIX:
                matrixMode.setBackMenu(&modeMenu);
                context.setState(&matrixMode);
                context.setEngine(Context::ENGINE_TYPE_MATRIX);
                break;
            case MODE_DRUM:
                drumMode.setBackMenu(&modeMenu);
                context.setState(&drumMode);
                context.setEngine(Context::ENGINE_TYPE_DRUM);
                break;
            default:
                break;
                
        }
    }
    if(key == BACK_BUTTON) {
        context.setState(&mainMenu);
    }
    
    return true;
}

void ModeMenu::paint(juce::Graphics& g) {
    Display::clear_screen();
    
    int y_offset = 32 - (22+2) / 2;
    int x_offset = 64 - (23+2) * 2;
    int row = 0;
    int col = 0;
    char *caption;
    
    //    MODE_AB,
    //    MODE_WAVETABLE,
    //    MODE_MATRIX,
    //    MODE_DRUM,
    
    switch(currentState) {
        case MODE_AB:
            caption = (char*)"AB WAVE";
            break;
        case MODE_WAVETABLE:
            caption = (char*)"WAVETABLE";
            break;
        case MODE_MATRIX:
            caption = (char*)"MATRIX";
            break;
        case MODE_DRUM:
            caption = (char*)"DRUM";
            break;
        default:
            break;
    }
    
    Display::put_string_9x9(64-Display::get_string_9x9_width("MODE", 3)/2,5,strlen("MODE"),"MODE", false, 3);
    
    Display::put_string_9x9(64-Display::get_string_9x9_width(caption, 2)/2,64-13,strlen(caption),caption, false, 2);
    
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_mode_menu_ab);
    
    col++;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_mode_menu_wavetable);
    
    col++;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_mode_menu_wave_matrix);
    
    col++;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_mode_menu_drum);
    
    col = currentState;
    row = 0;
    Display::invert_rectangle(col*(23+2)+x_offset, row*(22+2)+y_offset, 23, 22);
}
