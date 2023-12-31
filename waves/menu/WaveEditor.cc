/*
  ==============================================================================

    WaveEditor.cpp
    Created: 14 Nov 2023 4:45:53pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"
#include "waves/dsp/fft/fft.h"
#include <cmath>

WaveEditor::WaveEditor() {
//    setLeftState(AB_LOAD_HOVER);
//    setRightState(AB_LOAD_HOVER);
//    left_wavetable_ = 0;
//    left_frame_ = 0;
//    right_wavetable_ = 0;
//    right_frame_ = 0;
//    left_wavetable_offset_ = 0;
//    left_frame_offset_ = 0;
//    right_wavetable_offset_ = 0;
//    right_frame_offset_ = 0;
//    active_menu_ = NONE;
    selection_x1_ = 10;
    selection_x2_ = 30;
    right_state_ = WAVE_EDITOR_RIGHT_ENCODER_DRAW;
    selection_ = WAVE_EDITOR_SELECTION_SINE;
    mode_ = WAVE_EDITOR_SELECTION_SINE;
    state_ = WAVE_EDITOR_STATE_EDITOR;
    menu_selection_offset_ = 0;
    menu_offset_y_ = -30;
    menu_target_offset_y_ = -30;
    spectral_cursor_ = 0;
    pen_drawing_ = false;
    pen_x_ = 32;
    pen_y_ = 48;
    line_x_ = 32;
    line_y_ = 48;
}

WaveEditor::~WaveEditor() {
    
}
//AB_NONE,
//AB_LOAD_HOVER,
//AB_EDIT_HOVER,
//AB_SELECT_WAVETABLE,
//AB_SELECT_FRAME,

bool WaveEditor::handleKeyRelease(int key) {
    if(state_ == WAVE_EDITOR_STATE_EDITOR) {
        if(key == LEFT_ENCODER_CCW) {
            if(selection_x1_ > 0) {
                selection_x1_ = CLAMP(--selection_x1_, 0, 127);
                selection_x2_ = CLAMP(--selection_x2_, 0, 127);
            }
            
        }
        if(key == LEFT_ENCODER_CW) {
            if(selection_x2_ < 127) {
                selection_x1_ = CLAMP(++selection_x1_, 0, 127);
                selection_x2_ = CLAMP(++selection_x2_, 0, 127);
            }
        }
        if(key == RIGHT_ENCODER_CCW) {
            if(right_state_ == WAVE_EDITOR_RIGHT_ENCODER_EXPAND) {
                if(selection_x2_ - selection_x1_ > 2) {
                    selection_x1_++;
                    selection_x2_--;
                    selection_x1_ = CLAMP(selection_x1_, 0, selection_x2_ - 2);
                    selection_x2_ = CLAMP(selection_x2_, selection_x1_ + 2, 127);
                }
            } else {
                float c = 0.0f;
                float width = (selection_x2_ - selection_x1_) * 16.0f;
                // draw wave
                for (int i = selection_x1_; i < selection_x2_; i++) {
                    int integral = i * 16; // 16 = 2048 / 128
                    for(int j = integral; j < integral + 16; j++) {
                        switch(mode_) {
                            case WAVE_EDITOR_SELECTION_SQUARE:
                            {
                                wavedata_[j] = CLAMP<int32_t>(wavedata_[j]-512*2, -32768, 32767);
                                break;
                            }
                            case WAVE_EDITOR_SELECTION_TRIANGLE:
                            {
                                float sample = c / width;
                                if(sample >= 0.5) {
                                    sample = 1.0f - sample;
                                }
                                c += 1.0f;
                                wavedata_[j] = CLAMP<int32_t>(wavedata_[j]-512*4*sample, -32768, 32767);
                                break;
                            }
                            case WAVE_EDITOR_SELECTION_SINE:
                            {
                                float sample = sin(M_PI * c / width);
                                c += 1.0f;
                                wavedata_[j] = CLAMP<int32_t>(wavedata_[j]-512*4*sample, -32768, 32767);
                                break;
                            }
                            case WAVE_EDITOR_SELECTION_RAMP:
                            {
                                float sample = c / width;
                                c += 1.0f;
                                wavedata_[j] = CLAMP<int32_t>(wavedata_[j]-512*4*sample, -32768, 32767);
                                break;
                            }
                            case WAVE_EDITOR_SELECTION_SAW:
                            {
                                float sample = c / width;
                                sample = 1.0f - sample;
                                c += 1.0f;
                                wavedata_[j] = CLAMP<int32_t>(wavedata_[j]-512*4*sample, -32768, 32767);
                                break;
                            }
                            case WAVE_EDITOR_SELECTION_PEAK:
                            {
                                float sample;
                                float index = c / width;
                                if(index >= 0.5f)
                                    sample = -log(sin(M_PI * (0.5f + (1.0f - c/width))));
                                else
                                    sample = -log(sin(M_PI * (0.5f + c/width)));
                                c += 1.0f;
                                wavedata_[j] = CLAMP<int32_t>(wavedata_[j]-512*2*sample, -32768, 32767);
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
                
            }
            
        }
        if(key == RIGHT_ENCODER_CW) {
            if(right_state_ == WAVE_EDITOR_RIGHT_ENCODER_EXPAND) {
                selection_x1_--;
                selection_x2_++;
                selection_x1_ = CLAMP(selection_x1_, 0, selection_x2_ - 2);
                selection_x2_ = CLAMP(selection_x2_, selection_x1_ + 2, 127);
            } else {
                // draw wave
                float c = 0.0f;
                float width = (selection_x2_ - selection_x1_) * 16.0f;
                
                for (int i = selection_x1_; i < selection_x2_; i++) {
                    int integral = i * 16; // 16 = 2048 / 128
                    // add sine based on phase = PI / (x2 - x1) width
                    for(int j = integral; j < integral + 16; j++) {
                        switch(mode_) {
                            case WAVE_EDITOR_SELECTION_SQUARE:
                            {
                                wavedata_[j] = CLAMP<int32_t>(wavedata_[j]+512*2, -32768, 32767);
                                break;
                            }
                            case WAVE_EDITOR_SELECTION_TRIANGLE:
                            {
                                float sample = c / width;
                                if(sample >= 0.5) {
                                    sample = 1.0f - sample;
                                }
                                c += 1.0f;
                                wavedata_[j] = CLAMP<int32_t>(wavedata_[j]+512*4*sample, -32768, 32767);
                                break;
                            }
                            case WAVE_EDITOR_SELECTION_SINE:
                            {
                                float sample = sin(M_PI * c / width);
                                c += 1.0f;
                                wavedata_[j] = CLAMP<int32_t>(wavedata_[j]+512*4*sample, -32768, 32767);
                                break;
                            }
                            case WAVE_EDITOR_SELECTION_RAMP:
                            {
                                float sample = c / width;
                                c += 1.0f;
                                wavedata_[j] = CLAMP<int32_t>(wavedata_[j]+512*4*sample, -32768, 32767);
                                break;
                            }
                            case WAVE_EDITOR_SELECTION_SAW:
                            {
                                float sample = c / width;
                                sample = 1.0f - sample;
                                c += 1.0f;
                                wavedata_[j] = CLAMP<int32_t>(wavedata_[j]+512*4*sample, -32768, 32767);
                                break;
                            }
                            case WAVE_EDITOR_SELECTION_PEAK:
                            {
                                float sample;
                                float index = c / width;
                                if(index >= 0.5f)
                                    sample = -log(sin(M_PI * (0.5f + (1.0f - c/width))));
                                else
                                    sample = -log(sin(M_PI * (0.5f + c/width)));
                                c += 1.0f;
                                wavedata_[j] = CLAMP<int32_t>(wavedata_[j]+512*2*sample, -32768, 32767);
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
                
            }
        }
        if(key == RIGHT_ENCODER_CLICK) {
            right_state_ = right_state_ == WAVE_EDITOR_RIGHT_ENCODER_DRAW ? WAVE_EDITOR_RIGHT_ENCODER_EXPAND : WAVE_EDITOR_RIGHT_ENCODER_DRAW;
        }
        if(key == LEFT_ENCODER_CLICK) {
            state_ = WAVE_EDITOR_STATE_MENU;
            selection_ = mode_;

            if(menu_selection_offset_ > selection_)
                menu_selection_offset_ = selection_;
            if(menu_selection_offset_ <= selection_ - 5)
                menu_selection_offset_ = selection_ - 5;

            menu_offset_y_ = -30;
            menu_target_offset_y_ = 0;
            timer_ = system_clock.milliseconds();
        }
        if(key == BACK_BUTTON) {
            if(back_menu_)
                context.setState(back_menu_, true);
            else
                context.setState(&abModeMenu, true);
        }
    }
    else if(state_ == WAVE_EDITOR_STATE_MENU) {
        if(key == LEFT_ENCODER_CCW) {
            selection_ = CLAMP<int8_t>(--selection_, WAVE_EDITOR_SELECTION_SQUARE, WAVE_EDITOR_SELECTION_SAVE);
            if(menu_selection_offset_ > selection_)
                menu_selection_offset_ = selection_;

        }
        if(key == LEFT_ENCODER_CW) {
            selection_ = CLAMP<int8_t>(++selection_, WAVE_EDITOR_SELECTION_SQUARE, WAVE_EDITOR_SELECTION_SAVE);

            if(menu_selection_offset_ <= selection_ - 5)
                menu_selection_offset_ = selection_ - 5;
        }
        if(key == RIGHT_ENCODER_CCW) {
        }
        if(key == RIGHT_ENCODER_CW) {
        }
        if(key == RIGHT_ENCODER_CLICK) {
        }
        if(key == LEFT_ENCODER_CLICK) {
            if(selection_ == WAVE_EDITOR_SELECTION_CLEAR) {
                memset(wavedata_, 0, 2048*2);
                if(mode_ == WAVE_EDITOR_SELECTION_SPECTRAL) {
                    CalculateFFT();
                }
            } else if(selection_ == WAVE_EDITOR_SELECTION_SPECTRAL) {
                mode_ = selection_;
                state_ = WAVE_EDITOR_STATE_SPECTRAL;
                menu_target_offset_y_ = -30;
                timer_ = system_clock.milliseconds();

                CalculateFFT();
            } else if(selection_ == WAVE_EDITOR_SELECTION_PEN) {
                mode_ = selection_;
                state_ = WAVE_EDITOR_STATE_PEN;
                menu_target_offset_y_ = -30;
                timer_ = system_clock.milliseconds();
            } else if(selection_ == WAVE_EDITOR_SELECTION_LINE) {
                mode_ = selection_;
                state_ = WAVE_EDITOR_STATE_LINE;
                menu_target_offset_y_ = -30;
                timer_ = system_clock.milliseconds();
            } else if (selection_ == WAVE_EDITOR_SELECTION_LOAD) {
                context.setState(&loadWaveMenu);
                loadWaveMenu.setBackMenu(&waveEditor);
                if(wavedata_ == BUF3)
                    loadWaveMenu.setTarget(LoadWaveMenu::AB_ENGINE_A);
                else if(wavedata_ == BUF4)
                    loadWaveMenu.setTarget(LoadWaveMenu::AB_ENGINE_B);
                else if (wavedata_ == BUF5)
                    loadWaveMenu.setTarget(LoadWaveMenu::WAVE_MANAGER);
            } else if (selection_ == WAVE_EDITOR_SELECTION_SAVE) {
//                if(wavedata_ == BUF5) {
//                    bool success = storage.SaveWave(storage.GetWavetable(wavetable_)->waves[frame_].name, BUF5, wavetable_, frame_);
//                    if(success) {
//                        popup.show();
//                        popup.SetLine(0, (char*)"SAVE");
//                        popup.SetLine(1, (char*)"SUCCESSFUL!");
//                        popup.SetLine(2, (char*)"\0");
//                    } else {
//                        popup.show();
//                        popup.SetLine(0, (char*)"CANNOT OVERWRITE");
//                        popup.SetLine(1, (char*)"FACTORY PRESETS!");
//                        popup.SetLine(2, (char*)"\0");
//                    }
//                } else {
                context.setState(&saveWaveMenu);
                saveWaveMenu.setWavedata(wavedata_);
                saveWaveMenu.setBackMenu(&waveEditor);
//                }
            }
            else {
                mode_ = selection_;
                state_ = WAVE_EDITOR_STATE_EDITOR;
                menu_target_offset_y_ = -30;
                timer_ = system_clock.milliseconds();
            }
        }
        if(key == BACK_BUTTON) {
            switch(mode_) {
                case WAVE_EDITOR_SELECTION_SPECTRAL:
                    state_ = WAVE_EDITOR_STATE_SPECTRAL;
                    break;
                case WAVE_EDITOR_SELECTION_PEN:
                    state_ = WAVE_EDITOR_STATE_PEN;
                    break;
                case WAVE_EDITOR_SELECTION_LINE:
                    state_ = WAVE_EDITOR_STATE_LINE;
                    break;
                default:
                    state_ = WAVE_EDITOR_STATE_EDITOR;
                    break;
            }
            menu_target_offset_y_ = -30;
            timer_ = system_clock.milliseconds();
        }
    }
    else if(state_ == WAVE_EDITOR_STATE_SPECTRAL) {
        if(key == LEFT_ENCODER_CCW) {
            spectral_cursor_ = CLAMP(--spectral_cursor_, 0, 31);
        }
        if(key == LEFT_ENCODER_CW) {
            spectral_cursor_ = CLAMP(++spectral_cursor_, 0, 31);
        }
        if(key == RIGHT_ENCODER_CCW) {
            spectral_gain_[spectral_cursor_] = CLAMP(spectral_gain_[spectral_cursor_] - 0.1f, 0.0f, 1.0f);
            CalculateIFFT();
        }
        if(key == RIGHT_ENCODER_CW) {
            spectral_gain_[spectral_cursor_] = CLAMP(spectral_gain_[spectral_cursor_] + 0.1f, 0.0f, 1.0f);
            CalculateIFFT();
        }
        if(key == RIGHT_ENCODER_CLICK) {
        }
        if(key == LEFT_ENCODER_CLICK) {
            state_ = WAVE_EDITOR_STATE_MENU;
            selection_ = mode_;
            menu_offset_y_ = -30;
            menu_target_offset_y_ = 0;
            timer_ = system_clock.milliseconds();
        }
        if(key == BACK_BUTTON) {
            if(back_menu_)
                context.setState(back_menu_, true);
            else
                context.setState(&abModeMenu, true);
        }
    }
    else if(state_ == WAVE_EDITOR_STATE_PEN) {
        if(key == LEFT_ENCODER_CCW) {
            pen_y_ = CLAMP<int16_t>(--pen_y_, 0, 63);
            if(pen_drawing_)
                for (int i = 0; i < 16; i++)
                    wavedata_[pen_x_*16 + i] = (-pen_y_ + 32) * 1024 + (pen_y_ < 32 ? -1 : 0);
        }
        if(key == LEFT_ENCODER_CW) {
            pen_y_ = CLAMP<int16_t>(++pen_y_, 0, 63);
            if(pen_drawing_)
                for (int i = 0; i < 16; i++)
                    wavedata_[pen_x_*16 + i] = (-pen_y_ + 32) * 1024 + (pen_y_ < 32 ? -1 : 0);
        }
        if(key == RIGHT_ENCODER_CCW) {
            if(!pen_drawing_)
                pen_x_ = CLAMP<int16_t>(--pen_x_, 0, 127);
        }
        if(key == RIGHT_ENCODER_CW) {
            pen_x_ = CLAMP<int16_t>(++pen_x_, 0, 127);
            if(pen_drawing_)
                for (int i = 0; i < 16; i++)
                    wavedata_[pen_x_*16 + i] = (-pen_y_ + 32) * 1024 + (pen_y_ < 32 ? -1 : 0);
        }
        if(key == RIGHT_ENCODER_CLICK) {
            pen_drawing_ = !pen_drawing_;
            if(pen_drawing_)
                for (int i = 0; i < 16; i++)
                    wavedata_[pen_x_*16 + i] = (-pen_y_ + 32) * 1024 + (pen_y_ < 32 ? -1 : 0);
        }
        if(key == LEFT_ENCODER_CLICK) {
            state_ = WAVE_EDITOR_STATE_MENU;
            selection_ = mode_;
            menu_offset_y_ = -30;
            menu_target_offset_y_ = 0;
            pen_drawing_ = false;
            timer_ = system_clock.milliseconds();
        }
        if(key == BACK_BUTTON) {
            pen_drawing_ = false;
            if(back_menu_)
                context.setState(back_menu_, true);
            else
                context.setState(&abModeMenu, true);
        }
    }
    else if(state_ == WAVE_EDITOR_STATE_LINE) {
        int pen_increment = 5;
        if(key == LEFT_ENCODER_CCW) {
            pen_y_ = CLAMP<int16_t>(pen_y_ - pen_increment, 0, 63);
            if(pen_drawing_)
                LineToWavedata(line_x_, line_y_, pen_x_, pen_y_);
        }
        if(key == LEFT_ENCODER_CW) {
            pen_y_ = CLAMP<int16_t>(pen_y_ + pen_increment, 0, 63);
            if(pen_drawing_)
                LineToWavedata(line_x_, line_y_, pen_x_, pen_y_);
        }
        if(key == RIGHT_ENCODER_CCW) {
            pen_x_ = CLAMP<int16_t>(pen_x_ - pen_increment, 0, 127);
            if(pen_drawing_)
                LineToWavedata(line_x_, line_y_, pen_x_, pen_y_);
        }
        if(key == RIGHT_ENCODER_CW) {
            pen_x_ = CLAMP<int16_t>(pen_x_ + pen_increment, 0, 127);
            if(pen_drawing_)
                LineToWavedata(line_x_, line_y_, pen_x_, pen_y_);
        }
        if(key == RIGHT_ENCODER_CLICK) {
            if(pen_drawing_) {
                // draw line onto wavedata buffer
            } else {
            }
            line_x_ = pen_x_;
            line_y_ = pen_y_;
            pen_drawing_ = !pen_drawing_;
        }
        if(key == LEFT_ENCODER_CLICK) {
            state_ = WAVE_EDITOR_STATE_MENU;
            selection_ = mode_;
            menu_offset_y_ = -30;
            menu_target_offset_y_ = 0;
            pen_drawing_ = false;
            timer_ = system_clock.milliseconds();
        }
        if(key == BACK_BUTTON) {
            pen_drawing_ = false;
            if(back_menu_)
                context.setState(back_menu_, true);
            else
                context.setState(&abModeMenu, true);
        }
    }
    return true;
}

void WaveEditor::LineToWavedata(int x0, int y0, int x1, int y1) {
    int8_t dx;
    int8_t sx;
    int8_t dy;
    int8_t sy;
    int8_t err;
    int8_t e2;
    
    dx = abs((int16_t )x1 - (int16_t )x0);
    sx = x0 < x1 ? 1 : -1;
    dy = abs((int16_t )y1 - (int16_t )y0);
    sy = y0 < y1 ? 1 : -1;
    err = (dx > dy ? dx : -dy) / 2;
    
    for (;;)
    {
//        Display::Put_Pixel(x0, y0, set);
        for(int i = 0; i < 16; i++)
            wavedata_[x0 * 16 + i] = (-y0 + 32) * 1024 + (y0 < 32 ? -1 : 0);
        if ((x0 == x1) && (y0 == y1))
            break;
        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x0 = (int16_t)((int16_t) x0 + sx);
        }
        if (e2 < dy)
        {
            err += dx;
            y0 = (int16_t)((int16_t) y0 + sy);
        }
    }
}

void WaveEditor::triggerUpdate(bool back_pressed) {
    if(!back_pressed) {
        if(state_ == WAVE_EDITOR_STATE_MENU) {
            menu_target_offset_y_ = -30;
            switch(mode_) {
                case WAVE_EDITOR_SELECTION_SPECTRAL:
                    state_ = WAVE_EDITOR_STATE_SPECTRAL;
                    break;
                case WAVE_EDITOR_SELECTION_PEN:
                    state_ = WAVE_EDITOR_STATE_PEN;
                    break;
                case WAVE_EDITOR_SELECTION_LINE:
                    state_ = WAVE_EDITOR_STATE_LINE;
                    break;
                default:
                    state_ = WAVE_EDITOR_STATE_EDITOR;
                    break;
            }
        }
    }
    if(mode_ == WAVE_EDITOR_SELECTION_SPECTRAL) {
//        state_ = WAVE_EDITOR_STATE_SPECTRAL;
//        menu_target_offset_y_ = -30;
//        timer_ = juce::Time::currentTimeMillis();

        CalculateFFT();
    }

}

void WaveEditor::DrawTriangle(int x, int y, bool reversed) {
    if(reversed) {
        Display::Put_Pixel(x, y + 2, true);
        Display::Put_Pixel(x + 1, y+1, true);
        Display::Put_Pixel(x + 1, y+2, true);
        Display::Put_Pixel(x + 1, y+3, true);
        Display::Put_Pixel(x + 2, y, true);
        Display::Put_Pixel(x + 2, y+1, true);
        Display::Put_Pixel(x + 2, y+2, true);
        Display::Put_Pixel(x + 2, y+3, true);
        Display::Put_Pixel(x + 2, y+4, true);
    } else {
        Display::Put_Pixel(x + 2, y + 2, true);
        Display::Put_Pixel(x + 1, y+1, true);
        Display::Put_Pixel(x + 1, y+2, true);
        Display::Put_Pixel(x + 1, y+3, true);
        Display::Put_Pixel(x, y, true);
        Display::Put_Pixel(x, y+1, true);
        Display::Put_Pixel(x, y+2, true);
        Display::Put_Pixel(x, y+3, true);
        Display::Put_Pixel(x, y+4, true);
    }
}

void WaveEditor::CalculateFFT() {
    float wavedata[2048];
    for (int i = 0; i < 2048; i++) {
        wavedata[i] = wavedata_[i] / 32768.0f;
    }
    FFT::fft(wavedata, 2048, spectral_phasors_);
    for(int i = 0; i < 32; i++) {
        spectral_gain_[i] = std::min(FFT::complexMagnitude(spectral_phasors_[i+1]), 2048.0f) / 2048.0f;
        if(spectral_phasors_[i + 1].imag == 0.0f)
            spectral_angles_[i] = M_PI;
        else
            spectral_angles_[i] = atan2(spectral_phasors_[i + 1].real, spectral_phasors_[i + 1].imag);
        spectral_angles_[64 - (i+1)] = atan2(spectral_phasors_[2048 - (i + 1)].real, spectral_phasors_[2048 - (i + 1)].imag);
    }

    CalculateIFFT();
}

void WaveEditor::CalculateIFFT() {
    FFT::COMPLEX_NUMBER waveoutput[2048];

    float angle;
    for(int i = 0; i < 32; i++) {
        angle = spectral_angles_[i];
        spectral_phasors_[i + 1].real = sin(angle) * 2048.0f * spectral_gain_[i];
        spectral_phasors_[i + 1].imag = cos(angle) * 2048.0f * spectral_gain_[i];
        
        angle = spectral_angles_[64 - (i + 1)];
        spectral_phasors_[2048 - (i + 1)].real = sin(angle) * 2048.0f * spectral_gain_[i];
        spectral_phasors_[2048 - (i + 1)].imag = cos(angle) * 2048.0f * spectral_gain_[i];
    }

    FFT::ifft(spectral_phasors_, 2048, waveoutput);
    
    for(int i = 0; i < 2048; i++) {
        wavedata_[i] = CLAMP(waveoutput[i].real * 32767.0f, -32768.0f, 32767.0f);
    }
}

void WaveEditor::DrawMenu() {
    uint32_t elapsed_time = system_clock.milliseconds() - timer_;
    if(menu_target_offset_y_ != menu_offset_y_ && elapsed_time > 10) {
        timer_ = system_clock.milliseconds();
        if(menu_target_offset_y_ > menu_offset_y_)
            menu_offset_y_++;
        else if(menu_target_offset_y_ < menu_offset_y_)
            menu_offset_y_--;
    }
    Display::clear_rectangle_simple(0, menu_offset_y_, 128, 26);
    Display::LCD_Line(0, menu_offset_y_+26, 127, menu_offset_y_+26, true);

    int x_offset = 64 - (15 * 6 + 2 * 5) / 2;
    int gap = 2;
    
    const uint8_t * list[] = {
        &Graphic_icon_wave_square[0][0],
        &Graphic_icon_wave_triangle[0][0],
        &Graphic_icon_wave_hump[0][0],
        &Graphic_icon_wave_ramp[0][0],
        &Graphic_icon_wave_reverse_ramp[0][0],
        &Graphic_icon_wave_spike[0][0],
        &Graphic_icon_edit_15x15[0][0],
        &Graphic_icon_line_15x15[0][0],
        &Graphic_icon_spectral_15x15[0][0],
        &Graphic_icon_delete_15x15[0][0],
        &Graphic_icon_load_15x15[0][0],
        &Graphic_icon_save_15x15[0][0],
    };
//    list[0]
    for(int i = menu_selection_offset_; i < menu_selection_offset_ + 6; i++) {
        Display::put_image_16bit(x_offset, menu_offset_y_ + 3, (const unsigned char (*)[2])list[i], 15);

        if(mode_ == i) {
            Display::invert_rectangle(x_offset+1, menu_offset_y_+4, 13, 13);
        }
        if(selection_ == i) {
//            Display::invert_rectangle(x_offset+1, menu_offset_y_+4, 13, 13);
            Display::LCD_Line(x_offset-1, menu_offset_y_+4, x_offset-1, menu_offset_y_+4 + 14, true);
            Display::LCD_Line(x_offset-1, menu_offset_y_+4 + 14, x_offset-1 + 15, menu_offset_y_+4 + 14, true);
        }
        x_offset += 15 + gap;
    }
    if(menu_selection_offset_ > 0) {
        Display::put_image_16bit(0+1, menu_offset_y_ + 6, Graphic_icon_arrow_left_9x9, 9);
    }
    if(menu_selection_offset_ < 6) {
        Display::put_image_16bit(128-9-1, menu_offset_y_ + 6, Graphic_icon_arrow_right_9x9, 9);
    }
    
    char *line;
    switch(selection_) {
        case WAVE_EDITOR_SELECTION_SQUARE:
            line = (char*)"SQUARE";
            break;
        case WAVE_EDITOR_SELECTION_TRIANGLE:
            line = (char*)"TRIANGLE";
            break;
        case WAVE_EDITOR_SELECTION_SINE:
            line = (char*)"SINE";
            break;
        case WAVE_EDITOR_SELECTION_RAMP:
            line = (char*)"RAMP";
            break;
        case WAVE_EDITOR_SELECTION_SAW:
            line = (char*)"SAW";
            break;
        case WAVE_EDITOR_SELECTION_PEAK:
            line = (char*)"LOG";
            break;
        case WAVE_EDITOR_SELECTION_PEN:
            line = (char*)"PEN";
            break;
        case WAVE_EDITOR_SELECTION_LINE:
            line = (char*)"LINE";
            break;
        case WAVE_EDITOR_SELECTION_SPECTRAL:
            line = (char*)"SPECTRAL";
            break;
        case WAVE_EDITOR_SELECTION_CLEAR:
            line = (char*)"CLEAR";
            break;
        case WAVE_EDITOR_SELECTION_LOAD:
            line = (char*)"LOAD";
            break;
        case WAVE_EDITOR_SELECTION_SAVE:
            line = (char*)"SAVE";
            break;
        default:
            break;
    }
    Display::put_string_5x5(64 - strlen(line)*6 / 2, menu_offset_y_ + 20, strlen(line), line);
}

void WaveEditor::paint(juce::Graphics& g) {
    Display::clear_screen();

    if(mode_ == WAVE_EDITOR_SELECTION_SPECTRAL) {
        Display::LCD_DottedLine(0, 16, 127, 16, 4, 2, true);
//        Display::LCD_DottedLine(0, 32, 127, 32, 4, 2, true);
        Display::LCD_Line(0, 32, 127, 32, true);
        if(wavedata_)
            Display::Draw_Wave(0, 0, 128, 32, wavedata_);
        for(int i = 0; i < 32; i++) {
            if(i == spectral_cursor_) {
                Display::LCD_Line(i*4 + 2, 32, i*4 + 2, 62, true);
            }
            
            Display::LCD_Line(i*4 + 1, 62 - spectral_gain_[i] * 30.0f, i*4 + 1, 62, true);
            Display::LCD_Line(i*4 + 2, 62 - spectral_gain_[i] * 30.0f, i*4 + 2, 62, true);
            Display::LCD_Line(i*4 + 3, 62 - spectral_gain_[i] * 30.0f, i*4 + 3, 62, true);

            Display::LCD_Line(i*4 + 1, 62, i*4 + 3, 62, true);
        }
    } else if (mode_ == WAVE_EDITOR_SELECTION_PEN){
        Display::LCD_DottedLine(0, 32, 127, 32, 4, 2, true);

        // draw plus sign
        Display::LCD_Line(pen_x_-2, pen_y_, pen_x_+2, pen_y_, true);
        Display::LCD_Line(pen_x_, pen_y_-2, pen_x_, pen_y_+2, true);

        if(wavedata_)
            Display::Draw_Wave(0, 0, 128, 64, wavedata_);
    } else if (mode_ == WAVE_EDITOR_SELECTION_LINE){
        Display::LCD_DottedLine(0, 32, 127, 32, 4, 2, true);

        // draw circle
        
        Display::LCD_Circle(pen_x_, pen_y_, 2, true);
        if(pen_drawing_)
            Display::LCD_Line(line_x_, line_y_, pen_x_, pen_y_, true);
//        Display::LCD_Line(line_x_, line_y_-2, pen_x_, line_y_+2, true);

        if(wavedata_)
            Display::Draw_Wave(0, 0, 128, 64, wavedata_);
    } else {
        Display::LCD_DottedLine(0, 32, 127, 32, 4, 2, true);
        Display::LCD_DottedLine(selection_x1_, 0, selection_x1_, 63, 1, 1, true);
        Display::LCD_DottedLine(selection_x2_, 0, selection_x2_, 63, 1, 1, true);
        if(right_state_ == WAVE_EDITOR_RIGHT_ENCODER_EXPAND) {
            // draw triangles
            DrawTriangle(selection_x1_ - 3, 64 - 5, false);
            DrawTriangle(selection_x2_+1, 64 - 5, true);
            DrawTriangle(selection_x1_ - 3, 0, false);
            DrawTriangle(selection_x2_+1, 0, true);
        }
        if(wavedata_)
            Display::Draw_Wave(0, 0, 128, 64, wavedata_);
    }

    DrawMenu();
    //
//    int graph_y_offset = 3;
//    int graph_height = 32 - graph_y_offset;
//    int gap = 1;
//    int graph_width = 128 / 2 - gap;
//    
//    Display::outline_rectangle(0, graph_y_offset, graph_width, graph_height);
//    Display::outline_rectangle(graph_width + gap * 2, graph_y_offset, graph_width, graph_height);
//    
//    abEngine.FillWaveform(BUF1, left_wavetable_, left_frame_);
//
//    Display::Draw_Wave(1, graph_y_offset + 1, graph_width-2, graph_height-2, BUF1);
//
//    abEngine.FillWaveform(BUF2, right_wavetable_, right_frame_);
//
//    Display::Draw_Wave(graph_width + gap * 2 + 1, graph_y_offset + 1, graph_width-2, graph_height-2, BUF2);
//
//    int y_offset = graph_y_offset + graph_height + 5;
//    int x_offset = 64 - 5;
//    if(left_state_ == AB_SELECT_WAVETABLE) {
//        for(int i = 0; i < 3; i++)
//        {
//            char line[20];
//            snprintf(line, 20, "TABLE %d", i + left_wavetable_offset_);
//            Display::put_string_5x5(5, y_offset + i * 7, strlen(line), line, i+left_wavetable_offset_ == left_wavetable_);
//
//            int y_shift = 2;
//            int bar_height = 3 * 7 + y_shift * 2;
//            int y_cursor_offset = ((bar_height-3/2) * left_wavetable_offset_) / 15;
//            Display::outline_rectangle(x_offset+1, y_offset + 1 - y_shift + y_cursor_offset, 1, 3);
//            Display::invert_rectangle(x_offset, y_offset - y_shift, 3, bar_height);
//        }
//    } else if (left_state_ == AB_SELECT_FRAME) {
//        for(int i = 0; i < 3; i++)
//        {
//            char line[20];
//            snprintf(line, 20, "FRAME %d", i + left_frame_offset_);
//            Display::put_string_5x5(5, y_offset + i * 7, strlen(line), line, i+left_frame_offset_ == left_frame_);
//
//            int y_shift = 2;
//            int bar_height = 3 * 7 + y_shift * 2;
//            int y_cursor_offset = ((bar_height-3/2) * left_frame_offset_) / 15;
//            Display::outline_rectangle(x_offset+1, y_offset + 1 - y_shift + y_cursor_offset, 1, 3);
//            Display::invert_rectangle(x_offset, y_offset - y_shift, 3, bar_height);
//        }
//    }
//    else if(left_state_ == AB_LOAD_HOVER || left_state_ == AB_EDIT_HOVER) {
//        int y_offset = graph_y_offset + graph_height + 5;
//        int x_offset = 64 / 2 - (15 + 1);
//        Display::put_image_16bit(x_offset, y_offset, Graphic_icon_load_15x15, 15);
//        Display::put_image_16bit(x_offset + 15 + 2, y_offset, Graphic_icon_edit_15x15, 15);
//        
//        if(left_state_ == AB_LOAD_HOVER) {
//            Display::invert_rectangle(x_offset+1, y_offset+1, 13, 13);
//        } else {
//            Display::invert_rectangle(x_offset+1 + 15 + 2, y_offset+1, 13, 13);
//        }
//    }
    
}
