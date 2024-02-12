/*
  ==============================================================================

    QuantizerMenu.cpp
    Created: 14 Nov 2023 4:43:20pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

namespace waves {

QuantizerMenu::QuantizerMenu() {
}

QuantizerMenu::~QuantizerMenu() {
    
}

bool QuantizerMenu::handleKeyRelease(int key) {

    if(key == BACK_BUTTON) {
        context.setState(&deviceConfigurationMenu);
    }
    else if(key == LEFT_ENCODER_CCW) {
        quantizer.setScale((quantizer.getScale() + Quantizer::QUANTIZER_SCALE_LAST - 1) % Quantizer::QUANTIZER_SCALE_LAST);
    }
    else if(key == LEFT_ENCODER_CW) {
        quantizer.setScale((quantizer.getScale() + 1) % Quantizer::QUANTIZER_SCALE_LAST);
    }
    else if(key == LEFT_ENCODER_CLICK) {
//        context.setState(&deviceConfigurationMenu);
    }
    return true;
}

void QuantizerMenu::paint() {
    Display::clear_screen();
    
    int y_offset = 5;
    
    Display::put_string_9x9(128 / 2 - Display::get_string_9x9_width("QUANTIZER", 2) / 2, y_offset, strlen("QUANTIZER"), "QUANTIZER", false, 2);

    char line[20];
    char * line2;
    int16_t scale_binary = 0b000000000000;
    switch(quantizer.getScale()) {
        case Quantizer::QUANTIZER_SCALE_OFF:
            line2 = (char*)"OFF";
            scale_binary = 0b000000000000;
            break;
        case Quantizer::QUANTIZER_SCALE_PENTATONIC:
            line2 = (char*)"PENTATONIC";
            scale_binary = 0b001010010101;
//            notes = numpy.array([0, 2, 4, 7, 9])
            break;
        case Quantizer::QUANTIZER_SCALE_BLUES:
            line2 = (char*)"BLUES";
            scale_binary = 0b001010011101;
//            notes = numpy.array([0, 2, 3, 4, 7, 9])
            break;
        case Quantizer::QUANTIZER_SCALE_DIMINISH:
            line2 = (char*)"DIMINISH";
            scale_binary = 0b101101101101;
//            notes = numpy.array([0, 2, 3, 5, 6, 8, 9, 11])
            break;
        case Quantizer::QUANTIZER_SCALE_HARMONIC_MAJOR:
            line2 = (char*)"HARMONIC MAJOR";
            scale_binary = 0b100110110101;
//            notes = numpy.array([0, 2, 4, 5, 7, 8, 11])
            break;
        case Quantizer::QUANTIZER_CALE_HARMONIC_MINOR:
            line2 = (char*)"HARMONIC MINOR";
            scale_binary = 0b100110101101;
//            notes = numpy.array([0, 2, 3, 5, 7, 8, 11])
            break;
        case Quantizer::QUANTIZER_SCALE_WHOLETONE:
            line2 = (char*)"WHOLE-TONE";
            scale_binary = 0b010101010101;
//            notes = numpy.array([0, 2, 4, 6, 8, 10])
            break;
        case Quantizer::QUANTIZER_SCALE_CHROMATIC:
            line2 = (char*)"CHROMATIC";
            scale_binary = 0b111111111111;
//            notes = numpy.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11])
            break;
        case Quantizer::QUANTIZER_SCALE_ARABIAN:
            line2 = (char*)"ARABIAN";
            scale_binary = 0b100110110011;
//            notes = numpy.array([0, 1, 4, 5, 7, 8, 11])
            break;
        case Quantizer::QUANTIZER_SCALE_DIATONIC:
            line2 = (char*)"DIATONIC";
            scale_binary = 0b101010110101;
//            notes = numpy.array([0, 2, 4, 5, 7, 9, 11])
            break;
        case Quantizer::QUANTIZER_SCALE_LAST:
            line2 = (char*)"";
            scale_binary = 0b000000000000;
            break;
        default:
            line2 = (char*)"";
            scale_binary = 0b000000000000;
            break;
    }
    
    /*
     * Draw keyboard
     */
    y_offset += 10 + 5;
    int keyboard_width = 92;
    int keyboard_height = 30;
    Display::outline_rectangle(64 - keyboard_width / 2, y_offset, keyboard_width, keyboard_height);
    
    
    // draw 7 white keys
    int key_width = (keyboard_width - 1) / 7;
        // 1 2 3 4 5 6 7
        // 0 2 4 5 7 9 11
    if((scale_binary >> 0) & 0x1)
        Display::clear_rectangle_grey(64 - keyboard_width / 2 + 0 * key_width, y_offset, key_width + 1, keyboard_height);
    else
        Display::outline_rectangle(64 - keyboard_width / 2 + 0 * key_width, y_offset, key_width + 1, keyboard_height);
    if((scale_binary >> 2) & 0x1)
        Display::clear_rectangle_grey(64 - keyboard_width / 2 + 1 * key_width, y_offset, key_width + 1, keyboard_height);
    else
        Display::outline_rectangle(64 - keyboard_width / 2 + 1 * key_width, y_offset, key_width + 1, keyboard_height);
    if((scale_binary >> 4) & 0x1)
        Display::clear_rectangle_grey(64 - keyboard_width / 2 + 2 * key_width, y_offset, key_width + 1, keyboard_height);
    else
        Display::outline_rectangle(64 - keyboard_width / 2 + 2 * key_width, y_offset, key_width + 1, keyboard_height);
    if((scale_binary >> 5) & 0x1)
        Display::clear_rectangle_grey(64 - keyboard_width / 2 + 3 * key_width, y_offset, key_width + 1, keyboard_height);
    else
        Display::outline_rectangle(64 - keyboard_width / 2 + 3 * key_width, y_offset, key_width + 1, keyboard_height);
    if((scale_binary >> 7) & 0x1)
        Display::clear_rectangle_grey(64 - keyboard_width / 2 + 4 * key_width, y_offset, key_width + 1, keyboard_height);
    else
        Display::outline_rectangle(64 - keyboard_width / 2 + 4 * key_width, y_offset, key_width + 1, keyboard_height);
    if((scale_binary >> 9) & 0x1)
        Display::clear_rectangle_grey(64 - keyboard_width / 2 + 5 * key_width, y_offset, key_width + 1, keyboard_height);
    else
        Display::outline_rectangle(64 - keyboard_width / 2 + 5 * key_width, y_offset, key_width + 1, keyboard_height);
    if((scale_binary >> 11) & 0x1)
        Display::clear_rectangle_grey(64 - keyboard_width / 2 + 6 * key_width, y_offset, key_width + 1, keyboard_height);
    else
        Display::outline_rectangle(64 - keyboard_width / 2 + 6 * key_width, y_offset, key_width + 1, keyboard_height);

    
    
    int black_key_width = 8;
    int black_key_height = keyboard_height / 2 + 1;
    int i = 1;
    if((scale_binary >> 1) & 0x1)
        Display::clear_rectangle_grey(64 - keyboard_width / 2 + i * key_width - black_key_width / 2, y_offset, black_key_width + 1, black_key_height);
    else
        Display::clear_rectangle_simple(64 - keyboard_width / 2 + i * key_width - black_key_width / 2, y_offset, black_key_width + 1, black_key_height, true);
    i = 2;
    if((scale_binary >> 3) & 0x1)
        Display::clear_rectangle_grey(64 - keyboard_width / 2 + i * key_width - black_key_width / 2, y_offset, black_key_width + 1, black_key_height);
    else
        Display::clear_rectangle_simple(64 - keyboard_width / 2 + i * key_width - black_key_width / 2, y_offset, black_key_width + 1, black_key_height, true);
    i = 4;
    if((scale_binary >> 6) & 0x1)
        Display::clear_rectangle_grey(64 - keyboard_width / 2 + i * key_width - black_key_width / 2, y_offset, black_key_width + 1, black_key_height);
    else
        Display::clear_rectangle_simple(64 - keyboard_width / 2 + i * key_width - black_key_width / 2, y_offset, black_key_width + 1, black_key_height, true);
    i = 5;
    if((scale_binary >> 8) & 0x1)
        Display::clear_rectangle_grey(64 - keyboard_width / 2 + i * key_width - black_key_width / 2, y_offset, black_key_width + 1, black_key_height);
    else
        Display::clear_rectangle_simple(64 - keyboard_width / 2 + i * key_width - black_key_width / 2, y_offset, black_key_width + 1, black_key_height, true);
    i = 6;
    if((scale_binary >> 10) & 0x1)
        Display::clear_rectangle_grey(64 - keyboard_width / 2 + i * key_width - black_key_width / 2, y_offset, black_key_width + 1, black_key_height);
    else
        Display::clear_rectangle_simple(64 - keyboard_width / 2 + i * key_width - black_key_width / 2, y_offset, black_key_width + 1, black_key_height, true);

    y_offset = 64 - 6 - 3;
    Display::put_string_5x5(64 - strlen(line2) * 6 / 2, y_offset, strlen(line2), line2, true);
}

}