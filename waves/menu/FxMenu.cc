/*
 ==============================================================================
 
 FxMenu.cpp
 Created: 12 Nov 2023 9:46:54pm
 Author:  Eugene Clewlow
 
 ==============================================================================
 */

#include "waves/Display.h"
#include "waves/graphics.h"
#include "waves/Globals.h"

namespace waves {

FxMenu::FxMenu() {
    setLeftState(FX_MENU_LEFT_FX);
    setRightState(FX_MENU_RIGHT_MOD);
}

FxMenu::~FxMenu() {
    
}

bool FxMenu::handleKeyRelease(int key) {
    
    if(key == LEFT_ENCODER_CCW) {
        switch(left_state_) {
            case FX_MENU_LEFT_FX:
                // if(abEngine.get_fx_effect() == EFFECT_TYPE_FM) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_BYPASS);
                // }
                // else if(abEngine.get_fx_effect() == EFFECT_TYPE_RING_MODULATOR) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_FM);
                // }
                // else if(abEngine.get_fx_effect() == EFFECT_TYPE_PHASE_DISTORTION) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_RING_MODULATOR);
                // }
                // else if(abEngine.get_fx_effect() == EFFECT_TYPE_WAVEFOLDER) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_PHASE_DISTORTION);
                // }
                // else if(abEngine.get_fx_effect() == EFFECT_TYPE_WAVEWRAPPER) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_WAVEFOLDER);
                // }
                // else if(abEngine.get_fx_effect() == EFFECT_TYPE_BITCRUSH) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_WAVEWRAPPER);
                // }
                // else if(abEngine.get_fx_effect() == EFFECT_TYPE_DRIVE) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_BITCRUSH);
                // }
                // else if(abEngine.get_fx_effect() == EFFECT_TYPE_BYPASS) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_DRIVE);
                // }
                // break;                
                if(effect_manager.getEffect() == &fm) {
                    effect_manager.setEffect(EFFECT_TYPE_BYPASS);
                }
                else if(effect_manager.getEffect() == &ring_modulator) {
                    effect_manager.setEffect(EFFECT_TYPE_FM);
                }
                else if(effect_manager.getEffect() == &phase_distortion) {
                    effect_manager.setEffect(EFFECT_TYPE_RING_MODULATOR);
                }
                else if(effect_manager.getEffect() == &wavefolder) {
                    effect_manager.setEffect(EFFECT_TYPE_PHASE_DISTORTION);
                }
                else if(effect_manager.getEffect() == &wavewrapper) {
                    effect_manager.setEffect(EFFECT_TYPE_WAVEFOLDER);
                }
                else if(effect_manager.getEffect() == &bitcrush) {
                    effect_manager.setEffect(EFFECT_TYPE_WAVEWRAPPER);
                }
                else if(effect_manager.getEffect() == &drive) {
                    effect_manager.setEffect(EFFECT_TYPE_BITCRUSH);
                }
                else if(effect_manager.getEffect() == &bypass) {
                    effect_manager.setEffect(EFFECT_TYPE_DRIVE);
                }
                break;
            case FX_MENU_LEFT_DEPTH:
                effect_manager.setDepth(effect_manager.getDepth()-0.1f);
                break;
            default:
                break;
        }
    }
    if(key == LEFT_ENCODER_CW) {
        switch(left_state_) {
            case FX_MENU_LEFT_FX:
                // if(abEngine.get_fx_effect() == EFFECT_TYPE_FM) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_RING_MODULATOR);
                // }
                // else if(abEngine.get_fx_effect() == EFFECT_TYPE_RING_MODULATOR) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_PHASE_DISTORTION);
                // }
                // else if(abEngine.get_fx_effect() == EFFECT_TYPE_PHASE_DISTORTION) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_WAVEFOLDER);
                // }
                // else if(abEngine.get_fx_effect() == EFFECT_TYPE_WAVEFOLDER) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_WAVEWRAPPER);
                // }
                // else if(abEngine.get_fx_effect() == EFFECT_TYPE_WAVEWRAPPER) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_BITCRUSH);
                // }
                // else if(abEngine.get_fx_effect() == EFFECT_TYPE_BITCRUSH) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_DRIVE);
                // }
                // else if(abEngine.get_fx_effect() == EFFECT_TYPE_DRIVE) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_BYPASS);
                // }
                // else if(abEngine.get_fx_effect() == EFFECT_TYPE_BYPASS) {
                //     abEngine.set_fx_effect(EFFECT_TYPE_FM);
                // }
                // break;                

                if(effect_manager.getEffect() == &fm) {
                    effect_manager.setEffect(EFFECT_TYPE_RING_MODULATOR);
                }
                else if(effect_manager.getEffect() == &ring_modulator) {
                    effect_manager.setEffect(EFFECT_TYPE_PHASE_DISTORTION);
                }
                else if(effect_manager.getEffect() == &phase_distortion) {
                    effect_manager.setEffect(EFFECT_TYPE_WAVEFOLDER);
                }
                else if(effect_manager.getEffect() == &wavefolder) {
                    effect_manager.setEffect(EFFECT_TYPE_WAVEWRAPPER);
                }
                else if(effect_manager.getEffect() == &wavewrapper) {
                    effect_manager.setEffect(EFFECT_TYPE_BITCRUSH);
                }
                else if(effect_manager.getEffect() == &bitcrush) {
                    effect_manager.setEffect(EFFECT_TYPE_DRIVE);
                }
                else if(effect_manager.getEffect() == &drive) {
                    effect_manager.setEffect(EFFECT_TYPE_BYPASS);
                }
                else if(effect_manager.getEffect() == &bypass) {
                    effect_manager.setEffect(EFFECT_TYPE_FM);
                }
                break;
            case FX_MENU_LEFT_DEPTH:
                effect_manager.setDepth(effect_manager.getDepth()+0.1f);
                break;
            default:
                break;
        }
    }
    
    
    
    if(key == RIGHT_ENCODER_CCW) {
        switch(right_state_) {
            case FX_MENU_RIGHT_MOD:
                if(effect_manager.getControlType() == MANUAL_CONTROL) {
                    ClearWaveform();
                    effect_manager.setControlType(EXTERNAL_MODULATOR);
                }
                else if(effect_manager.getControlType() == INTERNAL_MODULATOR)
                    effect_manager.setControlType(MANUAL_CONTROL);
                else if(effect_manager.getControlType() == EXTERNAL_MODULATOR)
                    effect_manager.setControlType(INTERNAL_MODULATOR);
                break;
            case FX_MENU_RIGHT_SYNC:
                effect_manager.setSync(!effect_manager.getSync());
                break;
            case FX_MENU_RIGHT_RANGE:
                effect_manager.setRange(effect_manager.getRange() - 1);
                break;
            case FX_MENU_RIGHT_SCALE:
                effect_manager.setScale(effect_manager.getScale() - 1);
                break;
            case FX_MENU_RIGHT_WAVE:
                effect_manager.setOscillatorShape((effect_manager.getOscillatorShape() + OSCILLATOR_SHAPE_LAST - 1) % OSCILLATOR_SHAPE_LAST);
                break;
            case FX_MENU_RIGHT_RATIO:
//                effect_manager.setRatio(effect_manager.getRatio()-1);
                break;
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CW) {
        switch(right_state_) {
            case FX_MENU_RIGHT_MOD:
                if(effect_manager.getControlType() == MANUAL_CONTROL)
                    effect_manager.setControlType(INTERNAL_MODULATOR);
                else if(effect_manager.getControlType() == INTERNAL_MODULATOR) {
                    ClearWaveform();
                    effect_manager.setControlType(EXTERNAL_MODULATOR);
                }
                else if(effect_manager.getControlType() == EXTERNAL_MODULATOR)
                    effect_manager.setControlType(MANUAL_CONTROL);
                break;
            case FX_MENU_RIGHT_SYNC:
                effect_manager.setSync(!effect_manager.getSync());
                break;
            case FX_MENU_RIGHT_RANGE:
                effect_manager.setRange(effect_manager.getRange() + 1);
                break;
            case FX_MENU_RIGHT_SCALE:
                effect_manager.setScale(effect_manager.getScale() + 1);
                break;
            case FX_MENU_RIGHT_WAVE:
                effect_manager.setOscillatorShape((effect_manager.getOscillatorShape() + 1) % OSCILLATOR_SHAPE_LAST);
                break;
            case FX_MENU_RIGHT_RATIO:
//                effect_manager.setRatio(effect_manager.getRatio()+1);
                break;
            default:
                break;
        }
    }
    
    
    if(key == LEFT_ENCODER_CLICK) {
        switch(left_state_) {
            case FX_MENU_LEFT_FX:
                setLeftState(FX_MENU_LEFT_DEPTH);
                break;
            case FX_MENU_LEFT_DEPTH:
                setLeftState(FX_MENU_LEFT_FX);
                break;
            default:
                break;
        }
    }
    if(key == RIGHT_ENCODER_CLICK) {
        switch(right_state_) {
            case FX_MENU_RIGHT_MOD:
                if(effect_manager.getControlType() == MANUAL_CONTROL) {
                    break;
                }
                else if(effect_manager.getControlType() == EXTERNAL_MODULATOR) {
                    setRightState(FX_MENU_RIGHT_SCALE);
                    break;
                }
                else if(effect_manager.getControlType() == INTERNAL_MODULATOR) {
                    setRightState(FX_MENU_RIGHT_WAVE);
                    break;
                }
                break;
            case FX_MENU_RIGHT_SCALE:
                setRightState(FX_MENU_RIGHT_RANGE);
                break;
            case FX_MENU_RIGHT_WAVE:
                setRightState(FX_MENU_RIGHT_SYNC);
                break;
            case FX_MENU_RIGHT_SYNC:
                if(effect_manager.getSync()) {
                    setRightState(FX_MENU_RIGHT_RATIO);
                    break;
                } else {
                    setRightState(FX_MENU_RIGHT_FREQUENCY);
                    break;
                }
                break;
            case FX_MENU_RIGHT_RATIO:
            case FX_MENU_RIGHT_FREQUENCY:
            case FX_MENU_RIGHT_RANGE:
                setRightState(FX_MENU_RIGHT_MOD);
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

void FxMenu::paint() {
    Display::clear_screen();
    
    int y_offset = 4;
    int x_offset;
    char *effectName;
    
    if(effect_manager.getEffect()==&fm)
    {
        effectName = (char*)"FM";
    }
    else if(effect_manager.getEffect()==&ring_modulator)
    {
        effectName = (char*)"RM";
    }
    else if(effect_manager.getEffect()==&phase_distortion)
    {
        effectName = (char*)"PHDIST";
    }
    else if(effect_manager.getEffect()==&wavefolder)
    {
        effectName = (char*)"FOLD";
    }
    else if(effect_manager.getEffect()==&wavewrapper)
    {
        effectName = (char*)"WRAP";
    }
    else if(effect_manager.getEffect()==&bitcrush)
    {
        effectName = (char*)"CRUSH";
    }
    else if(effect_manager.getEffect()==&drive)
    {
        effectName = (char*)"DRIVE";
    }
    else if(effect_manager.getEffect()==&bypass)
    {
        effectName = (char*)"BYPASS";
    }
    else
    {
        effectName = (char*)"";
    }
    

    int row_height = 9;
    x_offset = 4;
    Display::put_string_9x9(x_offset, y_offset, strlen("FX"), "FX", false, 2);
    
    int centered_effect_name = (69 + 5 + 10 * 2) / 2 - strlen(effectName) * 6 / 2 - 1;
    Display::put_string_5x5(centered_effect_name, y_offset + (row_height-5) / 2, strlen(effectName), effectName, left_state_ == FX_MENU_LEFT_FX);
    
    
    
    x_offset += 64;
    Display::put_string_9x9(x_offset, y_offset, strlen("MOD"), "MOD", false, 2);
    
    char * controlName;
    if(effect_manager.getControlType() == MANUAL_CONTROL)
    {
        controlName = (char*)"POT";
    }
    else if(effect_manager.getControlType() == INTERNAL_MODULATOR)
    {
        controlName = (char*)"OSC";
    }
    else if(effect_manager.getControlType() == EXTERNAL_MODULATOR)
    {
        controlName = (char*)"IN";
    }
    else
    {
        controlName = (char*)"";
    }
    
    int centered_control_name = (64 + 5 + 10 * 3 + 128) / 2 - strlen(controlName) * 6 / 2;
    Display::put_string_5x5(centered_control_name, y_offset + (row_height-5) / 2, strlen(controlName), controlName, right_state_ == FX_MENU_RIGHT_MOD);
    
    int graph_height = 22;
    int graph_y_offset = y_offset + row_height + 1;
    Display::outline_rectangle(0, graph_y_offset, 64 - 3, graph_height);
    Display::outline_rectangle(64, graph_y_offset, 64 - 3, graph_height);
    
    uint16_t tune = adc.getChannelProcessed(0);
    uint16_t fx_amount = adc.getChannelProcessed(1);
    uint16_t fx = adc.getChannelProcessed(2);
    uint16_t morph = adc.getChannelProcessed(3);

    context.getEngine()->FillWaveform(BUF1, tune,  fx_amount,  fx,  morph);
    // abEngine.FillWaveform(BUF1, tune,  fx_amount,  fx,  morph, true);

    int depth_y_offset = graph_y_offset + graph_height + 3;
    Display::put_string_5x5(1, depth_y_offset, strlen("DEPTH:"), "DEPTH:");
    
    char depth_string[5];
    
    snprintf(depth_string, 5, "%d%%", static_cast<uint8_t>(effect_manager.getDepth() * 100.0f));
    
    Display::put_string_5x5(strlen("DEPTH:")*6 + 1, depth_y_offset, strlen(depth_string), depth_string, left_state_ == FX_MENU_LEFT_DEPTH);

    Display::Draw_Wave(1, graph_y_offset + 1, 64-3-2, graph_height - 2, BUF1);

    if(effect_manager.getControlType() == MANUAL_CONTROL) {
        char pot_value_string[4];
        snprintf(pot_value_string, 4, "%d", (fx * 100) / 4095);
        Display::put_string_9x9(64 + (64-3) / 2 - Display::get_string_9x9_width(pot_value_string, 2) / 2, graph_y_offset + graph_height / 2 - 4, strlen(pot_value_string), pot_value_string, false, 2);
    }
    else if(effect_manager.getControlType() == INTERNAL_MODULATOR) {
        Display::Draw_Wave(64+1, graph_y_offset + 1, 64-3-2, graph_height - 2, BUF1);
        
        char * waveName;
        if(effect_manager.getOscillatorShape() == SINE_SHAPE)
        {
            waveName = (char*)"SINE";
        }
        else if(effect_manager.getOscillatorShape() == TRIANGLE_SHAPE)
        {
            waveName = (char*)"TRI";
        }
        else if(effect_manager.getOscillatorShape() == SNH_SHAPE)
        {
            waveName = (char*)"S&H";
        }
        else if(effect_manager.getOscillatorShape() == SQUARE_SHAPE)
        {
            waveName = (char*)"SQUA";
        }
        else if(effect_manager.getOscillatorShape() == RAMP_SHAPE)
        {
            waveName = (char*)"RAMP";
        }
        else if(effect_manager.getOscillatorShape() == SAWTOOTH_SHAPE)
        {
            waveName = (char*)"SAW";
        }
        else
        {
            waveName = (char*)"";
        }
        
        char rs_strings[5];
        row_height = 7;
        Display::put_string_5x5(64, depth_y_offset, strlen(" WAVE:"), " WAVE:");
        Display::put_string_5x5(64 + 6*6+1, depth_y_offset, strlen(waveName), waveName, right_state_ == FX_MENU_RIGHT_WAVE);
        depth_y_offset += row_height;
        
        Display::put_string_5x5(64, depth_y_offset, strlen(" SYNC:"), " SYNC:");
        snprintf(rs_strings, 5, "%s", effect_manager.getSync() ? "ON":"OFF");
        Display::put_string_5x5(64 + 6*6+1, depth_y_offset, strlen(rs_strings), rs_strings, right_state_ == FX_MENU_RIGHT_SYNC);
        depth_y_offset += row_height;
        
        if(effect_manager.getSync()) {
            float ratio;

            if(fx >= 2048) {
                ratio = int(1.0f + (fx - 2048.0f) * 15.0f / 2047.0f);
            } else {
                ratio = int(1.0f + (2047 - fx) * 15.0f / 2047.0f);
            }

            Display::put_string_5x5(64, depth_y_offset, strlen("RATIO:"), "RATIO:");
            if(fx < 2048) {
                snprintf(rs_strings, 5, "/%.0f", ratio);
            }
            else
                snprintf(rs_strings, 5, "X%.0f", ratio);
            Display::put_string_5x5(64 + 6*6+1, depth_y_offset, strlen(rs_strings), rs_strings, right_state_ == FX_MENU_RIGHT_RATIO);
        } else {
            float frequency = pow(2, ((15.0 * fx) / 4095.0) - 3.0f);
            
            Display::put_string_5x5(64, depth_y_offset, strlen(" FREQ:"), " FREQ:");
            
            if(frequency < 10.0f)
                snprintf(rs_strings, 5, "%.2f", frequency);
            else
                snprintf(rs_strings, 5, "%.0f", frequency);
            Display::put_string_5x5(64 + 6*6+1, depth_y_offset, strlen(rs_strings), rs_strings, right_state_ == FX_MENU_RIGHT_FREQUENCY);
        }
    }
    else if(effect_manager.getControlType() == EXTERNAL_MODULATOR) {
        // TODO: draw incoming ADC FX CV value
        UpdateWaveform();
        Display::Draw_NWave(64 + 1, graph_y_offset + 1, 64 - 3 - 2, graph_height - 2, wavedata_, 59);
        
        char line[20];
        snprintf(line, 20, "%d", effect_manager.getScale());

        row_height = 7;
        Display::put_string_5x5(64, depth_y_offset, strlen("SCALE:"), "SCALE:");
        Display::put_string_5x5(64+6*6, depth_y_offset, strlen(line), line, right_state_ == FX_MENU_RIGHT_SCALE);
        depth_y_offset += row_height;
        
        snprintf(line, 20, "~%dV", effect_manager.getRange());
        Display::put_string_5x5(64, depth_y_offset, strlen("RANGE:"), "RANGE:");
        Display::put_string_5x5(64+6*6, depth_y_offset, strlen(line), line, right_state_ == FX_MENU_RIGHT_RANGE);
    }
}


void FxMenu::triggerUpdate(bool back_pressed) {
    ClearWaveform();
}

void FxMenu::ClearWaveform() {
    for(int i = 0; i < 59; i++)
        wavedata_[i] = 2048;
}

// called 48000x per second
void FxMenu::UpdateWaveform() {
    for(int i = 0; i < 58; i++) {
        wavedata_[i] = wavedata_[i+1];
    }
    wavedata_[58] = adc.getChannelProcessed(Adc::ADC_CHANNEL_FX_CV);
}

}
