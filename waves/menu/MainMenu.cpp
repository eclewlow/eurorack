/*
 ==============================================================================
 
 MainMenu.cpp
 Created: 7 Nov 2023 1:30:39pm
 Author:  Eugene Clewlow
 
 ==============================================================================
 */

#include "Display.h"
#include "graphics.h"
#include "Globals.h"

MainMenu::MainMenu() {
    setState(MAIN_WAVE_DESIGN);
}

MainMenu::~MainMenu() {
    
}

bool MainMenu::handleKeyRelease(int key) {
    //    MAIN_WAVE_DESIGN,
    //    MODE_SELECT,
    //    FX_MANAGEMENT,
    //    SUBOSCILLATOR_CONFIG,
    //    PLAYBACK_MONITORING,
    //    SNAPSHOTS_LIST,
    //    WAVETABLE_MANAGEMENT,
    //    MODULE_SETUP_CONFIG
    
    if(key == LEFT_ENCODER_CCW) {
        currentState = std::clamp<int8_t>(currentState - 1, MAIN_WAVE_DESIGN, MODULE_SETUP_CONFIG);
    }
    if(key == LEFT_ENCODER_CW) {
        currentState = std::clamp<int8_t>(currentState + 1, MAIN_WAVE_DESIGN, MODULE_SETUP_CONFIG);
    }
    //    printf("%d, %d\n", key, juce::KeyPress::returnKey, currentState, );
    if(key == LEFT_ENCODER_CLICK) {
        switch(currentState) {
            case MODE_SELECT:
                context.setState(&modeMenu);
                break;
            case PLAYBACK_MONITORING:
                context.setState(&oscilloscope);
                oscilloscope.setBackMenu(this);
                break;
            case FX_MANAGEMENT:
                context.setState(&fxMenu);
                break;
            case MAIN_WAVE_DESIGN:
                if(context.getEngine() == &wavetableEngine) {
                    wavetableModeMenu.setBackMenu(&mainMenu);
                    context.setState(&wavetableModeMenu);
                } else if(context.getEngine() == &abEngine) {
                    abModeMenu.setBackMenu(&mainMenu);
                    context.setState(&abModeMenu);
                } else if(context.getEngine() == &matrixEngine) {
                    abModeMenu.setBackMenu(&mainMenu);
                    context.setState(&matrixMode);
                } else if(context.getEngine() == &drumEngine) {
                    abModeMenu.setBackMenu(&mainMenu);
                    context.setState(&drumMode);
                }
                break;
            case SUBOSCILLATOR_CONFIG:
                context.setState(&subOscillatorMenu);
                break;
            case SNAPSHOTS_LIST:
                context.setState(&snapshotMenu);
                break;
            case WAVETABLE_MANAGEMENT:
                context.setState(&manageMenu);
                break;
            case MODULE_SETUP_CONFIG:
                context.setState(&deviceConfigurationMenu);
                break;
            default:
                break;
        }
    }
    if(key == BACK_BUTTON) {
        setState(MAIN_WAVE_DESIGN);
    }

    return true;
}

void MainMenu::paint(juce::Graphics& g) {
    Display::clear_screen();
    
    int y_offset = 4;
    int x_offset = 64 - (23+2) * 2;
    int row = 0;
    int col = 0;
    char *caption;
    
    switch(currentState) {
        case MAIN_WAVE_DESIGN:
//            caption = (char*)"AB WAVE";
            if(context.getEngine() == &wavetableEngine) {
                caption = (char*)"WAVETABLE";
            } else if(context.getEngine() == &abEngine) {
                caption = (char*)"AB WAVE";
            } else if(context.getEngine() == &matrixEngine) {
                caption = (char*)"MATRIX";
            } else if(context.getEngine() == &drumEngine) {
                caption = (char*)"DRUM";
            }
            break;
        case MODE_SELECT:
            caption = (char*)"MODE";
            break;
        case FX_MANAGEMENT:
            caption = (char*)"FX";
            break;
        case SUBOSCILLATOR_CONFIG:
            caption = (char*)"SUB";
            break;
        case PLAYBACK_MONITORING:
            caption = (char*)"SCOPE";
            break;
        case SNAPSHOTS_LIST:
            caption = (char*)"SNAPSHOT";
            break;
        case WAVETABLE_MANAGEMENT:
            caption = (char*)"MANAGE";
            break;
        case MODULE_SETUP_CONFIG:
            caption = (char*)"CONFIG";
            break;
        default:
            caption = (char*)"";
            break;
    }

    
    Display::put_string_9x9(64-Display::get_string_9x9_width(caption, 2)/2,64-11,strlen(caption),caption, false, 2);
    
    if(false) {}
    else if(context.getEngine() == &abEngine)
        Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_mode_menu_ab);
    else if(context.getEngine() == &wavetableEngine)
        Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_mode_menu_wavetable);
    else if(context.getEngine() == &matrixEngine)
        Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_mode_menu_wave_matrix);
    else if(context.getEngine() == &drumEngine)
        Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_mode_menu_drum);

    col++;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_main_menu_mode_select);
    
    col++;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_main_menu_fx);
    
    col++;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_main_menu_sub);
    
    
    row++;
    col=0;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_main_menu_oscilloscope);
    
    col++;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_main_menu_snapshots);
    
    col++;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_main_menu_wave_management);
    
    col++;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_main_menu_setup);
    
    col = currentState % 4;
    row = currentState / 4;
    Display::invert_rectangle(col*(23+2)+x_offset, row*(22+2)+y_offset, 23, 22);

}
