/*
  ==============================================================================

    EnterNameMenu.cpp
    Created: 14 Nov 2023 4:45:29pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "EnterNameMenu.h"

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

EnterNameMenu::EnterNameMenu() {
    char_index = 0;
    name_index = 0;
    memset(name_chars,0,9);
//    name_chars[name_index] = char_list[char_index];
}

EnterNameMenu::~EnterNameMenu() {
    
}

bool EnterNameMenu::handleKeyRelease(int key) {
    if(key == RIGHT_ENCODER_CCW) {
        char_index = (char_index + num_of_chars - 1) % num_of_chars;
//        char_index = std::clamp<int>(char_index, 0, num_of_chars-1);
    }
    else if(key == RIGHT_ENCODER_CW) {
        char_index = (char_index + 1) % num_of_chars;
//        char_index ++;
//        char_index = std::clamp<int>(char_index, 0, num_of_chars-1);
    }
    if(key == RIGHT_ENCODER_CLICK) {
        if(char_index == num_of_chars - 2) {
            bool del = true;
            if(del) {
                if(name_index == 8-1 && name_chars[name_index] != 0) {
//                    name_index --;
//                    name_index = std::clamp<int>(name_index, 0, 8-1);
                    name_chars[name_index] = 0;
                } else {
                    name_index --;
                    name_index = std::clamp<int>(name_index, 0, 8-1);
                    name_chars[name_index] = 0;
                }
            }
            else {
                name_index = 0;
                //            char_index = 0;
                memset(name_chars,0,8);
            }
        }
        else if(char_index == num_of_chars - 1) {
            if(name_index > 0) {
                if(exec_func)
                    (*exec_func)(name_chars);
                if(back_menu_)
                    context.setState(back_menu_, true);
            } else {
                popup.show();
                popup.SetLine(0, (char*)"PLEASE ENTER");
                popup.SetLine(1, (char*)"A NAME!");
                popup.SetLine(2, (char*)"\0");
            }
        }
        else {
            name_chars[name_index] = char_list[char_index];
            name_index ++;
            name_index = std::clamp<int>(name_index, 0, 8-1);
        }
    }
    if(key == BACK_BUTTON) {
        if(back_menu_)
            context.setState(back_menu_, true);
    }

    return true;
}

void EnterNameMenu::triggerUpdate(bool back_pressed) {
    char_index = 0;
    name_index = 0;
    memset(name_chars,0,9);
}

void EnterNameMenu::paint(juce::Graphics& g) {
    Display::clear_screen();
    
    int y_offset;
    int x_offset;
        
    y_offset = 3;
    x_offset = 1 + 2 * 4;

    Display::put_string_5x5(x_offset, y_offset, strlen("ENTER NAME"), "ENTER NAME");
    
    Display::invert_rectangle(0, 0, 128, 11);

    x_offset = 5;
    y_offset += 14;
    
    // draw input characters
    for(int i=0; i < 8; i++) {
        int y = y_offset + 9 + 2;
        int x = i * 12 + x_offset;
        Display::LCD_Line(x, y, x + 8, y, true);
        if(i == name_index)
            Display::LCD_Line(x, y+1, x + 8, y+1, true);

        char theChar[2];
        snprintf(theChar, 2, "%c", name_chars[i]);

        Display::put_string_9x9(x, y_offset, 1, theChar, false, 0);
        
    }
    

    y_offset += 17;
    
    Display::outline_rectangle(x_offset, y_offset, 128-5-5, 8*3 + 4);

    y_offset += 3;
    x_offset += 3;
    int row = 0;
    for(int i = 0; i < num_of_chars - 2; i++) {
        // 16 per row
        int row = i / 16;
        int col = i % 16;
        char theChar[2];
        snprintf(theChar, 2, "%c", char_list[i]);
        
        Display::put_string_5x5(x_offset + col * 7, y_offset + row * 8, 1, theChar, i == char_index);
    }
    Display::put_string_5x5(x_offset + 8 * 7, y_offset + 2 * 8, 3, "CLR", (num_of_chars - 2) == char_index);
    Display::put_string_5x5(x_offset + 8 * 7 + 5 * 5, y_offset + 2 * 8, 2, "OK", (num_of_chars - 1) == char_index);
}
