/*
  ==============================================================================

    DisplayMenu.cpp
    Created: 14 Nov 2023 4:41:08pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

namespace waves {


DisplayConfigurationMenu::DisplayConfigurationMenu() {
    setState(DISPLAY_MENU_BRIGHTNESS);
    brightness = 100;
    contrast = 100;
    invert = false;
    scope_setting = SETTING_SCOPE_LINE;
    morph_setting = SETTING_MORPH_SMOOTH;
}

DisplayConfigurationMenu::~DisplayConfigurationMenu() {
    
}

bool DisplayConfigurationMenu::handleKeyRelease(int key) {
//    DISPLAY_MENU_BRIGHTNESS,
//    DISPLAY_MENU_CONTRAST,
//    DISPLAY_MENU_INVERT,
//    DISPLAY_MENU_SCOPE,

    if(key == BACK_BUTTON) {
        context.setState(&deviceConfigurationMenu);
    }
    else if(key == LEFT_ENCODER_CCW) {
        switch(state_) {
            case DISPLAY_MENU_BRIGHTNESS:
                setBrightness(getBrightness()-1);
                break;
            case DISPLAY_MENU_CONTRAST:
                setContrast(getContrast()-1);
                break;
            case DISPLAY_MENU_INVERT:
                setInvert(!getInvert());
                break;
            case DISPLAY_MENU_SCOPE:
                setScopeSetting(getScopeSetting() == SETTING_SCOPE_LINE ? SETTING_SCOPE_FILL: SETTING_SCOPE_LINE);
                break;
            default:
                break;
        }
    }
    else if(key == LEFT_ENCODER_CW) {
        switch(state_) {
            case DISPLAY_MENU_BRIGHTNESS:
                setBrightness(getBrightness()+1);
                break;
            case DISPLAY_MENU_CONTRAST:
                setContrast(getContrast()+1);
                break;
            case DISPLAY_MENU_INVERT:
                setInvert(!getInvert());
                break;
            case DISPLAY_MENU_SCOPE:
                setScopeSetting(getScopeSetting() == SETTING_SCOPE_LINE ? SETTING_SCOPE_FILL: SETTING_SCOPE_LINE);
                break;
            default:
                break;
        }
    }
    else if(key == LEFT_ENCODER_CLICK) {
        switch(state_) {
            case DISPLAY_MENU_BRIGHTNESS:
                setState(DISPLAY_MENU_CONTRAST);
                break;
            case DISPLAY_MENU_CONTRAST:
                setState(DISPLAY_MENU_INVERT);
                break;
            case DISPLAY_MENU_INVERT:
                setState(DISPLAY_MENU_SCOPE);
                break;
            case DISPLAY_MENU_SCOPE:
                setState(DISPLAY_MENU_BRIGHTNESS);
                break;
            default:
                break;
        }
    }
    return true;
}

void DisplayConfigurationMenu::paint() {
    Display::clear_screen();
    
    int y_offset = 5;
    
    Display::put_string_9x9(128 / 2 - Display::get_string_9x9_width("DISPLAY", 3) / 2, y_offset, strlen("DISPLAY"), "DISPLAY", false, 3);

    y_offset += 10 + 5;
    
    char line[20];

    // int wavetable_count = 32;
    // int snapshot_count = 4;
    
    int center = 128 / 2 - 6 / 2;
//    snprintf(line, 20, "   VERSION; %s", "0.07");
    Display::put_string_5x5(center, y_offset, 1, ":");
    Display::put_string_5x5(center - strlen("BRIGHTNESS") * 6, y_offset, strlen("BRIGHTNESS"), "BRIGHTNESS");
    snprintf(line, 20, "%d", getBrightness());
    Display::put_string_5x5(center + 6, y_offset, strlen(line), line, state_==DISPLAY_MENU_BRIGHTNESS);

    y_offset += 8;
    Display::put_string_5x5(center, y_offset, 1, ":");
    Display::put_string_5x5(center - strlen("CONTRAST") * 6, y_offset, strlen("CONTRAST"), "CONTRAST");
    snprintf(line, 20, "%d", getContrast());
    Display::put_string_5x5(center + 6, y_offset, strlen(line), line, state_==DISPLAY_MENU_CONTRAST);

    y_offset += 8;
    Display::put_string_5x5(center, y_offset, 1, ":");
    Display::put_string_5x5(center - strlen("INVERT") * 6, y_offset, strlen("INVERT"), "INVERT");
    snprintf(line, 20, "%s", getInvert() ? "ON" : "OFF");
    Display::put_string_5x5(center + 6, y_offset, strlen(line), line, state_==DISPLAY_MENU_INVERT);

    y_offset += 8;
    Display::put_string_5x5(center, y_offset, 1, ":");
    Display::put_string_5x5(center - strlen("SCOPE") * 6, y_offset, strlen("SCOPE"), "SCOPE");
    snprintf(line, 20, "%s", getScopeSetting() == SETTING_SCOPE_LINE ? "LINE" : "FILL");
    Display::put_string_5x5(center + 6, y_offset, strlen(line), line, state_==DISPLAY_MENU_SCOPE);
}

}
