/*
  ==============================================================================

    ManageMenu.cpp
    Created: 5 Dec 2023 2:46:59pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "Display.h"
#include "graphics.h"
#include "Globals.h"

ManageMenu::ManageMenu() {
    setState(MANAGE_MENU_SELECT_WAVETABLE);
    wavetable_ = 0;
    frame_ = 0;
    ticker_timer_ = 0;
    absorb_keypress_ = false;
}

ManageMenu::~ManageMenu() {
    
}

void ManageMenu::triggerUpdate(bool back_pressed) {
    if(!back_pressed) {
        wavetable_offset_ = 0;
        wavetable_ = 0;
        frame_ = 0;
        frame_offset_ = 0;
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
}

void ManageMenu::ResetTicker() {
    ticker_timer_ = system_clock.milliseconds();
    ticker_ = 0;
}

bool ManageMenu::handleKeyLongPress(int key) {
    if( key == LEFT_ENCODER_CLICK ) {
        if(state_ == MANAGE_MENU_SELECT_WAVETABLE) {
            if(system_clock.milliseconds() - press_timer_ > 1000) {
                option_selected_ = MANAGE_MENU_COPY;
                setState(MANAGE_MENU_WAVETABLE_OPTIONS);
                absorb_keypress_ = true;
                return true;
            }
        } else if(state_ == MANAGE_MENU_SELECT_FRAME) {
            if(system_clock.milliseconds() - press_timer_ > 1000) {
                if(storage.GetWavetable(wavetable_)->factory_preset) {
                    option_selected_ = MANAGE_MENU_COPY;
                } else {
                    option_selected_ = MANAGE_MENU_EDIT;
                }
                
                if(option_selected_ < option_offset_) {
                    option_offset_ = option_selected_;
                }
                if(option_selected_ > option_offset_ + 1) {
                    option_offset_ = option_selected_ - 1;
                }


                setState(MANAGE_MENU_FRAME_OPTIONS);
                absorb_keypress_ = true;
                return true;
            }
        }
    }
    return false;
}

bool ManageMenu::handleKeyPress(int key) {
    if( key == LEFT_ENCODER_CLICK ) {
        press_timer_ = system_clock.milliseconds();
    }
    return false;
}

bool ManageMenu::handleKeyRelease(int key) {

    if(key == LEFT_ENCODER_CCW) {
        switch(state_) {
            case MANAGE_MENU_SELECT_WAVETABLE:
                wavetable_ = std::clamp<int16_t>(wavetable_ - 1, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
                
                if(wavetable_ < wavetable_offset_) {
                    wavetable_offset_ = wavetable_;
                }

                morph_ = 0.0f;

                ResetTicker();

                break;
            case MANAGE_MENU_MOVE_WAVETABLE: {
                int16_t target_wavetable = std::clamp<int16_t>(wavetable_ - 1, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
                
                // wavetable swap
                storage.SwapWavetables(wavetable_, target_wavetable);
                
                wavetable_ = target_wavetable;
                
                if(wavetable_ < wavetable_offset_) {
                    wavetable_offset_ = wavetable_;
                }
                
                morph_ = 0.0f;
                
                ResetTicker();
                
                break;
            }
            case MANAGE_MENU_SELECT_FRAME:
                frame_ = std::clamp<int16_t>(frame_ - 1, 0, 15);

                if(frame_ < frame_offset_) {
                    frame_offset_ = frame_;
                }

                ResetTicker();

                break;
            case MANAGE_MENU_MOVE_FRAME: {
                int16_t target_frame = std::clamp<int16_t>(frame_ - 1, 0, 15);
                
                storage.SwapWaves(wavetable_, frame_, target_frame);
                
                frame_ = target_frame;
                
                if(frame_ < frame_offset_) {
                    frame_offset_ = frame_;
                }

                ResetTicker();
                
                break;
            }
            case MANAGE_MENU_WAVETABLE_OPTIONS:
                if(storage.GetWavetable(wavetable_)->factory_preset)
                {
                    
                }
                else
                    setOptionSelected(std::clamp<int8_t>(option_selected_ - 1, MANAGE_MENU_COPY, MANAGE_MENU_DELETE));
                break;
            case MANAGE_MENU_FRAME_OPTIONS:
                if(storage.GetWavetable(wavetable_)->factory_preset)
                {
                    
                }
                else {
                    setOptionSelected(std::clamp<int8_t>(option_selected_ - 1, MANAGE_MENU_EDIT, MANAGE_MENU_DELETE));
                    
                    if(option_selected_ < option_offset_) {
                        option_offset_ = option_selected_;
                    }


                }
                break;
            case MANAGE_MENU_CONFIRM:
                setOptionSelected(std::clamp<int8_t>(option_selected_ - 1, MANAGE_MENU_NO, MANAGE_MENU_YES));
                break;
            default:
                break;
        }
    }
    if(key == LEFT_ENCODER_CW) {
        switch(state_) {
            case MANAGE_MENU_SELECT_WAVETABLE:
                wavetable_ = std::clamp<int16_t>(wavetable_ + 1, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
                
                if(wavetable_ > wavetable_offset_ + 5) {
                    wavetable_offset_ = wavetable_ - 5;
                }

                morph_ = 0.0f;
                
                ResetTicker();
                
                break;
            case MANAGE_MENU_MOVE_WAVETABLE: {
                int16_t target_wavetable = std::clamp<int16_t>(wavetable_ + 1, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
                
                // wavetable swap
                storage.SwapWavetables(wavetable_, target_wavetable);
                
                wavetable_ = target_wavetable;
                
                if(wavetable_ > wavetable_offset_ + 5) {
                    wavetable_offset_ = wavetable_ - 5;
                }

                morph_ = 0.0f;
                
                ResetTicker();
                
                break;
            }            
            case MANAGE_MENU_SELECT_FRAME:
                frame_ = std::clamp<int16_t>(frame_ + 1, 0, 15);

                if(frame_ > frame_offset_ + 5) {
                    frame_offset_ = frame_ - 5;
                }

                ResetTicker();

                break;
            case MANAGE_MENU_MOVE_FRAME: {
                int16_t target_frame = std::clamp<int16_t>(frame_ + 1, 0, 15);
                
                storage.SwapWaves(wavetable_, frame_, target_frame);
                
                frame_ = target_frame;
                
                if(frame_ > frame_offset_ + 5) {
                    frame_offset_ = frame_ - 5;
                }

                ResetTicker();
                
                break;
            }
            case MANAGE_MENU_WAVETABLE_OPTIONS:
                if(storage.GetWavetable(wavetable_)->factory_preset)
                {
                    
                }
                else
                    setOptionSelected(std::clamp<int8_t>(option_selected_ + 1, MANAGE_MENU_COPY, MANAGE_MENU_DELETE));
                break;
            case MANAGE_MENU_FRAME_OPTIONS:
                if(storage.GetWavetable(wavetable_)->factory_preset)
                {
                    
                }
                else {
                    setOptionSelected(std::clamp<int8_t>(option_selected_ + 1, MANAGE_MENU_EDIT, MANAGE_MENU_DELETE));

                    if(option_selected_ > option_offset_ + 1) {
                        option_offset_ = option_selected_ - 1;
                    }

                }
                break;
            case MANAGE_MENU_CONFIRM:
                setOptionSelected(std::clamp<int8_t>(option_selected_ + 1, MANAGE_MENU_NO, MANAGE_MENU_YES));
                break;
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CCW) {
        switch(state_) {
            case MANAGE_MENU_SELECT_WAVETABLE:
                morph_ = std::clamp<float>(morph_ - 1.0f / 30.0f, 0.0f, 1.0f);
                break;
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CW) {
        switch(state_) {
            case MANAGE_MENU_SELECT_WAVETABLE:
                morph_ = std::clamp<float>(morph_ + 1.0f / 30.0f, 0.0f, 1.0f);
                break;
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CLICK) {
        switch(state_) {
            case MANAGE_MENU_SELECT_WAVETABLE:
//                setState(MANAGE_MENU_SELECT_FRAME);
                break;
            case MANAGE_MENU_SELECT_FRAME:
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
        if(absorb_keypress_) {
            absorb_keypress_ = false;
            return true;
        }
        switch(state_) {
            case MANAGE_MENU_SELECT_WAVETABLE:
                if(copy_state_ == MANAGE_MENU_COPY_STATE_WAVETABLE && storage.GetWavetable(wavetable_)->factory_preset) {
                    popup.show();
                    popup.SetLine(0, (char*)"CANNOT OVERWRITE");
                    popup.SetLine(1, (char*)"FACTORY PRESETS!");
                    popup.SetLine(2, (char*)"\0");
                }
                else if(copy_state_ == MANAGE_MENU_COPY_STATE_WAVETABLE) {
                    // overwrite ok?
                    // TODO:  set yes func and no func
                    SetLine(0, (char*)"OVERWRITE?");
                    SetLine(1, (char*)"\0");
                    SetLine(2, (char*)"\0");
                    setCancelFunc(&ManageMenu::CancelCopyWavetable);
                    setConfirmFunc(&ManageMenu::ConfirmCopyWavetable);

                    option_selected_ = MANAGE_MENU_NO;
                    setState(MANAGE_MENU_CONFIRM);
                }
                else {
                    ResetTicker();
                    setState(MANAGE_MENU_SELECT_FRAME);
                    frame_ = 0;
                    frame_offset_ = 0;
                }
                break;
            case MANAGE_MENU_MOVE_WAVETABLE:
                setState(MANAGE_MENU_SELECT_WAVETABLE);
                break;
            case MANAGE_MENU_SELECT_FRAME: {
                if(copy_state_ == MANAGE_MENU_COPY_STATE_FRAME && storage.GetWavetable(wavetable_)->factory_preset) {
                    popup.show();
                    popup.SetLine(0, (char*)"CANNOT OVERWRITE");
                    popup.SetLine(1, (char*)"FACTORY PRESETS!");
                    popup.SetLine(2, (char*)"\0");
                }
                else if(copy_state_ == MANAGE_MENU_COPY_STATE_FRAME) {
                    // overwrite ok?
                    // TODO:  set yes func and no func
                    SetLine(0, (char*)"OVERWRITE?");
                    SetLine(1, (char*)"\0");
                    SetLine(2, (char*)"\0");
                    setCancelFunc(&ManageMenu::CancelCopyWave);
                    setConfirmFunc(&ManageMenu::ConfirmCopyWave);

                    option_selected_ = MANAGE_MENU_NO;
                    setState(MANAGE_MENU_CONFIRM);
                }
                break;
            }
            case MANAGE_MENU_MOVE_FRAME:
                setState(MANAGE_MENU_SELECT_FRAME);
                break;
            case MANAGE_MENU_WAVETABLE_OPTIONS:
                if(option_selected_ == MANAGE_MENU_EDIT) {
                    ResetTicker();
                    setState(MANAGE_MENU_SELECT_FRAME);
                    frame_ = 0;
                    frame_offset_ = 0;
                } else if(option_selected_ == MANAGE_MENU_COPY) {
                    copy_state_ = MANAGE_MENU_COPY_STATE_WAVETABLE;
                    blink_timer_ = system_clock.milliseconds();
                    selected_wavetable_ = wavetable_;
                    setState(MANAGE_MENU_SELECT_WAVETABLE);
                } else if(option_selected_ == MANAGE_MENU_RENAME) {
                    context.setState(&enterNameMenu);
                    enterNameMenu.setBackMenu(this);
                    enterNameMenu.setExecFunc(ManageMenu::SaveWavetable);
                    enterNameMenu.setNameChars(storage.GetWavetable(wavetable_)->name);
                } else if(option_selected_ == MANAGE_MENU_DELETE) {
                    option_selected_ = MANAGE_MENU_NO;
                    setState(MANAGE_MENU_CONFIRM);
                    // TODO:  set yes func and no func
                    SetLine(0, (char*)"DELETE?");
                    SetLine(1, (char*)"\0");
                    SetLine(2, (char*)"\0");
                    setCancelFunc(&ManageMenu::CancelDeleteWavetable);
                    setConfirmFunc(&ManageMenu::ConfirmDeleteWavetable);
                }
                break;
            case MANAGE_MENU_FRAME_OPTIONS:
                if(option_selected_ == MANAGE_MENU_EDIT) {
                    waveEditor.setBackMenu(this);
                    storage.LoadWaveSample(BUF5, wavetable_, frame_);
                    waveEditor.setWavedata(BUF5);
                    waveEditor.setWavetable(wavetable_);
                    waveEditor.setFrame(frame_);
                    context.setState(&waveEditor);
                    setState(MANAGE_MENU_SELECT_FRAME);
                } else if(option_selected_ == MANAGE_MENU_COPY) {
                    copy_state_ = MANAGE_MENU_COPY_STATE_FRAME;
                    blink_timer_ = system_clock.milliseconds();
                    selected_wavetable_ = wavetable_;
                    selected_frame_ = frame_;
                    setState(MANAGE_MENU_SELECT_FRAME);
                } else if(option_selected_ == MANAGE_MENU_RENAME) {
                    context.setState(&enterNameMenu);
                    enterNameMenu.setBackMenu(this);
                    enterNameMenu.setExecFunc(ManageMenu::SaveWave);
                    enterNameMenu.setNameChars(storage.GetWavetable(wavetable_)->waves[frame_].name);
                } else if(option_selected_ == MANAGE_MENU_DELETE) {
                    option_selected_ = MANAGE_MENU_NO;
                    setState(MANAGE_MENU_CONFIRM);
                    // TODO:  set yes func and no func
                    SetLine(0, (char*)"DELETE?");
                    SetLine(1, (char*)"\0");
                    SetLine(2, (char*)"\0");
                    setCancelFunc(&ManageMenu::CancelDeleteFrame);
                    setConfirmFunc(&ManageMenu::ConfirmDeleteFrame);
                }
                break;
            case MANAGE_MENU_CONFIRM:
                if(option_selected_ == MANAGE_MENU_NO) {
                    if(cancel_func_)
                        (this->*cancel_func_)();
//                    option_selected_ = MANAGE_MENU_EDIT;
//                    setState(MANAGE_MENU_WAVETABLE_OPTIONS);
                } else if(option_selected_ == MANAGE_MENU_YES) {
                    if(confirm_func_)
                        (this->*confirm_func_)();
//                    storage.DeleteWavetable(wavetable_);
//                    setState(MANAGE_MENU_SELECT_WAVETABLE);
                }
                break;
//            case MANAGE_MENU_CONFIRM_DELETE_WT:
//                if(option_selected_ == MANAGE_MENU_NO) {
//                    option_selected_ = MANAGE_MENU_EDIT;
//                    setState(MANAGE_MENU_WAVETABLE_OPTIONS);
//                } else if(option_selected_ == MANAGE_MENU_YES) {
//                    storage.DeleteWavetable(wavetable_);
//                    setState(MANAGE_MENU_SELECT_WAVETABLE);
//                }
//                break;
//            case MANAGE_MENU_CONFIRM_DELETE_WAVE:
//                if(option_selected_ == MANAGE_MENU_NO) {
//                    option_selected_ = MANAGE_MENU_EDIT;
//                    setState(MANAGE_MENU_FRAME_OPTIONS);
//                } else if(option_selected_ == MANAGE_MENU_YES) {
//                    storage.DeleteWave(wavetable_, frame_);
//                    setState(MANAGE_MENU_SELECT_FRAME);
//                }
//                break;
            default:
                break;
        }
    }
    if(key == BACK_BUTTON) {
        switch(state_) {
            case MANAGE_MENU_SELECT_WAVETABLE:
                if(copy_state_ != MANAGE_MENU_COPY_STATE_NONE) {
                    copy_state_ = MANAGE_MENU_COPY_STATE_NONE;
                    break;
                }
                if(back_menu_)
                    context.setState(back_menu_, true);
                else
                    context.setState(&mainMenu);
                break;
            case MANAGE_MENU_SELECT_FRAME:
                ResetTicker();
                setState(MANAGE_MENU_SELECT_WAVETABLE);
                morph_ = 0.0f;
                break;
            case MANAGE_MENU_WAVETABLE_OPTIONS:
                setState(MANAGE_MENU_SELECT_WAVETABLE);
                break;
            case MANAGE_MENU_FRAME_OPTIONS:
                setState(MANAGE_MENU_SELECT_FRAME);
                break;
            case MANAGE_MENU_CONFIRM:
                if(cancel_func_)
                    (this->*cancel_func_)();
//                option_selected_ = MANAGE_MENU_EDIT;
//                setState(MANAGE_MENU_WAVETABLE_OPTIONS);
                break;
//            case MANAGE_MENU_CONFIRM_DELETE_WT:
//                option_selected_ = MANAGE_MENU_EDIT;
//                setState(MANAGE_MENU_WAVETABLE_OPTIONS);
//                break;
//            case MANAGE_MENU_CONFIRM_DELETE_WAVE:
//                option_selected_ = MANAGE_MENU_EDIT;
//                setState(MANAGE_MENU_FRAME_OPTIONS);
//                break;
            default:
                break;
        }
    }

    return true;
}

void ManageMenu::SetLine(int line_no, char* str) {
    char* line = confirm_lines_[line_no];
    memset(line, 0, 20);
    strncpy(line, str, strlen(str));
}

void ManageMenu::ConfirmCopyWavetable() {
    storage.CopyWavetable(wavetable_, selected_wavetable_);
    setState(MANAGE_MENU_SELECT_WAVETABLE);
    copy_state_ = MANAGE_MENU_COPY_STATE_NONE;
}

void ManageMenu::CancelCopyWavetable() {
    setState(MANAGE_MENU_SELECT_WAVETABLE);
    copy_state_ = MANAGE_MENU_COPY_STATE_NONE;
}

void ManageMenu::ConfirmCopyWave() {
    storage.CopyWave(wavetable_, frame_, selected_wavetable_, selected_frame_);
    setState(MANAGE_MENU_SELECT_FRAME);
    copy_state_ = MANAGE_MENU_COPY_STATE_NONE;
    ResetTicker();
}

void ManageMenu::CancelCopyWave() {
    setState(MANAGE_MENU_SELECT_FRAME);
    copy_state_ = MANAGE_MENU_COPY_STATE_NONE;
    ResetTicker();
}

void ManageMenu::ConfirmDeleteWavetable() {
    storage.DeleteWavetable(wavetable_);
    setState(MANAGE_MENU_SELECT_WAVETABLE);
    ResetTicker();
}
void ManageMenu::CancelDeleteWavetable() {
    setState(MANAGE_MENU_SELECT_WAVETABLE);
    ResetTicker();
}


void ManageMenu::ConfirmDeleteFrame() {
    storage.DeleteWave(wavetable_, frame_);
    setState(MANAGE_MENU_SELECT_FRAME);
    ResetTicker();
}
void ManageMenu::CancelDeleteFrame() {
    setState(MANAGE_MENU_SELECT_FRAME);
    ResetTicker();
}

void ManageMenu::SaveWavetable(char* param) {
    storage.SaveWavetable(param, manageMenu.wavetable_);
    manageMenu.setState(MANAGE_MENU_SELECT_WAVETABLE);
    manageMenu.ResetTicker();
}

void ManageMenu::SaveWave(char* param) {
    Storage::WAVETABLE * wt = storage.GetWavetable(manageMenu.wavetable_);
    strncpy(wt->waves[manageMenu.frame_].name, param, 9);
    manageMenu.setState(MANAGE_MENU_SELECT_FRAME);
    manageMenu.ResetTicker();
}



void ManageMenu::paint(juce::Graphics& g) {
    Display::clear_screen();
    if(state_ == MANAGE_MENU_WAVETABLE_OPTIONS) {
        char * title;

        if(storage.GetWavetable(wavetable_)->name[0] == '\0')
            title = (char*) "--------";
        else
            title = (char *) storage.GetWavetable(wavetable_)->name;

        int y_offset = 3;
        int x_offset = 1 + 2 * 4;

        Display::put_string_5x5(x_offset, y_offset, strlen(title), title);
        
        Display::invert_rectangle(0, 0, 128, 11);
        
        x_offset = 20;
        y_offset = 18;
        
        if(storage.GetWavetable(wavetable_)->factory_preset) {
            Display::put_image_16bit(x_offset, y_offset, Graphic_icon_edit_11x11, 11);
            Display::put_string_9x9(x_offset + 16, y_offset + 1, strlen("COPY"), "COPY", option_selected_ == MANAGE_MENU_COPY, 3);
        }
        else {
            Display::put_image_16bit(x_offset, y_offset, Graphic_icon_edit_11x11, 11);
            Display::put_string_9x9(x_offset + 16, y_offset + 1, strlen("COPY"), "COPY", option_selected_ == MANAGE_MENU_COPY, 3);
            
            y_offset += 14;
            Display::put_image_16bit(x_offset, y_offset, Graphic_icon_rename_11x11, 11);
            Display::put_string_9x9(x_offset + 16, y_offset + 1, strlen("RENAME"), "RENAME", option_selected_ == MANAGE_MENU_RENAME, 3);
            
            y_offset += 14;
            Display::put_image_16bit(x_offset, y_offset, Graphic_icon_delete_11x11, 11);
            Display::put_string_9x9(x_offset + 16, y_offset + 1, strlen("DELETE"), "DELETE", option_selected_ == MANAGE_MENU_DELETE, 3);
        }
    }
    else if(state_ == MANAGE_MENU_FRAME_OPTIONS) {
        char * title;
        if(storage.GetWavetable(wavetable_)->waves[frame_].name[0] == '\0')
            title = (char*) "--------";
        else
            title = (char *) storage.GetWavetable(wavetable_)->waves[frame_].name;

        int y_offset = 3;
        int x_offset = 1 + 2 * 4;

        Display::put_string_5x5(x_offset, y_offset, strlen(title), title);
        
        Display::invert_rectangle(0, 0, 128, 11);
        
        x_offset = 20;
        y_offset = 18;
        
        if(storage.GetWavetable(wavetable_)->factory_preset) {
            Display::put_image_16bit(x_offset, y_offset, Graphic_icon_edit_11x11, 11);
            Display::put_string_9x9(x_offset + 16, y_offset + 1, strlen("COPY"), "COPY", option_selected_ == MANAGE_MENU_COPY, 3);
        }
        else {
            char *names[4] = {
                (char*)"EDIT",
                (char*)"COPY",
                (char*)"RENAME",
                (char*)"DELETE"
            };
            
            const uint8_t * list[] = {
                &Graphic_icon_edit_11x11[0][0],
                &Graphic_icon_copy_11x11[0][0],
                &Graphic_icon_rename_11x11[0][0],
                &Graphic_icon_delete_11x11[0][0],
            };
            
            y_offset += 7;
            for (int i = 0; i < 2; i++) {
                Display::put_image_16bit(x_offset, y_offset, (const unsigned char (*)[2])list[i + option_offset_], 11);
                Display::put_string_9x9(x_offset + 16, y_offset + 1, strlen(names[i + option_offset_]), names[i + option_offset_], option_selected_ == MANAGE_MENU_EDIT + i + option_offset_, 3);

                y_offset += 14;
            }
            
            y_offset = 20 + (64 - 20) / 2 - 14 * 3 / 2;
            y_offset += 7;

            if(option_offset_ > 0) {
//                y_offset = 20 + (64 - 20) / 2 - 14 * 3 / 2;
//                y_offset -= 4;
                Display::put_image_16bit(128 - 12, y_offset, Graphic_icon_arrow_up_9x9, 9);
            }
            y_offset += 14;
            if(option_offset_ < 2) {
//                y_offset = 64 - 9;
                Display::put_image_16bit(128 - 12, y_offset, Graphic_icon_arrow_down_9x9, 9);
            }

//            Display::put_image_16bit(x_offset, y_offset, Graphic_icon_rename_11x11, 11);
//            Display::put_string_9x9(x_offset + 16, y_offset + 1, strlen("RENAME"), "RENAME", option_selected_ == MANAGE_MENU_RENAME, 3);
//            
//            y_offset += 14;
//            Display::put_image_16bit(x_offset, y_offset, Graphic_icon_delete_11x11, 11);
//            Display::put_string_9x9(x_offset + 16, y_offset + 1, strlen("DELETE"), "DELETE", option_selected_ == MANAGE_MENU_DELETE, 3);
        }
    }
    else if(state_ == MANAGE_MENU_CONFIRM) {
        int y_offset = 16;
        int x_offset = 0;

        int line_count = 0;
        for(int i = 0; i < 3; i++) {
            if(strlen(confirm_lines_[i]) > 0)
                line_count++;
        }
        
        y_offset -= line_count * 6 / 2;
        
//        int y_offset = 32 - line_count * 3;
        
        for(int i = 0; i < 3; i++) {
            if(strlen(confirm_lines_[i]) > 0) {
                int x_offset = 64 - strlen(confirm_lines_[i]) * 6 / 2;
                Display::put_string_5x5(x_offset, y_offset, strlen(confirm_lines_[i]), confirm_lines_[i]);
            }
            y_offset += 6;
        }
//        Display::put_string_5x5(64 - strlen("DELETE?") * 6 / 2, y_offset, strlen("DELETE?"), "DELETE?");
        
        y_offset = 48 - 10 / 2;
        x_offset = 32 - (16 + Display::get_string_9x9_width("NO", 3)) / 2;
        Display::put_image_16bit(x_offset, y_offset, Graphic_icon_cancel_11x11, 11);
        Display::put_string_9x9(x_offset + 16, y_offset, strlen("NO"), "NO", option_selected_ == MANAGE_MENU_NO, 3);

        x_offset = 96 - (16 + Display::get_string_9x9_width("YES", 3)) / 2;
        Display::put_image_16bit(x_offset, y_offset, Graphic_icon_delete_11x11, 11);
        Display::put_string_9x9(x_offset + 16, y_offset, strlen("YES"), "YES", option_selected_ == MANAGE_MENU_YES, 3);
    }
    else if(state_ == MANAGE_MENU_MOVE_WAVETABLE || state_ == MANAGE_MENU_SELECT_WAVETABLE) {

        char * title;
        if(copy_state_ == MANAGE_MENU_COPY_STATE_WAVETABLE)
            title = (char *) "COPY WAVETABLE TO:";
        else if(copy_state_ == MANAGE_MENU_COPY_STATE_FRAME)
            title = (char *) "COPY WAVE TO:";
        else if(state_ == MANAGE_MENU_MOVE_WAVETABLE)
            title = (char *) "MOVE WAVETABLE";
        else
            title = (char *) "MANAGE WAVETABLE";
        
        int y_offset = 3;
        int x_offset = 1 + 2 * 4;

        Display::put_string_5x5(x_offset, y_offset, strlen(title), title);
        
        if(copy_state_ != MANAGE_MENU_COPY_STATE_WAVETABLE && copy_state_ != MANAGE_MENU_COPY_STATE_FRAME)
            Display::invert_rectangle(0, 0, 128, 11);
        else if((system_clock.milliseconds() - blink_timer_) % 1000 < 500)
            Display::invert_rectangle(0, 0, 128, 11);

        x_offset = 64 - 5;
        y_offset += 11;

        for(int i = 0; i < 6; i++)
        {
            char line[20];
            snprintf(line, 20, "%*d", 2, i + wavetable_offset_ + 1);
            Display::put_string_3x5(2, y_offset + i * 8, strlen(line), line);
            
            char * name = storage.GetWavetable(i + wavetable_offset_)->name;

            char * line2 = name;

            int32_t elapsed_time = system_clock.milliseconds() - ticker_timer_;

            int8_t num_chars = 7;

            if(i + wavetable_offset_ == wavetable_) {
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
                Display::put_string_5x5_loop(2 + 2 * 3 + 4, y_offset + i * 8, strlen(line2), line2, i+wavetable_offset_ == wavetable_, num_chars, strlen(line2) > num_chars ? ticker_ : 0);
            } else {
                Display::put_string_5x5_loop(2 + 2 * 3 + 4, y_offset + i * 8, strlen(line2), line2, i+wavetable_offset_ == wavetable_, num_chars, 0);
            }
        }

        int y_shift = 2;
        int bar_height = 64 - y_offset - 1;
        int y_cursor_offset = ((bar_height - 5) * wavetable_offset_) / (USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1 - 5);
        Display::outline_rectangle(x_offset+1, y_offset + 1 - y_shift + y_cursor_offset, 1, 3);
        Display::invert_rectangle(x_offset, y_offset - y_shift, 3, bar_height);

        storage.LoadWaveSample(BUF1, wavetable_, morph_);

        Display::Draw_Wave(64, y_offset - y_shift, 64, bar_height - 3, BUF1);

        int bar_width = 48;
        int x_cursor_offset = morph_ * (bar_width - 5);
        Display::outline_rectangle(95 - bar_width / 2 + 1 + x_cursor_offset, y_offset - y_shift + bar_height - 3 + 1, 3, 1);
        Display::invert_rectangle(95 - bar_width / 2, y_offset - y_shift + bar_height - 3, bar_width, 3);
    }
    else if(state_ == MANAGE_MENU_MOVE_FRAME || state_ == MANAGE_MENU_SELECT_FRAME) {
        char * title;
        if(state_ == MANAGE_MENU_MOVE_FRAME)
            title = (char *) "MOVE WAVE";
        if(copy_state_ == MANAGE_MENU_COPY_STATE_WAVETABLE)
            title = (char *) "COPY WAVETABLE TO:";
        else if(copy_state_ == MANAGE_MENU_COPY_STATE_FRAME)
            title = (char *) "COPY WAVE TO:";
        else
            title = (char *) storage.GetWavetable(wavetable_)->name;
        
        int y_offset = 3;
        int x_offset = 1 + 2 * 4;

        Display::put_string_5x5(x_offset, y_offset, strlen(title), title);
        
        if(copy_state_ != MANAGE_MENU_COPY_STATE_FRAME)
            Display::invert_rectangle(0, 0, 128, 11);
        else if((system_clock.milliseconds() - blink_timer_) % 1000 < 500)
            Display::invert_rectangle(0, 0, 128, 11);

        x_offset = 64 - 5;
        y_offset += 11;

        for(int i = 0; i < 6; i++)
        {
            char line[20];
            snprintf(line, 20, "%*d", 2, i + frame_offset_ + 1);
            Display::put_string_3x5(2, y_offset + i * 8, strlen(line), line);
            
            char * name = storage.GetWavetable(wavetable_)->waves[i + frame_offset_].name;

            char * line2 = name;

            int32_t elapsed_time = system_clock.milliseconds() - ticker_timer_;

            int8_t num_chars = 7;

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

        storage.LoadWaveSample(BUF1, wavetable_, frame_);

        Display::Draw_Wave(64, y_offset - y_shift, 64, bar_height - 3, BUF1);
    }
}
