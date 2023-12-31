/*
  ==============================================================================

    DeviceConfigurationMenu.cpp
    Created: 14 Nov 2023 4:40:15pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

DeviceConfigurationMenu::DeviceConfigurationMenu() {
    setState(DEVICE_INFO);
}

DeviceConfigurationMenu::~DeviceConfigurationMenu() {
    
}

bool DeviceConfigurationMenu::handleKeyRelease(int key) {
//    DEVICE_INFO,
//    DEVICE_DISPLAY,
//    DEVICE_OSCILLOSCOPE,
//    DEVICE_IO,
//    DEVICE_CALIBRATION,
//    DEVICE_QUANTIZER,
//    DEVICE_MEMORY,
    if(key == LEFT_ENCODER_CCW) {
        currentState = CLAMP<int8_t>(currentState - 1, DEVICE_INFO, DEVICE_MEMORY);
    }
    if(key == LEFT_ENCODER_CW) {
        currentState = CLAMP<int8_t>(currentState + 1, DEVICE_INFO, DEVICE_MEMORY);
    }

    if(key == LEFT_ENCODER_CLICK) {
        switch(currentState) {
            case DEVICE_INFO:
                context.setState(&infoMenu);
                break;
            case DEVICE_DISPLAY:
                context.setState(&displayConfigurationMenu);
                break;
            case DEVICE_OSCILLOSCOPE:
                context.setState(&oscilloscopeConfigurationMenu);
                break;
            case DEVICE_IO:
                context.setState(&ioConfigurationMenu);
                break;
            case DEVICE_CALIBRATION:
                context.setState(&calibrationMenu);
                break;
            case DEVICE_QUANTIZER:
                context.setState(&quantizerMenu);
                break;
            case DEVICE_MEMORY:
                context.setState(&memoryClearMenu);
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

void DeviceConfigurationMenu::paint(juce::Graphics& g) {
    Display::clear_screen();
    
    int y_offset = 4;
    int x_offset = 64 - (23+2) * 2;
    int row = 0;
    int col = 0;
    char *caption;
    
    switch(currentState) {
        case DEVICE_INFO:
            caption = (char*)"INFO";
            break;
        case DEVICE_DISPLAY:
            caption = (char*)"DISPLAY";
            break;
        case DEVICE_OSCILLOSCOPE:
            caption = (char*)"OSC";
            break;
        case DEVICE_IO:
            caption = (char*)"I/O";
            break;
        case DEVICE_CALIBRATION:
            caption = (char*)"CALIBRATE";
            break;
        case DEVICE_QUANTIZER:
            caption = (char*)"QUANTIZER";
            break;
        case DEVICE_MEMORY:
            caption = (char*)"MEMORY";
            break;
        default:
            break;
    }

    
    Display::put_string_9x9(64-Display::get_string_9x9_width(caption, 2)/2,64-11,strlen(caption),caption, false, 2);
    
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_setup_menu_info);
    
    col++;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_setup_menu_brightness);
    
    col++;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_setup_menu_oscilloscope);
    
    col++;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_setup_menu_io);
    
    
    row++;
    col=0;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_setup_menu_calibration);
    
    col++;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_setup_menu_quantizer);
    
    col++;
    Display::put_image_22x23(col*(23+2)+x_offset, row*(22+2)+y_offset, Graphic_setup_menu_memory);
    
    col = currentState % 4;
    row = currentState / 4;
    Display::invert_rectangle(col*(23+2)+x_offset, row*(22+2)+y_offset, 23, 22);
}
