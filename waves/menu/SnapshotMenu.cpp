/*
  ==============================================================================

    SnapshotMenu.cpp
    Created: 8 Dec 2023 8:42:12pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

SnapshotMenu::SnapshotMenu() {
    setState(SNAPSHOT_MENU_NONE);
    snapshot_ = 0;
    snapshot_offset_ = 0;
    function_selected_ = SNAPSHOT_MENU_DICE;
}

SnapshotMenu::~SnapshotMenu() {
    
}

void SnapshotMenu::triggerUpdate(bool back_pressed) {
    if(!back_pressed) {
        snapshot_offset_ = 0;
        snapshot_ = 0;
        return;
    }
    if(snapshot_ < snapshot_offset_) {
        snapshot_offset_ = snapshot_;
    }
    if(snapshot_ > snapshot_offset_ + 3) {
        snapshot_offset_ = snapshot_ - 3;
    }
    
    ResetTicker();

}

void SnapshotMenu::ResetTicker()
{
    ticker_timer_ = system_clock.milliseconds();
    ticker_ = 0;
}


bool SnapshotMenu::handleKeyLongPress(int key) {
    if( key == LEFT_ENCODER_CLICK ) {
        if(state_ == SNAPSHOT_MENU_NONE) {
            if(system_clock.milliseconds() - press_timer_ > 1000) {
                option_selected_ = SNAPSHOT_MENU_SAVE;
                setState(SNAPSHOT_MENU_OPTIONS);
                absorb_keypress_ = true;
                
                if(storage.GetSnapshot(snapshot_)->factory_preset) {
                    option_selected_ = SNAPSHOT_MENU_COPY;
                } else {
                    option_selected_ = SNAPSHOT_MENU_SAVE;
                }
                
                if(option_selected_ < option_offset_) {
                    option_offset_ = option_selected_;
                }
                if(option_selected_ > option_offset_ + 1) {
                    option_offset_ = option_selected_ - 1;
                }

                return true;
            }
        }
    }
    return false;
}

bool SnapshotMenu::handleKeyPress(int key) {
    if( key == LEFT_ENCODER_CLICK ) {
        press_timer_ = system_clock.milliseconds();
    }
    return false;
}

bool SnapshotMenu::handleKeyRelease(int key) {
    if(key == LEFT_ENCODER_CCW) {
        switch(state_) {
            case SNAPSHOT_MENU_OPTIONS:
                if(storage.GetSnapshot(snapshot_)->factory_preset)
                {
                    
                }
                else
                    setOptionSelected(std::clamp<int8_t>(option_selected_ - 1, SNAPSHOT_MENU_SAVE, SNAPSHOT_MENU_DELETE));
                
                if(option_selected_ < option_offset_) {
                    option_offset_ = option_selected_;
                }

                break;
            case SNAPSHOT_MENU_NONE:
                snapshot_ = std::clamp<int16_t>(snapshot_ - 1, 0, USER_SNAPSHOT_COUNT - 1);
                
                if(snapshot_ < snapshot_offset_) {
                    snapshot_offset_ = snapshot_;
                }

                ResetTicker();
                break;
            case SNAPSHOT_MENU_CONFIRM:
                setOptionSelected(std::clamp<int8_t>(option_selected_ - 1, SNAPSHOT_MENU_NO, SNAPSHOT_MENU_YES));
                break;
            default:
                break;
        }
    }
    if(key == LEFT_ENCODER_CW) {
        switch(state_) {
            case SNAPSHOT_MENU_OPTIONS:
                if(storage.GetSnapshot(snapshot_)->factory_preset)
                {
                    
                }
                else
                    setOptionSelected(std::clamp<int8_t>(option_selected_ + 1, SNAPSHOT_MENU_SAVE, SNAPSHOT_MENU_DELETE));
                
                if(option_selected_ > option_offset_ + 1) {
                    option_offset_ = option_selected_ - 1;
                }

                break;
            case SNAPSHOT_MENU_NONE:
                snapshot_ = std::clamp<int16_t>(snapshot_ + 1, 0, USER_SNAPSHOT_COUNT - 1);
                
                if(snapshot_ > snapshot_offset_ + 3) {
                    snapshot_offset_ = snapshot_ - 3;
                }

                ResetTicker();
                break;
            case SNAPSHOT_MENU_CONFIRM:
                setOptionSelected(std::clamp<int8_t>(option_selected_ + 1, SNAPSHOT_MENU_NO, SNAPSHOT_MENU_YES));
                break;
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CCW) {
        if(function_selected_ == SNAPSHOT_MENU_HOME)
            function_selected_ = SNAPSHOT_MENU_DICE;
    }
    if(key == RIGHT_ENCODER_CW) {
        if(function_selected_ == SNAPSHOT_MENU_DICE)
            function_selected_ = SNAPSHOT_MENU_HOME;
    }
    if(key == RIGHT_ENCODER_CLICK) {
        if(function_selected_ == SNAPSHOT_MENU_HOME) {
            user_settings.ResetSettings();
            popup.show();
            popup.SetLine(0, (char*)"RESET!");
            popup.SetLine(1, (char*)"\0");
            popup.SetLine(2, (char*)"\0");
        } else if(function_selected_ == SNAPSHOT_MENU_DICE) {
            user_settings.RandomizeSettings();
            popup.show();
            popup.SetLine(0, (char*)"RANDOMIZED!");
            popup.SetLine(1, (char*)"\0");
            popup.SetLine(2, (char*)"\0");
        }
    }
    if(key == LEFT_ENCODER_CLICK) {
        if(absorb_keypress_) {
            absorb_keypress_ = false;
            return true;
        }

        switch(state_) {
            case SNAPSHOT_MENU_NONE: {
                // TODO: recall snapshot
                if(copy_state_ == SNAPSHOT_MENU_COPY_STATE_SNAPSHOT && storage.GetSnapshot(snapshot_)->factory_preset) {
                    popup.show();
                    popup.SetLine(0, (char*)"CANNOT OVERWRITE");
                    popup.SetLine(1, (char*)"FACTORY PRESETS!");
                    popup.SetLine(2, (char*)"\0");
                }
                else if(copy_state_ == SNAPSHOT_MENU_COPY_STATE_SNAPSHOT) {
                    SetLine(0, (char*)"OVERWRITE?");
                    SetLine(1, (char*)"\0");
                    SetLine(2, (char*)"\0");
                    setCancelFunc(&SnapshotMenu::CancelCopy);
                    setConfirmFunc(&SnapshotMenu::ConfirmCopy);

                    option_selected_ = SNAPSHOT_MENU_NO;
                    setState(SNAPSHOT_MENU_CONFIRM);
                }
                else {
                    user_settings.LoadSnapshot(snapshot_);
                    context.setState(&oscilloscope);
                    oscilloscope.setBackMenu(this);
//                    setState(MANAGE_MENU_SELECT_FRAME);
//                    frame_ = 0;
//                    frame_offset_ = 0;
                }
                break;
            }
            case SNAPSHOT_MENU_OPTIONS:
                if(option_selected_ == SNAPSHOT_MENU_SAVE) {
                    if(storage.GetSnapshot(snapshot_)->is_empty) {
                        // TODO: open name menu save
                        context.setState(&enterNameMenu);
                        enterNameMenu.setBackMenu(this);
                        enterNameMenu.setExecFunc(SnapshotMenu::Save);
                        enterNameMenu.setNameChars(storage.GetSnapshot(snapshot_)->name);
                    } else {
                        // resave. no menu
                        Save(storage.GetSnapshot(snapshot_)->name);
                    }
                } else if(option_selected_ == SNAPSHOT_MENU_COPY) {
                    copy_state_ = SNAPSHOT_MENU_COPY_STATE_SNAPSHOT;
                    blink_timer_ = system_clock.milliseconds();
                    selected_snapshot_ = snapshot_;
                    setState(SNAPSHOT_MENU_NONE);
                } else if(option_selected_ == SNAPSHOT_MENU_RENAME) {
                    context.setState(&enterNameMenu);
                    enterNameMenu.setBackMenu(this);
                    enterNameMenu.setExecFunc(SnapshotMenu::Rename);
                    enterNameMenu.setNameChars(storage.GetSnapshot(snapshot_)->name);
                } else if(option_selected_ == SNAPSHOT_MENU_DELETE) {
                    option_selected_ = SNAPSHOT_MENU_NO;
                    setState(SNAPSHOT_MENU_CONFIRM);
                    // TODO:  set yes func and no func
                    SetLine(0, (char*)"DELETE?");
                    SetLine(1, (char*)"\0");
                    SetLine(2, (char*)"\0");
                    setCancelFunc(&SnapshotMenu::CancelDelete);
                    setConfirmFunc(&SnapshotMenu::ConfirmDelete);
                }
                break;
            case SNAPSHOT_MENU_CONFIRM:
                if(option_selected_ == SNAPSHOT_MENU_NO) {
                    if(cancel_func_)
                        (this->*cancel_func_)();
                } else if(option_selected_ == SNAPSHOT_MENU_YES) {
                    if(confirm_func_)
                        (this->*confirm_func_)();
                }
                break;
            default:
                break;
        }
    }
    if(key == BACK_BUTTON) {
        switch(state_) {
            case SNAPSHOT_MENU_NONE:
                if(copy_state_ == SNAPSHOT_MENU_COPY_STATE_SNAPSHOT) {
                    copy_state_ = SNAPSHOT_MENU_COPY_STATE_NONE;
                    break;
                }
                if(back_menu_)
                    context.setState(back_menu_, true);
                else
                    context.setState(&mainMenu);
                break;
            case SNAPSHOT_MENU_OPTIONS:
                setState(SNAPSHOT_MENU_NONE);
                break;
            case SNAPSHOT_MENU_CONFIRM:
                if(cancel_func_)
                    (this->*cancel_func_)();
                break;
            default:
                break;
        }
    }

    return true;
}

//void SnapshotMenu::SaveWavetable(char* param) {
//    storage.SaveWavetable(param, saveWaveMenu.wavetable_);
//}
//
//void SnapshotMenu::SaveWave(char* param) {
//    storage.SaveWave(param, saveWaveMenu.wavedata_, saveWaveMenu.wavetable_, saveWaveMenu.frame_);
//}

void SnapshotMenu::paint(juce::Graphics& g) {
    Display::clear_screen();
    if(state_ == SNAPSHOT_MENU_NONE) {

        char * title = (char *) "SNAPSHOT";

        int y_offset = 3;
        int x_offset = 1 + 2 * 4;

        int func_width = 128 - 92;

        if(copy_state_ == SNAPSHOT_MENU_COPY_STATE_SNAPSHOT) {
            title = (char *) "COPY SNAPSHOT TO:";
        
            int x_offset = 1 + 2 * 4;

            Display::put_string_5x5(x_offset, y_offset, strlen(title), title);
        } else {
            Display::put_string_5x5((18 + 11 * 7) / 2 - strlen(title) * 6 / 2, y_offset, strlen(title), title);
            
            Display::put_string_5x5(92 + func_width / 2 - strlen("FUNC") * 6 / 2, y_offset, strlen("FUNC"), "FUNC");
        }

        if(copy_state_ != SNAPSHOT_MENU_COPY_STATE_SNAPSHOT)
            Display::invert_rectangle(0, 0, 128, 11);
        else if((system_clock.milliseconds() - blink_timer_) % 1000 < 500)
            Display::invert_rectangle(0, 0, 128, 11);
        
        x_offset = 128 - 5;
        y_offset += 11;
        
        //9 * 4 + 5 * 3
        y_offset += 3;

        for(int i = 0; i < 4; i++)
        {
            char line[20];
            memset(line, 0, 20);
            snprintf(line, 20, "%*d", 2, i + snapshot_offset_ + 1);
            Display::put_string_3x5(2, y_offset + i * 12, strlen(line), line);
            
            char * line2;
            
            line2 = storage.GetSnapshot(i + snapshot_offset_)->name;

            int32_t elapsed_time = system_clock.milliseconds() - ticker_timer_;

            int8_t num_chars = 6;

            if(i + snapshot_offset_ == snapshot_) {
                if(ticker_ == 0) {
                    if(elapsed_time > 1000) {
                        ticker_++;
                        ticker_timer_ = system_clock.milliseconds();
                    }
                } else if(ticker_ == (strlen(line2) - num_chars) * 9 + ((strlen(line2) - num_chars)) * 2) {
                    if(elapsed_time > 2000) {
                        ResetTicker();
                    }
                }
                else if (elapsed_time > 20) {
                    ticker_++;
                    ticker_timer_ = system_clock.milliseconds();
                }
                Display::put_string_9x9_loop(2 + 2 * 3 + 4, y_offset + i * 12 - 2, strlen(line2), line2, true, 2, num_chars, strlen(line2) > num_chars ? ticker_ : 0);
            } else {
                Display::put_string_9x9_loop(2 + 2 * 3 + 4, y_offset + i * 12 - 2, strlen(line2), line2, false, 2, num_chars, 0);
            }
        }

        y_offset = 12;
        int bar_height = 50;
        int y_cursor_offset = ((bar_height - 5) * snapshot_offset_) / (USER_SNAPSHOT_COUNT + FACTORY_SNAPSHOT_COUNT - 1 - 3);
        x_offset = 12 + 11 * 7;
        Display::outline_rectangle(x_offset+1, y_offset + 1 + y_cursor_offset, 1, 3);
        Display::outline_rectangle(x_offset, y_offset, 3, bar_height);

        x_offset += 3;
        int action_width = 128 - x_offset;
        y_offset = 11;

        int action_height = 64 - y_offset;

        int center_x = x_offset + action_width / 2;
        int center_y = y_offset + action_height / 4;
        Display::put_image_22x23(center_x - 23 / 2, center_y - 22 / 2, Graphic_snapshot_menu_dice);
        if(function_selected_ == SNAPSHOT_MENU_DICE)
            Display::invert_rectangle(center_x - 23 / 2 + 1, center_y - 22 / 2 + 1, 21, 20);

        center_y = y_offset + 3 * action_height / 4;
        Display::put_image_22x23(center_x - 23 / 2, center_y - 22 / 2, Graphic_snapshot_menu_home);
        if(function_selected_ == SNAPSHOT_MENU_HOME)
            Display::invert_rectangle(center_x - 23 / 2 + 1, center_y - 22 / 2 + 1, 21, 20);

    } else if(state_ == SNAPSHOT_MENU_OPTIONS) {
        char * title;
        title = (char *) storage.GetSnapshot(snapshot_)->name;

        int y_offset = 3;
        int x_offset = 1 + 2 * 4;

        Display::put_string_5x5(x_offset, y_offset, strlen(title), title);
        
        Display::invert_rectangle(0, 0, 128, 11);
        
        x_offset = 20;
        y_offset = 20 + (64 - 20) / 2 - 14 * 3 / 2;
        
        if(storage.GetSnapshot(snapshot_)->factory_preset) {
            Display::put_image_16bit(x_offset, y_offset, Graphic_icon_edit_11x11, 11);
            Display::put_string_9x9(x_offset + 16, y_offset + 1, strlen("COPY"), "COPY", option_selected_ == SNAPSHOT_MENU_COPY, 3);
        }
        else {
            char *names[4] = {
                (char*)"SAVE",
                (char*)"COPY",
                (char*)"RENAME",
                (char*)"DELETE"
            };
            if(!storage.GetSnapshot(snapshot_)->is_empty) {
                names[0] = (char*)"RESAVE";
            }
            
            const uint8_t * list[] = {
                &Graphic_icon_edit_11x11[0][0],
                &Graphic_icon_copy_11x11[0][0],
                &Graphic_icon_rename_11x11[0][0],
                &Graphic_icon_delete_11x11[0][0],
            };
            
            y_offset += 7;
            for (int i = 0; i < 2; i++) {
                Display::put_image_16bit(x_offset, y_offset, (const unsigned char (*)[2])list[i + option_offset_], 11);
                Display::put_string_9x9(x_offset + 16, y_offset + 1, strlen(names[i + option_offset_]), names[i + option_offset_], option_selected_ == SNAPSHOT_MENU_SAVE + i + option_offset_, 3);

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
    } else if(state_ == SNAPSHOT_MENU_CONFIRM) {
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
        Display::put_string_9x9(x_offset + 16, y_offset, strlen("NO"), "NO", option_selected_ == SNAPSHOT_MENU_NO, 3);
        
        x_offset = 96 - (16 + Display::get_string_9x9_width("YES", 3)) / 2;
        Display::put_image_16bit(x_offset, y_offset, Graphic_icon_delete_11x11, 11);
        Display::put_string_9x9(x_offset + 16, y_offset, strlen("YES"), "YES", option_selected_ == SNAPSHOT_MENU_YES, 3);
    }
}

void SnapshotMenu::ConfirmCopy() {
    storage.SaveSnapshot(storage.GetSnapshot(selected_snapshot_)->name, snapshot_, storage.GetSnapshot(selected_snapshot_));
    setState(SNAPSHOT_MENU_NONE);
    copy_state_ = SNAPSHOT_MENU_COPY_STATE_NONE;
    ResetTicker();
}

void SnapshotMenu::CancelCopy() {
    setState(SNAPSHOT_MENU_NONE);
    copy_state_ = SNAPSHOT_MENU_COPY_STATE_NONE;
}

void SnapshotMenu::ConfirmDelete() {
    storage.EraseSnapshot(storage.GetSnapshot(snapshot_), snapshot_, true);
    setState(SNAPSHOT_MENU_NONE);
}

void SnapshotMenu::CancelDelete() {
    setState(SNAPSHOT_MENU_NONE);
}

void SnapshotMenu::Save(char* param) {
    storage.SaveSnapshot(param, snapshotMenu.snapshot_, user_settings.settings_ptr());
    snapshotMenu.setState(SNAPSHOT_MENU_NONE);
    snapshotMenu.ResetTicker();
}

void SnapshotMenu::Rename(char* param) {
    storage.SaveSnapshot(param, snapshotMenu.snapshot_, storage.GetSnapshot(snapshotMenu.snapshot_));
    snapshotMenu.setState(SNAPSHOT_MENU_NONE);
    snapshotMenu.ResetTicker();
}


void SnapshotMenu::SetLine(int line_no, char* str) {
    char* line = confirm_lines_[line_no];
    memset(line, 0, 20);
    strncpy(line, str, strlen(str));
}
