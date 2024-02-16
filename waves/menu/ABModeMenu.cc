/*
  ==============================================================================

    ABWaveSelectMenu.cpp
    Created: 18 Nov 2023 8:29:46pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

namespace waves {

ABModeMenu::ABModeMenu() {
    left_state_ = AB_LOAD_HOVER;
    right_state_ = AB_LOAD_HOVER;
    left_wavetable_ = 0;
    left_frame_ = 0;
    right_wavetable_ = 0;
    right_frame_ = 0;
    left_wavetable_offset_ = 0;
    left_frame_offset_ = 0;
    right_wavetable_offset_ = 0;
    right_frame_offset_ = 0;
    active_menu_ = LEFT;

    left_ticker_timer_ = 0;
    right_ticker_timer_ = 0;
    left_ticker_ = 0;
    right_ticker_ = 0;
}

ABModeMenu::~ABModeMenu() {
    
}

void ABModeMenu::triggerUpdate(bool back_pressed) {
    if(!back_pressed) {
        left_wavetable_offset_ = 0;
        left_wavetable_ = 0;
        left_frame_ = 0;
        left_frame_offset_ = 0;

        right_wavetable_offset_ = 0;
        right_wavetable_ = 0;
        right_frame_ = 0;
        right_frame_offset_ = 0;

        flash.StartFrameDMARead((uint32_t*)left_wavetable_names_, 16 * 9, 0, NULL, EEPROM_PERSISTENT_SS);
        flash.StartFrameDMARead((uint32_t*)right_wavetable_names_, 16 * 9, 0, NULL, EEPROM_PERSISTENT_SS);
        flash.StartFrameDMARead((uint32_t*)left_frame_names_, 16 * 9, 16 * 9 + 16 * 9 * left_wavetable_, NULL, EEPROM_PERSISTENT_SS);
        flash.StartFrameDMARead((uint32_t*)right_frame_names_, 16 * 9, 16 * 9 + 16 * 9 * right_wavetable_, NULL, EEPROM_PERSISTENT_SS);

        return;
    }

    if(left_wavetable_ < left_wavetable_offset_) {
        left_wavetable_offset_ = left_wavetable_;
    }

    if(left_frame_ < left_frame_offset_) {
        left_frame_offset_ = left_frame_;
    }
    
    if(left_wavetable_ > left_wavetable_offset_ + 2) {
        left_wavetable_offset_ = left_wavetable_ - 2;
    }

    if(left_frame_ > left_frame_offset_ + 2) {
        left_frame_offset_ = left_frame_ - 2;
    }

    if(right_wavetable_ < right_wavetable_offset_) {
        right_wavetable_offset_ = right_wavetable_;
    }

    if(right_frame_ < right_frame_offset_) {
        right_frame_offset_ = right_frame_;
    }
    
    if(right_wavetable_ > right_wavetable_offset_ + 2) {
        right_wavetable_offset_ = right_wavetable_ - 2;
    }

    if(right_frame_ > right_frame_offset_ + 2) {
        right_frame_offset_ = right_frame_ - 2;
    }

    flash.StartFrameDMARead((uint32_t*)left_wavetable_names_, 16 * 9, 0, NULL, EEPROM_PERSISTENT_SS);
    flash.StartFrameDMARead((uint32_t*)right_wavetable_names_, 16 * 9, 0, NULL, EEPROM_PERSISTENT_SS);
    flash.StartFrameDMARead((uint32_t*)left_frame_names_, 16 * 9, 16 * 9 + 16 * 9 * left_wavetable_, NULL, EEPROM_PERSISTENT_SS);
    flash.StartFrameDMARead((uint32_t*)right_frame_names_, 16 * 9, 16 * 9 + 16 * 9 * right_wavetable_, NULL, EEPROM_PERSISTENT_SS);
}
//AB_NONE,
//AB_LOAD_HOVER,
//AB_EDIT_HOVER,
//AB_SELECT_WAVETABLE,
//AB_SELECT_FRAME,
void ABModeMenu::ResetTicker(int8_t side) {
    if(side == 0) {
        left_ticker_timer_ = system_clock.milliseconds();
        left_ticker_ = 0;
    } else {
        right_ticker_timer_ = system_clock.milliseconds();
        right_ticker_ = 0;
    }
}

void ABModeMenu::on_load_one_ab_left_finished() {
    waveEditor.setWavedata(&front_buffer_2[0]);
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}
void ABModeMenu::on_load_one_ab_right_finished() {
    waveEditor.setWavedata(&front_buffer_2[2048]);
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}


void ABModeMenu::on_load_left_wavetable_names_finished() {
    abModeMenu.left_state_ = AB_SELECT_WAVETABLE;
    abModeMenu.ResetTicker(0);
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}

void ABModeMenu::on_load_right_wavetable_names_finished() {
    abModeMenu.right_state_ = AB_SELECT_WAVETABLE;
    abModeMenu.ResetTicker(1);
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}

void ABModeMenu::on_load_left_frame_names_finished() {
    abModeMenu.left_state_ = AB_SELECT_FRAME;
    abModeMenu.ResetTicker(0);
    if(abModeMenu.left_wavetable_ != abEngine.GetLeftWavetable()) {
        abModeMenu.left_frame_offset_ = 0;
        abModeMenu.left_frame_ = 0;
    }
    else {
        abModeMenu.left_frame_offset_ = CLAMP(abEngine.GetLeftFrame(), 0, 15-2);
        abModeMenu.left_frame_ = abEngine.GetLeftFrame();
    }
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}

void ABModeMenu::on_load_right_frame_names_finished() {
    abModeMenu.right_state_ = AB_SELECT_FRAME;
    abModeMenu.ResetTicker(1);
    if(abModeMenu.right_wavetable_ != abEngine.GetRightWavetable()) {
        abModeMenu.right_frame_offset_ = 0;
        abModeMenu.right_frame_ = 0;
    }
    else {
        abModeMenu.right_frame_offset_ = CLAMP(abEngine.GetRightFrame(), 0, 15-2);
        abModeMenu.right_frame_ = abEngine.GetRightFrame();
    }
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}


bool ABModeMenu::handleKeyRelease(int key) {
    if(key == LEFT_ENCODER_CCW) {
        active_menu_ = LEFT;
        switch(left_state_) {
            case AB_LOAD_HOVER:
                break;
            case AB_EDIT_HOVER:
                left_state_ = AB_LOAD_HOVER;
                break;
            case AB_SELECT_WAVETABLE:
                left_wavetable_ = CLAMP(left_wavetable_ - 1, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
                
                if(left_wavetable_ < left_wavetable_offset_) {
                    left_wavetable_offset_ = left_wavetable_;
                }
                
                if(abEngine.GetLeftWavetable() == left_wavetable_)
                    left_frame_ = abEngine.GetLeftFrame();
                else
                    left_frame_ = 0;

                ResetTicker(0);
                break;
            case AB_SELECT_FRAME:
                left_frame_ = CLAMP(left_frame_ - 1, 0, 15);

                if(left_frame_ < left_frame_offset_) {
                    left_frame_offset_ = left_frame_;
                }

                ResetTicker(0);
                break;
            default:
                break;
        }
    }
    if(key == LEFT_ENCODER_CW) {
        active_menu_ = LEFT;
        switch(left_state_) {
            case AB_LOAD_HOVER:
                left_state_ = AB_EDIT_HOVER;
                break;
            case AB_EDIT_HOVER:
                break;
            case AB_SELECT_WAVETABLE:
                left_wavetable_ = CLAMP(left_wavetable_ + 1, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);

                if(left_wavetable_ > left_wavetable_offset_ + 2) {
                    left_wavetable_offset_ = left_wavetable_ - 2;
                }
                
                if(abEngine.GetLeftWavetable() == left_wavetable_)
                    left_frame_ = abEngine.GetLeftFrame();
                else
                    left_frame_ = 0;
                
                ResetTicker(0);
                break;
            case AB_SELECT_FRAME:
                left_frame_ = CLAMP(left_frame_ + 1, 0, 15);

                if(left_frame_ > left_frame_offset_ + 2) {
                    left_frame_offset_ = left_frame_ - 2;
                }
                ResetTicker(0);

                break;
            default:
                break;
        }
    }
    if(key == LEFT_ENCODER_CLICK) {
        active_menu_ = LEFT;
        switch(left_state_) {
            case AB_LOAD_HOVER:
                flash.StartFrameDMARead((uint32_t*)left_wavetable_names_, 16 * 9, 0, ABModeMenu::on_load_left_wavetable_names_finished, EEPROM_PERSISTENT_SS);
                // left_state_ = AB_SELECT_WAVETABLE;
                // ResetTicker(0);
                break;
            case AB_EDIT_HOVER:
                if(!abEngine.IsEditingLeft()) {
                    // flash.StopDMA(true);
                    flash.StartFrameDMARead((uint32_t*)&front_buffer_2[0], 4096, abEngine.GetLeftWavetable() * 65536 + abEngine.GetLeftFrame() * 4096, ABModeMenu::on_load_one_ab_left_finished);
                    // abEngine.FillWaveform(&front_buffer_2[0], abEngine.GetLeftWavetable(), abEngine.GetLeftFrame());
                    abEngine.SetIsEditingLeft(true);
                }
                waveEditor.setWavedata(&front_buffer_2[0]);
                context.setState(&waveEditor);
                break;
            case AB_SELECT_WAVETABLE:
                flash.StartFrameDMARead((uint32_t*)left_frame_names_, 16 * 9, 16 * 9 + 16 * 9 * left_wavetable_, ABModeMenu::on_load_left_frame_names_finished, EEPROM_PERSISTENT_SS);
                // left_state_ = AB_SELECT_FRAME;
                // ResetTicker(0);
                // if(left_wavetable_ != abEngine.GetLeftWavetable()) {
                //     left_frame_offset_ = 0;
                //     left_frame_ = 0;
                // }
                // else {
                //     left_frame_offset_ = CLAMP(abEngine.GetLeftFrame(), 0, 15-2);
                //     left_frame_ = abEngine.GetLeftFrame();
                // }
                break;
            case AB_SELECT_FRAME:
                abEngine.SetIsEditingLeft(false);
                abEngine.SetLeftWave(left_wavetable_, left_frame_);
                break;
            default:
                break;
        }
    }
    // *************************************************************************** //
    // RIGHT MENU //
    // *************************************************************************** //
    if(key == RIGHT_ENCODER_CCW) {
        active_menu_ = RIGHT;
        switch(right_state_) {
            case AB_LOAD_HOVER:
                break;
            case AB_EDIT_HOVER:
                right_state_ = AB_LOAD_HOVER;
                break;
            case AB_SELECT_WAVETABLE:
                right_wavetable_ = CLAMP(right_wavetable_ - 1, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);
                
                if(right_wavetable_ < right_wavetable_offset_) {
                    right_wavetable_offset_ = right_wavetable_;
                }
                
                if(abEngine.GetRightWavetable() == right_wavetable_)
                    right_frame_ = abEngine.GetRightFrame();
                else
                    right_frame_ = 0;

                ResetTicker(1);
                break;
            case AB_SELECT_FRAME:
                right_frame_ = CLAMP(right_frame_ - 1, 0, 15);

                if(right_frame_ < right_frame_offset_) {
                    right_frame_offset_ = right_frame_;
                }

                ResetTicker(1);
                break;
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CW) {
        active_menu_ = RIGHT;
        switch(right_state_) {
            case AB_LOAD_HOVER:
                right_state_ = AB_EDIT_HOVER;
                break;
            case AB_EDIT_HOVER:
                break;
            case AB_SELECT_WAVETABLE:
                right_wavetable_ = CLAMP(right_wavetable_ + 1, 0, USER_WAVETABLE_COUNT + FACTORY_WAVETABLE_COUNT - 1);

                if(right_wavetable_ > right_wavetable_offset_ + 2) {
                    right_wavetable_offset_ = right_wavetable_ - 2;
                }
                
                if(abEngine.GetRightWavetable() == right_wavetable_)
                    right_frame_ = abEngine.GetRightFrame();
                else
                    right_frame_ = 0;
                
                ResetTicker(1);
                break;
            case AB_SELECT_FRAME:
                right_frame_ = CLAMP(right_frame_ + 1, 0, 15);

                if(right_frame_ > right_frame_offset_ + 2) {
                    right_frame_offset_ = right_frame_ - 2;
                }
                ResetTicker(1);

                break;
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CLICK) {
        active_menu_ = RIGHT;
        switch(right_state_) {
            case AB_LOAD_HOVER:
                flash.StartFrameDMARead((uint32_t*)right_wavetable_names_, 16 * 9, 0, ABModeMenu::on_load_right_wavetable_names_finished, EEPROM_PERSISTENT_SS);
                // right_state_ = AB_SELECT_WAVETABLE;
                // ResetTicker(1);
                break;
            case AB_EDIT_HOVER:
                if(!abEngine.IsEditingRight()) {

                    // flash.StopDMA(true);
                    flash.StartFrameDMARead((uint32_t*)&front_buffer_2[2048], 4096, abEngine.GetRightWavetable() * 65536 + abEngine.GetRightFrame() * 4096, ABModeMenu::on_load_one_ab_right_finished);

                    // abEngine.FillWaveform(&front_buffer_2[2048], abEngine.GetRightWavetable(), abEngine.GetRightFrame());
                    abEngine.SetIsEditingRight(true);
                }
                waveEditor.setWavedata(&front_buffer_2[2048]);
                context.setState(&waveEditor);
                break;
            case AB_SELECT_WAVETABLE:
                flash.StartFrameDMARead((uint32_t*)right_frame_names_, 16 * 9, 16 * 9 + 16 * 9 * right_wavetable_, ABModeMenu::on_load_right_frame_names_finished, EEPROM_PERSISTENT_SS);
                // right_state_ = AB_SELECT_FRAME;
                // ResetTicker(1);
                // if(right_wavetable_ != abEngine.GetRightWavetable()) {
                //     right_frame_offset_ = 0;
                //     right_frame_ = 0;
                // }
                // else {
                //     right_frame_offset_ = CLAMP(abEngine.GetRightFrame(), 0, 15-2);
                //     right_frame_ = abEngine.GetRightFrame();
                // }
                break;
            case AB_SELECT_FRAME:
                abEngine.SetIsEditingRight(false);
                abEngine.SetRightWave(right_wavetable_, right_frame_);
                break;
            default:
                break;
        }
    }
    if(key == BACK_BUTTON) {
        if(active_menu_ == LEFT) {
            switch(left_state_) {
                case AB_LOAD_HOVER:
                    if(back_menu_)
                        context.setState(back_menu_);
                    else
                        context.setState(&mainMenu);
                    break;
                case AB_EDIT_HOVER:
                    if(back_menu_)
                        context.setState(back_menu_);
                    else
                        context.setState(&mainMenu);
                    break;
                case AB_SELECT_WAVETABLE:
                    left_state_ = AB_LOAD_HOVER;
                    break;
                case AB_SELECT_FRAME:
                    ResetTicker(0);
                    left_state_ = AB_SELECT_WAVETABLE;
                    break;
                default:
                    break;
            }
        }
        if(active_menu_ == RIGHT) {
            switch(right_state_) {
                case AB_LOAD_HOVER:
                    if(back_menu_)
                        context.setState(back_menu_);
                    else
                        context.setState(&mainMenu);
                    break;
                case AB_EDIT_HOVER:
                    if(back_menu_)
                        context.setState(back_menu_);
                    else
                        context.setState(&mainMenu);
                    break;
                case AB_SELECT_WAVETABLE:
                    right_state_ = AB_LOAD_HOVER;
                    break;
                case AB_SELECT_FRAME:
                    ResetTicker(1);
                    right_state_ = AB_SELECT_WAVETABLE;
                    break;
                default:
                    break;
            }
        }    }

    return true;
}

void ABModeMenu::on_load_left_gui_finished() {
    if(abModeMenu.left_state_ == AB_SELECT_FRAME || abModeMenu.left_state_ == AB_SELECT_WAVETABLE) {
        abModeMenu.left_wavetable_gui_ = abModeMenu.left_wavetable_;
        abModeMenu.left_frame_gui_ = abModeMenu.left_frame_;
    } else {
        abModeMenu.left_wavetable_gui_ = abEngine.GetLeftWavetable();
        abModeMenu.left_frame_gui_ = abEngine.GetLeftFrame();
    }
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}

void ABModeMenu::on_load_right_gui_finished() {
    if(abModeMenu.right_state_ == AB_SELECT_FRAME || abModeMenu.right_state_ == AB_SELECT_WAVETABLE) {
        abModeMenu.right_wavetable_gui_ = abModeMenu.right_wavetable_;
        abModeMenu.right_frame_gui_ = abModeMenu.right_frame_;
    } else {
        abModeMenu.right_wavetable_gui_ = abEngine.GetRightWavetable();
        abModeMenu.right_frame_gui_ = abEngine.GetRightFrame();
    }
    SetFlag(&_EREG_, _RXNE_, FLAG_CLEAR);
    SetFlag(&_EREG_, _BUSY_, FLAG_CLEAR);
}


void ABModeMenu::DrawSide(int side) {
    int graph_y_offset = 3;
    int graph_height = 32 - graph_y_offset;
    int gap = 1;
    int graph_width = 128 / 2 - gap;
    
    int x_offset = side == 0 ? 0 : graph_width + gap * 2;
    
    int16_t* wavebuffer = side == 0 ? &front_buffer_3[0] : &front_buffer_3[2048];
    int16_t* alternate_wavebuffer = side == 0 ? &front_buffer_2[0] : &front_buffer_2[2048];
    uint32_t* ticker_timer = side == 0 ? &left_ticker_timer_ : &right_ticker_timer_;
    uint8_t* ticker = side == 0 ? &left_ticker_ : &right_ticker_;
    int wavetable = side == 0 ? left_wavetable_ : right_wavetable_;
    int frame = side == 0 ? left_frame_ : right_frame_;
    int16_t wavetable_offset = side == 0 ? left_wavetable_offset_ : right_wavetable_offset_;
    int16_t frame_offset = side == 0 ? left_frame_offset_ : right_frame_offset_;
    bool is_editing = side == 0 ? abEngine.IsEditingLeft() : abEngine.IsEditingRight();
    ABMenuState state = side == 0 ? left_state_ : right_state_;
    int abEngineWavetable = side == 0 ? abEngine.GetLeftWavetable() : abEngine.GetRightWavetable();
    int abEngineFrame = side == 0 ? abEngine.GetLeftFrame() : abEngine.GetRightFrame();
    int wavetable_gui = side == 0 ? left_wavetable_gui_ : right_wavetable_gui_;
    int frame_gui = side == 0 ? left_frame_gui_ : right_frame_gui_;

    Display::outline_rectangle(x_offset, graph_y_offset, graph_width, graph_height);
    
    if(state == AB_SELECT_FRAME || state == AB_SELECT_WAVETABLE) {
        // 
        if(wavetable_gui != wavetable || frame_gui != frame) {
            // load into wavebuffer the current frame
            // flash.StopDMA(true);
            if(side == 0) {
                flash.StartFrameDMARead((uint32_t*)wavebuffer, 4096, wavetable * 65536 + frame * 4096, ABModeMenu::on_load_left_gui_finished);
            } else {
                flash.StartFrameDMARead((uint32_t*)wavebuffer, 4096, wavetable * 65536 + frame * 4096, ABModeMenu::on_load_right_gui_finished);
            }
        }
        // abEngine.FillWaveform(wavebuffer, wavetable, frame);
    }
    else {
        if(wavetable_gui != abEngineWavetable || frame_gui != abEngineFrame) {
            // load into wavebuffer the current frame
            // flash.StopDMA(true);
            if(side == 0) {
                flash.StartFrameDMARead((uint32_t*)wavebuffer, 4096, abEngineWavetable * 65536 + abEngineFrame * 4096, ABModeMenu::on_load_left_gui_finished);
            } else {
                flash.StartFrameDMARead((uint32_t*)wavebuffer, 4096, abEngineWavetable * 65536 + abEngineFrame * 4096, ABModeMenu::on_load_right_gui_finished);
            }
        }
        // abEngine.FillWaveform(wavebuffer, abEngineWavetable, abEngineFrame);
    }

    if(state == AB_SELECT_FRAME || state == AB_SELECT_WAVETABLE)
        Display::Draw_Wave(x_offset + 1, graph_y_offset + 1, graph_width-2, graph_height-2, wavebuffer);
    else
        Display::Draw_Wave(x_offset + 1, graph_y_offset + 1, graph_width-2, graph_height-2, is_editing ? alternate_wavebuffer : wavebuffer);

//    abEngine.FillWaveform(BUF2, right_wavetable_, right_frame_);
//
//    Display::Draw_Wave(graph_width + gap * 2 + 1, graph_y_offset + 1, graph_width-2, graph_height-2, BUF2);

    int y_offset = graph_y_offset + graph_height + 5;
    x_offset -= 1;
    if(state == AB_SELECT_WAVETABLE) {
        for(int i = 0; i < 3; i++)
        {
            char line[20];
            snprintf(line, 20, "%*d", 2, i + wavetable_offset + 1);
            Display::put_string_3x5(x_offset + 2, y_offset + i * 8, strlen(line), line);
            
            // char * name = storage.GetWavetable(i + wavetable_offset)->name;
            char * name;
            if(side == 0) {
                name = left_wavetable_names_[i + frame_offset];
            } else {
                name = right_wavetable_names_[i + frame_offset];
            }

            char * line2 = name;

            int32_t elapsed_time = system_clock.milliseconds() - *ticker_timer;

            uint8_t num_chars = 7;

            if(i + wavetable_offset == wavetable) {
                if(*ticker == 0) {
                    if(elapsed_time > 1000) {
                        (*ticker)++;
                        *ticker_timer = system_clock.milliseconds();
                    }
                } else if(*ticker == (strlen(line2) - num_chars) * 6) {
                    if(elapsed_time > 2000) {
                        ResetTicker(side);
                    }
                }
                else if (elapsed_time > 20) {
                    (*ticker)++;
                    *ticker_timer = system_clock.milliseconds();
                }
                Display::put_string_5x5_loop(x_offset + 2 + 2 * 3 + 4, y_offset + i * 8, strlen(line2), line2, i + wavetable_offset == wavetable, num_chars, strlen(line2) > num_chars ? *ticker : 0);
            } else {
                Display::put_string_5x5_loop(x_offset + 2 + 2 * 3 + 4, y_offset + i * 8, strlen(line2), line2, i + wavetable_offset == wavetable, num_chars, 0);
            }
        }
        int y_shift = 2;
        int bar_height = 3 * 7 + y_shift * 2;
        int y_cursor_offset = ((bar_height - 5) * wavetable_offset) / (FACTORY_WAVETABLE_COUNT + USER_WAVETABLE_COUNT - 1 - 2);
        
        Display::outline_rectangle(x_offset + 64 - 5 + 1, y_offset + 1 - y_shift + y_cursor_offset, 1, 3);
        Display::invert_rectangle(x_offset + 64 - 5, y_offset - y_shift, 3, bar_height);

    } else if (state == AB_SELECT_FRAME) {
        for(int i = 0; i < 3; i++)
        {
            char line[20];

            snprintf(line, 20, "%*d", 2, i + frame_offset + 1);
            Display::put_string_3x5(x_offset + 2, y_offset + i * 8, strlen(line), line);

            // char * name = storage.GetWavetable(wavetable)->waves[i + frame_offset].name;
            char * name;
            if(side == 0) {
                name = left_frame_names_[i + frame_offset];
            } else {
                name = right_frame_names_[i + frame_offset];
            }

            char * line2 = name;

            int32_t elapsed_time = system_clock.milliseconds() - *ticker_timer;

            uint8_t num_chars = 7;

            if(i + frame_offset == frame) {
                if(*ticker == 0) {
                    if(elapsed_time > 1000) {
                        (*ticker)++;
                        *ticker_timer = system_clock.milliseconds();
                    }
                } else if(*ticker == (strlen(line2) - num_chars) * 6) {
                    if(elapsed_time > 2000) {
                        ResetTicker(side);
                    }
                }
                else if (elapsed_time > 20) {
                    (*ticker)++;
                    *ticker_timer = system_clock.milliseconds();
                }
                Display::put_string_5x5_loop(x_offset + 2 + 2 * 3 + 4, y_offset + i * 8, strlen(line2), line2, i + frame_offset == frame, num_chars, strlen(line2) > num_chars ? *ticker : 0);
            } else {
                Display::put_string_5x5_loop(x_offset + 2 + 2 * 3 + 4, y_offset + i * 8, strlen(line2), line2, i + frame_offset == frame, num_chars, 0);

            }

//            Display::put_string_5x5(x_offset + 2 + 2 * 3 + 4, y_offset + i * 7, strlen(line2), line2, i + frame_offset == frame);

        }
        int y_shift = 2;
        int bar_height = 3 * 7 + y_shift * 2;
        int y_cursor_offset = ((bar_height - 5) * frame_offset) / (13);
        
        Display::outline_rectangle(x_offset + 64 - 5 + 1, y_offset + 1 - y_shift + y_cursor_offset, 1, 3);
        Display::invert_rectangle(x_offset + 64 - 5, y_offset - y_shift, 3, bar_height);
    }
    else if(state == AB_LOAD_HOVER || state == AB_EDIT_HOVER) {
        int y_offset = graph_y_offset + graph_height + 5;
        int x_offset = 64 / 2 - (15 + 1);
        x_offset = x_offset + (side == 0 ? 0 : graph_width + gap * 2);
        
        Display::put_image_16bit(x_offset, y_offset, Graphic_icon_load_15x15, 15);
        Display::put_image_16bit(x_offset + 15 + 2, y_offset, Graphic_icon_edit_15x15, 15);
        
        if(state == AB_LOAD_HOVER) {
            Display::invert_rectangle(x_offset + 1, y_offset + 1, 13, 13);
        } else {
            Display::invert_rectangle(x_offset + 1 + 15 + 2, y_offset + 1, 13, 13);
        }
    }
}

void ABModeMenu::paint() {
    Display::clear_screen();

    DrawSide(0);
    DrawSide(1);
}

}
