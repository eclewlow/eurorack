/*
  ==============================================================================

    MemoryClearMenu.cpp
    Created: 14 Nov 2023 4:43:31pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

MemoryClearMenu::MemoryClearMenu() {
    option_selected_ = MEMORY_MENU_WAVETABLES;
}

MemoryClearMenu::~MemoryClearMenu() {
    
}

bool MemoryClearMenu::handleKeyRelease(int key) {

    if(key == BACK_BUTTON) {
        context.setState(&deviceConfigurationMenu);
    }
    else if(key == LEFT_ENCODER_CCW) {
        if(state_ == MEMORY_MENU_NONE) {
            option_selected_ = MEMORY_MENU_WAVETABLES;
        } else if(state_ == MEMORY_MENU_CONFIRM) {
            option_selected_ = MEMORY_MENU_NO;
        }
    }
    else if(key == LEFT_ENCODER_CW) {
        if(state_ == MEMORY_MENU_NONE) {
            option_selected_ = MEMORY_MENU_SNAPSHOTS;
        } else if(state_ == MEMORY_MENU_CONFIRM) {
            option_selected_ = MEMORY_MENU_YES;
        }
    }
    else if(key == LEFT_ENCODER_CLICK) {
        if(state_ == MEMORY_MENU_NONE) {
            referer_ = option_selected_;
            option_selected_ = MEMORY_MENU_NO;
            state_ = MEMORY_MENU_CONFIRM;
        } else if(state_ == MEMORY_MENU_CONFIRM) {
            if(option_selected_ == MEMORY_MENU_YES) {
                if(referer_ == MEMORY_MENU_WAVETABLES) {
                    // TODO: CLEAR user wavetables. SET STATE TO NONE . SHOW POPUP
                    storage.EraseAllWavetables();
                    popup.show();
                    popup.SetLine(0, (char*)"ERASING...");
                    popup.SetLine(1, (char*)"\0");
                    popup.SetLine(2, (char*)"\0");
                    state_ = MEMORY_MENU_NONE;
                    option_selected_ = referer_;
                } else if(referer_ == MEMORY_MENU_SNAPSHOTS) {
                    // TODO: CLEAR snapshots. SET STATE TO NONE . SHOW POPUP
                    storage.EraseAllSnapshots();
                    popup.show();
                    popup.SetLine(0, (char*)"ERASING...");
                    popup.SetLine(1, (char*)"\0");
                    popup.SetLine(2, (char*)"\0");
                    state_ = MEMORY_MENU_NONE;
                    option_selected_ = referer_;
                }
            } else if(option_selected_ == MEMORY_MENU_NO) {
                option_selected_ = referer_;
                state_ = MEMORY_MENU_NONE;
            }
        }
    }
    return true;
}

void MemoryClearMenu::paint(juce::Graphics& g) {
    Display::clear_screen();
    
    if(state_ == MEMORY_MENU_NONE) {
        int16_t y_offset = 5;
        
        Display::put_string_9x9(128 / 2 - Display::get_string_9x9_width("ERASE", 3) / 2, y_offset, strlen("ERASE"), "ERASE", false, 3);
        
//        y_offset += 10 + 5;
        
        int16_t x_offset = 2;
        y_offset = 20 + (64 - 20) / 2 - 14 * 3 / 2;

        char line[20];
        
//        int wavetable_count = 32;
//        int snapshot_count = 4;
        
        int center = 128 / 2;

        y_offset += 7;

        Display::put_image_16bit(x_offset, y_offset, Graphic_icon_delete_11x11, 11);
        Display::put_string_9x9(x_offset + 16, y_offset + 1, strlen("USER WT"), "USER WT", option_selected_ == MEMORY_MENU_WAVETABLES, 3);

        y_offset += 14;
        Display::put_image_16bit(x_offset, y_offset, Graphic_icon_delete_11x11, 11);
        Display::put_string_9x9(x_offset + 16, y_offset + 1, strlen("SNAPSHOTS"), "SNAPSHOTS", option_selected_ == MEMORY_MENU_SNAPSHOTS, 3);

        //    snprintf(line, 20, "   VERSION; %s", "0.07");
//        Display::put_string_5x5(center, y_offset, 1, ":");
//        Display::put_string_5x5(center - strlen("MORPH") * 6, y_offset, strlen("MORPH"), "MORPH");
//        snprintf(line, 20, "%s", user_settings.getMorphSetting() == UserSettings::SETTING_MORPH_SMOOTH ? "SMOOTH":"DISCRETE");
//        Display::put_string_5x5(center + 6, y_offset, strlen(line), line, true);
    }
    else if(state_ == MEMORY_MENU_CONFIRM) {
        int y_offset = 16;
        int x_offset = 0;
        
        int line_count = 0;
        
        char * confirm_lines[3];
        
        if(referer_ == MEMORY_MENU_WAVETABLES) {
            confirm_lines[0] = (char*) "DELETE ALL";
            confirm_lines[1] = (char*) "USER WAVETABLES?";
            confirm_lines[2] = (char*) "\0";
        } else if(referer_ == MEMORY_MENU_SNAPSHOTS) {
            confirm_lines[0] = (char*) "DELETE ALL";
            confirm_lines[1] = (char*) "USER SNAPSHOTS?";
            confirm_lines[2] = (char*) "\0";
        }
    
        for(int i = 0; i < 3; i++) {
            if(strlen(confirm_lines[i]) > 0)
                line_count++;
        }
        
        y_offset -= line_count * 6 / 2;
        
        for(int i = 0; i < 3; i++) {
            if(strlen(confirm_lines[i]) > 0) {
                int16_t x_offset = 64 - strlen(confirm_lines[i]) * 6 / 2;
                Display::put_string_5x5(x_offset, y_offset, strlen(confirm_lines[i]), confirm_lines[i]);
            }
            y_offset += 6;
        }
        
        y_offset = 48 - 10 / 2;
        x_offset = 32 - (16 + Display::get_string_9x9_width("NO", 3)) / 2;
        Display::put_image_16bit(x_offset, y_offset, Graphic_icon_cancel_11x11, 11);
        Display::put_string_9x9(x_offset + 16, y_offset, strlen("NO"), "NO", option_selected_ == MEMORY_MENU_NO, 3);
        
        x_offset = 96 - (16 + Display::get_string_9x9_width("YES", 3)) / 2;
        Display::put_image_16bit(x_offset, y_offset, Graphic_icon_delete_11x11, 11);
        Display::put_string_9x9(x_offset + 16, y_offset, strlen("YES"), "YES", option_selected_ == MEMORY_MENU_YES, 3);
    }
}
