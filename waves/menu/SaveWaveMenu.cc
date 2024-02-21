/*
  ==============================================================================

    SaveWaveMenu.cpp
    Created: 26 Nov 2023 5:50:08pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

namespace waves {

SaveWaveMenu::SaveWaveMenu() {
    setState(SAVE_WAVE_MENU_SELECT_WAVETABLE);
    wavetable_ = 0;
    frame_ = 0;
    ticker_timer_ = 0;
    ticker_ = 0;
}

SaveWaveMenu::~SaveWaveMenu() {
    
}

void SaveWaveMenu::triggerUpdate(bool back_pressed) {
    if(!back_pressed) {
        wavetable_offset_ = 0;
        wavetable_ = 0;
        frame_ = 0;
        frame_offset_ = 0;
        flash.StartFrameDMARead((uint32_t*)wavetable_names_, 16 * 9, 0, NULL, EEPROM_PERSISTENT_SS);
        flash.StartFrameDMARead((uint32_t*)frame_names_, 16 * 9, 4096 + 4096 * wavetable_, NULL, EEPROM_PERSISTENT_SS);
        return;
    }
    if(wavetable_ < wavetable_offset_) {
        wavetable_offset_ = wavetable_;
    }

    if(frame_ < frame_offset_) {
        frame_offset_ = frame_;
    }
    
    if(wavetable_ > wavetable_offset_ + 5) {
        wavetable_offset_ = wavetable_ - 5;
    }

    if(frame_ > frame_offset_ + 5) {
        frame_offset_ = frame_ - 5;
    }

    flash.StartFrameDMARead((uint32_t*)wavetable_names_, 16 * 9, 0, NULL, EEPROM_PERSISTENT_SS);
    flash.StartFrameDMARead((uint32_t*)frame_names_, 16 * 9, 4096 + 4096 * wavetable_, NULL, EEPROM_PERSISTENT_SS);
    
    ResetTicker();

}

void SaveWaveMenu::ResetTicker() {
    ticker_timer_ = system_clock.milliseconds();
    ticker_ = 0;
}

bool SaveWaveMenu::handleKeyRelease(int key) {
    if(key == LEFT_ENCODER_CCW) {
        switch(state_) {
            case SAVE_WAVE_MENU_SELECT_WAVETABLE:
                wavetable_ = CLAMP<int16_t>(wavetable_ - 1, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
                
                if(wavetable_ < wavetable_offset_) {
                    wavetable_offset_ = wavetable_;
                }

                break;
            case SAVE_WAVE_MENU_SELECT_FRAME:
                ResetTicker();

                frame_ = CLAMP<int16_t>(frame_ - 1, 0, 15);

                if(frame_ < frame_offset_) {
                    frame_offset_ = frame_;
                }

                break;
            default:
                break;
        }
    }
    if(key == LEFT_ENCODER_CW) {
        switch(state_) {
            case SAVE_WAVE_MENU_SELECT_WAVETABLE:
                wavetable_ = CLAMP<int16_t>(wavetable_ + 1, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
                
                if(wavetable_ > wavetable_offset_ + 5) {
                    wavetable_offset_ = wavetable_ - 5;
                }

                break;
            case SAVE_WAVE_MENU_SELECT_FRAME:
                ResetTicker();

                frame_ = CLAMP<int16_t>(frame_ + 1, 0, 15);

                if(frame_ > frame_offset_ + 5) {
                    frame_offset_ = frame_ - 5;
                }

                break;
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CCW) {
        switch(state_) {
            case SAVE_WAVE_MENU_SELECT_WAVETABLE:
                break;
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CW) {
        switch(state_) {
            case SAVE_WAVE_MENU_SELECT_WAVETABLE:
                break;
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CLICK) {
        switch(state_) {
            case SAVE_WAVE_MENU_SELECT_WAVETABLE:
//                setState(LOAD_WAVE_MENU_SELECT_FRAME);
                break;
            case SAVE_WAVE_MENU_SELECT_FRAME:
//                if(target_ == AB_ENGINE_A) {
//                    abEngine.SetLeftWavetable(wavetable_);
//                    abEngine.SetLeftFrame(frame_);
//                } else if(target_ == AB_ENGINE_B) {
//                    abEngine.SetRightWavetable(wavetable_);
//                    abEngine.SetRightFrame(frame_);
//                }
                break;
            default:
                break;
        }
    }
    if(key == LEFT_ENCODER_CLICK) {
        switch(state_) {
            case SAVE_WAVE_MENU_SELECT_WAVETABLE:
//                if(storage.GetWavetable(wavetable_)->name[0] == '\0') {
//                    context.setState(&enterNameMenu);
//                    enterNameMenu.setBackMenu(&saveWaveMenu);
//                    enterNameMenu.setExecFunc(SaveWaveMenu::SaveWavetable);
//                } else {
                ResetTicker();

                setState(SAVE_WAVE_MENU_SELECT_FRAME);
                frame_ = 0;
                frame_offset_ = 0;
                flash.StartFrameDMARead((uint32_t*)frame_names_, 16 * 9, 4096 + 4096 * wavetable_, NULL, EEPROM_PERSISTENT_SS);
//                }
                break;
            case SAVE_WAVE_MENU_SELECT_FRAME: {
                if(false) { //storage.GetWavetable(wavetable_)->factory_preset) {
                    popup.show();
                    popup.SetLine(0, (char*)"CANNOT OVERWRITE");
                    popup.SetLine(1, (char*)"FACTORY PRESETS!");
                    popup.SetLine(2, (char*)"\0");
                }
                else {
                    context.setState(&enterNameMenu);
                    enterNameMenu.setBackMenu(&saveWaveMenu);
                    enterNameMenu.setExecFunc(SaveWaveMenu::SaveWave);
                    enterNameMenu.setNameChars(storage.GetWavetable(wavetable_)->waves[frame_].name);
                }
                break;
            }
            default:
                break;
        }
    }
    if(key == BACK_BUTTON) {
        switch(state_) {
            case SAVE_WAVE_MENU_SELECT_WAVETABLE:
                if(back_menu_)
                    context.setState(back_menu_, true);
                else
                    context.setState(&mainMenu);
                break;
            case SAVE_WAVE_MENU_SELECT_FRAME:
                setState(SAVE_WAVE_MENU_SELECT_WAVETABLE);
                break;
            default:
                break;
        }
    }

    return true;
}

void SaveWaveMenu::SaveWavetable(char* param) {
    // storage.SaveWavetable(param, saveWaveMenu.wavetable_);
    strncpy(saveWaveMenu.wavetable_names_[saveWaveMenu.wavetable_], param, 9);
    flash.SectorErase4K(0, EEPROM_PERSISTENT_SS);
    flash.Page_Program_Repeat((uint8_t *)saveWaveMenu.wavetable_names_, 16 * 9, 0, EEPROM_PERSISTENT_SS);
}

void SaveWaveMenu::SaveWave(char* param) {
    // storage.SaveWave(param, saveWaveMenu.wavedata_, saveWaveMenu.wavetable_, saveWaveMenu.frame_);
    strncpy(saveWaveMenu.frame_names_[saveWaveMenu.frame_], param, 9);
    flash.SectorErase4K(4096 + 4096 * saveWaveMenu.wavetable_, EEPROM_PERSISTENT_SS);
    flash.Page_Program_Repeat((uint8_t *)saveWaveMenu.frame_names_, 16 * 9, 4096 + 4096 * saveWaveMenu.wavetable_, EEPROM_PERSISTENT_SS);

    flash.SectorErase4K(saveWaveMenu.wavetable_ * 65536 + saveWaveMenu.frame_ * 4096, EEPROM_FACTORY_SS);
    flash.Page_Program_Repeat((uint8_t *)saveWaveMenu.wavedata_, 4096, saveWaveMenu.wavetable_ * 65536 + saveWaveMenu.frame_ * 4096, EEPROM_FACTORY_SS);
}

void SaveWaveMenu::paint() {
    Display::clear_screen();
    if(state_ == SAVE_WAVE_MENU_SELECT_WAVETABLE) {

        char * title = (char *) "SELECT WAVETABLE";

        int y_offset = 3;
        int x_offset = 1 + 2 * 4;

        Display::put_string_5x5(x_offset, y_offset, strlen(title), title);
        
        Display::invert_rectangle(0, 0, 128, 11);
        
        x_offset = 128 - 5;
        y_offset += 11;

        for(int i = 0; i < 6; i++)
        {
            char line[20];
            memset(line, 0, 20);
            snprintf(line, 20, "%*d", 2, i + wavetable_offset_ + 1);
            Display::put_string_3x5(2, y_offset + i * 8, strlen(line), line);
            
            char line2[20];
            memset(line2, 0, 20);
//            if(storage.GetWavetable(i + wavetable_offset_)->name[0] == '\0') {
//                strncpy(line2, "+NEW WAVETABLE", strlen("+NEW WAVETABLE"));
//            }
//            else {
            snprintf(line2, 20, "%-8s    [%02d]", wavetable_names_[i + wavetable_offset_], 5);
//            }
            
            Display::put_string_5x5(2 + 2 * 3 + 4, y_offset + i * 8, strlen(line2), line2, i+wavetable_offset_ == wavetable_);
        }

        int y_shift = 2;
        int bar_height = 64 - y_offset - 1;
        int y_cursor_offset = ((bar_height - 5) * wavetable_offset_) / (USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1 - 5);
        Display::outline_rectangle(x_offset+1, y_offset + 1 - y_shift + y_cursor_offset, 1, 3);
        Display::invert_rectangle(x_offset, y_offset - y_shift, 3, bar_height);

    } else {
        char * title = (char *) "SELECT WAVE SLOT";
        title = wavetable_names_[wavetable_];

        int y_offset = 3;
        int x_offset = 1 + 2 * 4;

        Display::put_string_5x5(x_offset, y_offset, strlen(title), title);
        
        Display::invert_rectangle(0, 0, 128, 11);
        
        x_offset = 64 - 5;
        y_offset += 11;

        for(int i = 0; i < 6; i++)
        {
            char line[20];
            snprintf(line, 20, "%*d", 2, i + frame_offset_ + 1);
            Display::put_string_3x5(2, y_offset + i * 8, strlen(line), line);
            
            
            char * name = frame_names_[i + frame_offset_];
            
            char * line2 = name;
            
            int32_t elapsed_time = system_clock.milliseconds() - ticker_timer_;
            
            uint8_t num_chars = 7;
            
            if(i + frame_offset_ == frame_) {
                if(ticker_ == 0) {
                    if(elapsed_time > 1000) {
                        ticker_++;
                        ticker_timer_ = system_clock.milliseconds();
                    }
                } else if(ticker_ == (strlen(line2) - num_chars) * 6) {
                    if(elapsed_time > 2000) {
                        ResetTicker();
                    }
                }
                else if (elapsed_time > 20) {
                    ticker_++;
                    ticker_timer_ = system_clock.milliseconds();
                }
                Display::put_string_5x5_loop(2 + 2 * 3 + 4, y_offset + i * 8, strlen(line2), line2, i+frame_offset_ == frame_, num_chars, strlen(line2) > num_chars ? ticker_ : 0);
            } else {
                Display::put_string_5x5_loop(2 + 2 * 3 + 4, y_offset + i * 8, strlen(line2), line2, i+frame_offset_ == frame_, num_chars, 0);
            }
        }

        int y_shift = 2;
        int bar_height = 64 - y_offset - 1;
        int y_cursor_offset = ((bar_height - 5) * frame_offset_) / 10;
        Display::outline_rectangle(x_offset+1, y_offset + 1 - y_shift + y_cursor_offset, 1, 3);
        Display::invert_rectangle(x_offset, y_offset - y_shift, 3, bar_height);

        if(wavetable_gui_ != wavetable_ && frame_gui_ != frame_) {
            // load double frame. draw double frame
            flash.StartFrameDMARead((uint32_t*)front_buffer_4, 4096, wavetable_ * 65536 + frame_ * 4096, NULL, EEPROM_FACTORY_SS);
            wavetable_gui_ = wavetable_;
            frame_gui_ = frame_;
        }
        abEngine.FillWaveform(BUF1, 0.0f);

        // storage.LoadWaveSample(BUF1, wavetable_, frame_);

        Display::Draw_Wave(64, y_offset - y_shift, 64, bar_height - 3, BUF1);
    }
}

}
