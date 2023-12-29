/*
  ==============================================================================

    InfoMenu.cpp
    Created: 14 Nov 2023 4:39:44pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

InfoMenu::InfoMenu() {
    setState(INFO_MENU_NONE);
}

InfoMenu::~InfoMenu() {
    
}

bool InfoMenu::handleKeyRelease(int key) {
    if(key == BACK_BUTTON) {
        context.setState(&deviceConfigurationMenu);
    } else if(key == LEFT_ENCODER_CLICK) {
        context.setState(&deviceConfigurationMenu);
    }
    return true;
}

void InfoMenu::paint(juce::Graphics& g) {
    Display::clear_screen();
    
    int y_offset = 5;
    
    Display::put_string_9x9(128 / 2 - Display::get_string_9x9_width("INFO", 3) / 2, y_offset, strlen("INFO"), "INFO", false, 3);

    y_offset += 10 + 5;
    
    char line[20];

    int wavetable_count = 32;
    int snapshot_count = 4;
    
    int center = 128 / 2 - 6 / 2;
//    snprintf(line, 20, "   VERSION; %s", "0.07");
    Display::put_string_5x5(center, y_offset, 1, ";");
    Display::put_string_5x5(center - strlen("VERSION") * 6, y_offset, strlen("VERSION"), "VERSION");
    snprintf(line, 20, "%s", "0.07");
    Display::put_string_5x5(center + 2 * 6, y_offset, strlen(line), line);

    y_offset += 8;
    Display::put_string_5x5(center, y_offset, 1, ";");
    Display::put_string_5x5(center - strlen("WAVETABLES") * 6, y_offset, strlen("WAVETABLES"), "WAVETABLES");
    snprintf(line, 20, "%d/64", wavetable_count);
    Display::put_string_5x5(center + 2 * 6, y_offset, strlen(line), line);

    y_offset += 8;
    Display::put_string_5x5(center, y_offset, 1, ";");
    Display::put_string_5x5(center - strlen("WAVES") * 6, y_offset, strlen("WAVES"), "WAVES");
    snprintf(line, 20, "%d/1024", wavetable_count * 16);
    Display::put_string_5x5(center + 2 * 6, y_offset, strlen(line), line);

    y_offset += 8;
    Display::put_string_5x5(center, y_offset, 1, ";");
    Display::put_string_5x5(center - strlen("SNAPSHOTS") * 6, y_offset, strlen("SNAPSHOTS"), "SNAPSHOTS");
    snprintf(line, 20, "%d/64", snapshot_count);
    Display::put_string_5x5(center + 2 * 6, y_offset, strlen(line), line);
}
