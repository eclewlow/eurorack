/*
  ==============================================================================

    SubOscillatorMenu.cpp
    Created: 14 Nov 2023 4:38:34pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "Display.h"
#include "graphics.h"
#include "Globals.h"

SubOscillatorMenu::SubOscillatorMenu() {
    setState(SUB_OSC_MENU_OFFSET);
}

SubOscillatorMenu::~SubOscillatorMenu() {
    
}

bool SubOscillatorMenu::handleKeyRelease(int key) {
//    SUB_OSC_MENU_OFFSET,
//    SUB_OSC_MENU_DETUNE,
//    SUB_OSC_MENU_MIX,
//    SUB_OSC_MENU_WAVE,

    if(key == BACK_BUTTON) {
        context.setState(&mainMenu);
    }
    else if(key == LEFT_ENCODER_CLICK) {
        context.getEngine()->reset_phase();
        suboscillator.reset_phase();
    }
    else if(key == RIGHT_ENCODER_CCW) {
        switch(state_) {
            case SUB_OSC_MENU_OFFSET:
                user_settings.setSubOscOffset(user_settings.getSubOscOffset()-1);
                break;
            case SUB_OSC_MENU_DETUNE:
                user_settings.setSubOscDetune(user_settings.getSubOscDetune()-1);
                break;
            case SUB_OSC_MENU_MIX:
                user_settings.setSubOscMix(user_settings.getSubOscMix()-1);
                break;
            case SUB_OSC_MENU_WAVE:
                user_settings.setSubOscWave((user_settings.getSubOscWave() + UserSettings::SUBOSC_WAVE_LAST - 1) % UserSettings::SUBOSC_WAVE_LAST);
                break;
            default:
                break;
        }
    }
    else if(key == RIGHT_ENCODER_CW) {
        switch(state_) {
            case SUB_OSC_MENU_OFFSET:
                user_settings.setSubOscOffset(user_settings.getSubOscOffset()+1);
                break;
            case SUB_OSC_MENU_DETUNE:
                user_settings.setSubOscDetune(user_settings.getSubOscDetune()+1);
                break;
            case SUB_OSC_MENU_MIX:
                user_settings.setSubOscMix(user_settings.getSubOscMix()+1);
                break;
            case SUB_OSC_MENU_WAVE:
                user_settings.setSubOscWave((user_settings.getSubOscWave() + 1) % UserSettings::SUBOSC_WAVE_LAST);
                break;
            default:
                break;
        }
    }
    else if(key == LEFT_ENCODER_CW) {
        switch(state_) {
            case SUB_OSC_MENU_OFFSET:
                setState(SUB_OSC_MENU_DETUNE);
                break;
            case SUB_OSC_MENU_DETUNE:
                setState(SUB_OSC_MENU_MIX);
                break;
            case SUB_OSC_MENU_MIX:
                setState(SUB_OSC_MENU_WAVE);
                break;
            case SUB_OSC_MENU_WAVE:
                break;
            default:
                break;
        }
    }
    else if(key == LEFT_ENCODER_CCW) {
        switch(state_) {
            case SUB_OSC_MENU_OFFSET:
                break;
            case SUB_OSC_MENU_DETUNE:
                setState(SUB_OSC_MENU_OFFSET);
                break;
            case SUB_OSC_MENU_MIX:
                setState(SUB_OSC_MENU_DETUNE);
                break;
            case SUB_OSC_MENU_WAVE:
                setState(SUB_OSC_MENU_MIX);
                break;
            default:
                break;
        }
    }
    return true;
}

void SubOscillatorMenu::paint(juce::Graphics& g) {
    Display::clear_screen();
    
    int y_offset = 5;
    
    Display::put_string_9x9(128 / 2 - Display::get_string_9x9_width("SUB OSC", 2) / 2, y_offset, strlen("SUB OSC"), "SUB OSC", false, 2);

    y_offset += 10 + 5;
    
    char line[20];

    int wavetable_count = 32;
    int snapshot_count = 4;
    
    int center = 128 / 2 - 6 / 2;
//    snprintf(line, 20, "   VERSION; %s", "0.07");
    Display::put_string_5x5(center, y_offset, 1, ":");
    Display::put_string_5x5(center - strlen("OFFSET") * 6, y_offset, strlen("OFFSET"), "OFFSET");
    snprintf(line, 20, "%d SEMI", user_settings.getSubOscOffset());
    Display::put_string_5x5(center + 6, y_offset, strlen(line), line, state_==SUB_OSC_MENU_OFFSET);

    y_offset += 8;
    Display::put_string_5x5(center, y_offset, 1, ":");
    Display::put_string_5x5(center - strlen("DETUNE") * 6, y_offset, strlen("DETUNE"), "DETUNE");
    snprintf(line, 20, "%d CENT", user_settings.getSubOscDetune());
    Display::put_string_5x5(center + 6, y_offset, strlen(line), line, state_==SUB_OSC_MENU_DETUNE);

    y_offset += 8;
    Display::put_string_5x5(center, y_offset, 1, ":");
    Display::put_string_5x5(center - strlen("MIX") * 6, y_offset, strlen("MIX"), "MIX");
    snprintf(line, 20, "%d%%", user_settings.getSubOscMix());
    Display::put_string_5x5(center + 6, y_offset, strlen(line), line, state_==SUB_OSC_MENU_MIX);

    y_offset += 8;
    Display::put_string_5x5(center, y_offset, 1, ":");
    Display::put_string_5x5(center - strlen("WAVE") * 6, y_offset, strlen("WAVE"), "WAVE");
    
    char* wave_name;

    switch(user_settings.getSubOscWave()) {
        case UserSettings::SUBOSC_WAVE_SINE:
            wave_name=(char*)"SINE";
            break;
        case UserSettings::SUBOSC_WAVE_TRIANGLE:
            wave_name=(char*)"TRI";
            break;
        case UserSettings::SUBOSC_WAVE_SAWTOOTH:
            wave_name=(char*)"SAW";
            break;
        case UserSettings::SUBOSC_WAVE_RAMP:
            wave_name=(char*)"RAMP";
            break;
        case UserSettings::SUBOSC_WAVE_SQUARE:
            wave_name=(char*)"SQUARE";
            break;
        case UserSettings::SUBOSC_WAVE_COPY:
            wave_name=(char*)"COPY";
            break;
        default:
            wave_name=(char*)"";
            break;
    }
//    snprintf(line, 20, "%s", user_settings.getScopeSetting() == UserSettings::SETTING_SCOPE_LINE ? "LINE" : "SHADED");
    Display::put_string_5x5(center + 6, y_offset, strlen(wave_name), wave_name, state_==SUB_OSC_MENU_WAVE);
    
    y_offset = 64 - 7;
    Display::put_string_5x5(128 / 2 - strlen("PRESS L ENC TO SYNC") * 6 / 2, y_offset, strlen("PRESS L ENC TO SYNC"), "PRESS L ENC TO SYNC");

}
