/*
  ==============================================================================

    graphics.h
    Created: 6 Nov 2023 10:38:04pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include "stdint.h"

const uint8_t Graphic_main_menu_ab[3][23] = {
{0xff,0x1,0x1,0x1,0x81,0x81,0x81,0x81,0x1,0x1,0x1,0x1,0x81,0x1,0x81,0x81,0x81,0x81,0x81,0x1,0x1,0x1,0xff},
{0xff,0x0,0x0,0xff,0x8,0x8,0x8,0x8,0xff,0x0,0xe0,0x1c,0x3,0x0,0xff,0x88,0x88,0x88,0x88,0x77,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3f},
};

const uint8_t Graphic_main_menu_mode_select[3][23] = {
{0xff,0x1,0x1,0x1,0x1,0x1,0xe1,0x21,0x41,0x41,0x81,0x81,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0xff},
{0xff,0x0,0x0,0x0,0x0,0x0,0xff,0x80,0x40,0x40,0x20,0x20,0x11,0x11,0xa,0xa,0x4,0x4,0x0,0x0,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3f},
};

const uint8_t Graphic_main_menu_fx[3][23] = {
    {0xff,0x1,0x1,0x1,0xf1,0x11,0x11,0x11,0x11,0x11,0x11,0x1,0x31,0x41,0x81,0x1,0x81,0x41,0x31,0x1,0x1,0x1,0xff},
    {0xff,0x0,0x0,0x0,0x1f,0x1,0x81,0x41,0x41,0x40,0x80,0x0,0x18,0x4,0x2,0x1,0x2,0x4,0x98,0x40,0x0,0x0,0xff},
    {0x3f,0x20,0x20,0x24,0x22,0x21,0x20,0x20,0x20,0x20,0x20,0x21,0x22,0x24,0x24,0x24,0x22,0x21,0x20,0x20,0x20,0x20,0x3f},
    };

const uint8_t Graphic_main_menu_sub[3][23] = {
{0xff,0x1,0x1,0xe1,0x11,0x11,0x11,0x11,0x1,0xf1,0x1,0x1,0x1,0xf1,0x1,0xf1,0x11,0x11,0x11,0xe1,0x1,0x1,0xff},
{0xff,0x0,0x0,0x10,0x11,0x11,0x91,0x4e,0x40,0x4f,0x90,0x10,0x10,0xf,0x0,0x1f,0x11,0x11,0x91,0x4e,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x24,0x22,0x21,0x20,0x20,0x20,0x20,0x20,0x21,0x22,0x24,0x24,0x24,0x22,0x21,0x20,0x20,0x20,0x20,0x3f},
};

const uint8_t Graphic_main_menu_oscilloscope[3][23] = {
{0xff,0x1,0x1,0x1,0x81,0x41,0x21,0x11,0x89,0x49,0x89,0x9,0x9,0x9,0x9,0x11,0x21,0x41,0x81,0x1,0x1,0x1,0xff},
{0xff,0x0,0x0,0x3f,0x48,0x90,0x10,0xf,0x0,0x0,0x0,0x7f,0x80,0x80,0x78,0x4,0x2,0x84,0x48,0x3f,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x20,0x20,0x21,0x22,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x22,0x21,0x20,0x20,0x20,0x20,0x20,0x3f},
};

const uint8_t Graphic_main_menu_snapshots[3][23] = {
{0xff,0x1,0x1,0x1,0x81,0x81,0xc1,0xe1,0xc1,0x81,0x81,0x81,0x81,0x81,0xc1,0x21,0x21,0x21,0x21,0xc1,0x1,0x1,0xff},
{0xff,0x0,0x0,0xff,0x0,0x0,0x0,0x7c,0xc6,0x82,0x82,0x82,0xc6,0x7c,0x1,0x1,0x1,0x1,0x1,0xff,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x21,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x21,0x20,0x20,0x3f},
};
    
const uint8_t Graphic_main_menu_wave_management[3][23] = {
{0xff,0x1,0x1,0xe1,0xa1,0xe1,0x1,0x41,0x21,0x11,0x11,0x21,0x41,0x81,0x1,0x1,0x81,0x41,0x21,0x11,0x1,0x1,0xff},
{0xff,0x0,0x0,0x9c,0x94,0x9c,0x0,0x8,0x84,0x42,0x42,0x84,0x8,0x10,0x21,0x21,0x10,0x8,0x84,0x42,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x23,0x22,0x23,0x20,0x21,0x20,0x20,0x20,0x20,0x21,0x22,0x24,0x24,0x22,0x21,0x20,0x20,0x20,0x20,0x3f},
};

const uint8_t Graphic_main_menu_setup[3][23] = {
{0xff,0x1,0x1,0x1,0x1,0x1,0x1,0xe1,0x11,0x9,0x71,0x81,0x71,0x9,0x11,0xe1,0x1,0x1,0x1,0x1,0x1,0x1,0xff},
{0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0xfe,0x0,0x0,0x0,0xfe,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x27,0x28,0x24,0x28,0x27,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3f},
};

const uint8_t Graphic_mode_menu_ab[3][23] = {
{0xff,0x1,0x1,0x1,0x81,0x81,0x81,0x81,0x1,0x1,0x1,0x1,0x81,0x1,0x81,0x81,0x81,0x81,0x81,0x1,0x1,0x1,0xff},
{0xff,0x0,0x0,0xff,0x8,0x8,0x8,0x8,0xff,0x0,0xe0,0x1c,0x3,0x0,0xff,0x88,0x88,0x88,0x88,0x77,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3f},
};


const uint8_t Graphic_mode_menu_wavetable[3][23] = {
{0xff,0x1,0x1,0x1,0xf1,0x1,0x1,0x1,0x1,0x1,0xf1,0x1,0x1,0x11,0x11,0x11,0xf1,0x11,0x11,0x11,0x1,0x1,0xff},
{0xff,0x0,0x0,0x0,0xf,0x10,0xc,0x3,0xc,0x10,0xf,0x0,0x0,0x0,0x0,0x0,0x1f,0x0,0x0,0x0,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x26,0x26,0x20,0x20,0x20,0x26,0x26,0x20,0x20,0x20,0x26,0x26,0x20,0x20,0x20,0x26,0x26,0x20,0x20,0x3f},
};

const uint8_t Graphic_mode_menu_wave_matrix[3][23] = {
    {0xff,0x1,0x1,0x8d,0x8d,0x1,0x1,0x1,0x8d,0x8d,0x1,0x1,0x1,0x8d,0x8d,0x1,0x1,0x1,0x8d,0x8d,0x1,0x1,0xff},
    {0xff,0x0,0x0,0x31,0x31,0x0,0x0,0x0,0x31,0x31,0x0,0x0,0x0,0x31,0x31,0x0,0x0,0x0,0x31,0x31,0x0,0x0,0xff},
    {0x3f,0x20,0x20,0x26,0x26,0x20,0x20,0x20,0x26,0x26,0x20,0x20,0x20,0x26,0x26,0x20,0x20,0x20,0x26,0x26,0x20,0x20,0x3f},
};

const uint8_t Graphic_mode_menu_drum[3][23] = {
{0xff,0x1,0x1,0x11,0x21,0x41,0x81,0x41,0x41,0x21,0x21,0x21,0x21,0x21,0x41,0x41,0x81,0x41,0x21,0x11,0x1,0x1,0xff},
{0xff,0x0,0x0,0x0,0x3c,0xc3,0x0,0x1,0x2,0xc,0xc,0x0,0xc,0xc,0x2,0x1,0x0,0xc3,0x3c,0x0,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x20,0x20,0x21,0x22,0x22,0x24,0x24,0x24,0x24,0x24,0x22,0x22,0x21,0x20,0x20,0x20,0x20,0x20,0x3f},
};

const uint8_t Graphic_mode_menu_lasso[3][23] = {
{0xff,0x1,0x15,0x9,0x15,0x1,0x9,0x9,0x89,0x49,0x31,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0xff},
{0xff,0x0,0x0,0x0,0x18,0x24,0x22,0x11,0x10,0x8,0x8,0x4,0x4,0x82,0x42,0x21,0x11,0x9,0x6,0x0,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x23,0x24,0x24,0x24,0x24,0x20,0x2a,0x24,0x22,0x20,0x3f},
};

const uint8_t Graphic_setup_menu_brightness[3][23] = {
{0xff,0x1,0x1,0x1,0x1,0x21,0x41,0x1,0x1,0x81,0x81,0xb9,0x81,0x81,0x1,0x1,0x41,0x21,0x1,0x1,0x1,0x1,0xff},
{0xff,0x0,0x0,0x8,0x8,0x8,0x0,0x3e,0x41,0x80,0x80,0xff,0xff,0xff,0x7f,0x3e,0x0,0x8,0x8,0x8,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x20,0x22,0x21,0x20,0x20,0x20,0x20,0x2e,0x20,0x20,0x20,0x20,0x21,0x22,0x20,0x20,0x20,0x20,0x3f},
};
const uint8_t Graphic_setup_menu_calibration[3][23] = {
{0xff,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0xf9,0x79,0xf1,0xe1,0x81,0x1,0x1,0x1,0x1,0x1,0xff},
{0xff,0x0,0x0,0x0,0x0,0x0,0xc0,0xe0,0xf0,0xf0,0xf0,0xe0,0xff,0x0,0x0,0x1,0x7,0x0,0x0,0x0,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x20,0x20,0x21,0x23,0x27,0x27,0x27,0x23,0x21,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3f},
};
const uint8_t Graphic_setup_menu_info[3][23] = {
{0xff,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x61,0x61,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0xff},
{0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x1,0x1,0x1,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x20,0x20,0x20,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x20,0x20,0x20,0x20,0x20,0x20,0x3f},
};
const uint8_t Graphic_setup_menu_io[3][23] = {
{0xff,0x1,0x1,0x1,0x1,0x1,0x81,0x1,0x1,0x1,0x1,0x1,0x81,0x1,0x1,0x81,0x81,0x81,0x81,0x1,0x1,0x1,0xff},
{0xff,0x0,0x0,0x80,0x82,0x82,0xfe,0x80,0x80,0x0,0xe0,0x1c,0x3,0x0,0x7f,0x80,0x80,0x80,0x80,0x7f,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3f},
};
const uint8_t Graphic_setup_menu_memory[3][23] = {
{0xff,0x1,0x1,0x41,0x41,0x41,0xf1,0x11,0xd1,0xd1,0x11,0x11,0x11,0x11,0x11,0x11,0xf1,0x41,0x41,0x41,0x1,0x1,0xff},
{0xff,0x0,0x0,0x92,0x92,0x92,0xff,0x0,0x0,0x3c,0x8,0x10,0x8,0x3c,0x0,0x0,0xff,0x92,0x92,0x92,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x20,0x20,0x23,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x23,0x20,0x20,0x20,0x20,0x20,0x3f},
};
const uint8_t Graphic_setup_menu_oscilloscope[3][23] = {
{0xff,0x1,0x1,0x1,0x81,0x41,0x21,0x11,0x89,0x49,0x89,0x9,0x9,0x9,0x9,0x11,0x21,0x41,0x81,0x1,0x1,0x1,0xff},
{0xff,0x0,0x0,0x3f,0x48,0x90,0x10,0xf,0x0,0x0,0x0,0x7f,0x80,0x80,0x78,0x4,0x2,0x84,0x48,0x3f,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x20,0x20,0x21,0x22,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x22,0x21,0x20,0x20,0x20,0x20,0x20,0x3f},
};
const uint8_t Graphic_setup_menu_quantizer[3][23] = {
{0xff,0x1,0x1,0x1,0x1,0xf1,0x11,0x11,0xf1,0xf1,0xf1,0x11,0xf1,0xf1,0xf1,0x11,0x11,0xf1,0x1,0x1,0x1,0x1,0xff},
{0xff,0x0,0x0,0x0,0x0,0xff,0x0,0x0,0xf,0xff,0xf,0x0,0xf,0xff,0xf,0x0,0x0,0xff,0x0,0x0,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x20,0x23,0x22,0x22,0x22,0x23,0x22,0x22,0x22,0x23,0x22,0x22,0x22,0x23,0x20,0x20,0x20,0x20,0x3f},
};
const uint8_t Graphic_snapshot_menu_home[3][23] = {
{0xff,0x1,0x1,0x1,0x1,0x1,0x81,0x41,0x21,0x91,0x49,0x25,0x49,0x91,0x21,0x41,0x81,0x1,0x1,0x1,0x1,0x1,0xff},
{0xff,0x0,0x8,0x14,0xf2,0x9,0x4,0x2,0x1,0xc0,0x40,0x40,0x40,0xc0,0x1,0x2,0x4,0x9,0xf2,0x14,0x8,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x27,0x24,0x24,0x24,0x24,0x27,0x24,0x24,0x24,0x27,0x24,0x24,0x24,0x24,0x27,0x20,0x20,0x20,0x3f},
};
const uint8_t Graphic_snapshot_menu_dice[3][23] = {
{0xff,0x1,0x1,0xc1,0xc1,0xa1,0x21,0x11,0x11,0x49,0x9,0x5,0x9,0x49,0x11,0x11,0x21,0xa1,0xc1,0xc1,0x1,0x1,0xff},
{0xff,0x0,0x0,0xff,0x0,0x0,0x49,0x1,0x92,0x2,0x4,0xf8,0x4,0x82,0x2,0x21,0x1,0x8,0x0,0xff,0x0,0x0,0xff},
{0x3f,0x20,0x20,0x20,0x21,0x21,0x22,0x22,0x24,0x24,0x28,0x2f,0x28,0x24,0x24,0x22,0x22,0x21,0x21,0x20,0x20,0x20,0x3f},
};
const uint8_t Graphic_icon_edit_11x11[11][2] = {
{0x0,0x4},
{0x0,0x4},
{0xc0,0x1},
{0xe0,0x5},
{0x90,0x1},
{0x88,0x4},
{0x44,0x0},
{0x22,0x4},
{0x15,0x4},
{0xa,0x0},
{0x4,0x4},
};
const uint8_t Graphic_icon_rename_11x11[11][2] = {
{0x0,0x1},
{0x7c,0x1},
{0x14,0x1},
{0x14,0x1},
{0x14,0x1},
{0x7c,0x1},
{0x1,0x4},
{0xff,0x7},
{0x1,0x4},
{0x0,0x1},
{0x0,0x1},
};
const uint8_t Graphic_icon_delete_11x11[11][2] = {
{0x6,0x0},
{0xfa,0x3},
{0x2,0x4},
{0xf2,0x5},
{0x2,0x4},
{0xf3,0x5},
{0x2,0x4},
{0xf2,0x5},
{0x2,0x4},
{0xfa,0x3},
{0x6,0x0},
};

const uint8_t Graphic_icon_cancel_11x11[11][2] = {
{0x70,0x0},
{0x0,0x0},
{0x0,0x0},
{0x20,0x0},
{0x21,0x4},
{0xf9,0x4},
{0x21,0x4},
{0x20,0x0},
{0x0,0x0},
{0x0,0x0},
{0x70,0x0},
};

const uint8_t Graphic_icon_copy_11x11[11][2] = {
{0xff,0x1},
{0x5,0x1},
{0x3,0x1},
{0xfd,0x7},
{0x15,0x4},
{0xd,0x4},
{0x5,0x4},
{0x7,0x4},
{0x4,0x4},
{0x4,0x4},
{0xfc,0x7},
};

const uint8_t Graphic_icon_save_11x11[11][2] = {
{0xff,0x7},
{0xff,0x7},
{0xff,0x7},
{0x71,0x4},
{0x71,0x5},
{0x71,0x5},
{0x75,0x5},
{0x71,0x4},
{0xff,0x7},
{0xfe,0x7},
{0xfc,0x7},
};

const uint8_t Graphic_icon_edit_15x15[15][2] = {
{0xff,0x7f},
{0x1,0x40},
{0x1,0x50},
{0x1,0x57},
{0x81,0x56},
{0xc1,0x55},
{0xe1,0x53},
{0xf1,0x51},
{0xf9,0x50},
{0x75,0x50},
{0x29,0x50},
{0x11,0x50},
{0x1,0x50},
{0x1,0x40},
{0xff,0x7f},
};
const uint8_t Graphic_icon_delete_15x15[15][2] = {
{0xff,0x7f},
{0x1,0x40},
{0x5,0x50},
{0x9,0x48},
{0x11,0x44},
{0x21,0x42},
{0x41,0x41},
{0x81,0x40},
{0x41,0x41},
{0x21,0x42},
{0x11,0x44},
{0x9,0x48},
{0x5,0x50},
{0x1,0x40},
{0xff,0x7f},
};
const uint8_t Graphic_icon_line_15x15[15][2] = {
{0xff,0x7f},
{0x1,0x40},
{0x1,0x40},
{0x1,0x4c},
{0x1,0x4c},
{0x1,0x42},
{0x1,0x41},
{0x81,0x40},
{0x41,0x40},
{0x21,0x40},
{0x19,0x40},
{0x19,0x40},
{0x1,0x40},
{0x1,0x40},
{0xff,0x7f},
};
const uint8_t Graphic_icon_load_15x15[15][2] = {
{0xff,0x7f},
{0x1,0x40},
{0x1,0x48},
{0x1,0x40},
{0x41,0x48},
{0xc1,0x40},
{0xc1,0x49},
{0xfd,0x43},
{0xc1,0x49},
{0xc1,0x40},
{0x41,0x48},
{0x1,0x40},
{0x1,0x48},
{0x1,0x40},
{0xff,0x7f},
};
const uint8_t Graphic_icon_save_15x15[15][2] = {
{0xff,0x7f},
{0x1,0x40},
{0x1,0x40},
{0xf9,0x4f},
{0x79,0x48},
{0x79,0x4a},
{0x49,0x4a},
{0x49,0x4a},
{0x49,0x4a},
{0x79,0x4a},
{0x79,0x48},
{0xf9,0x4f},
{0x1,0x40},
{0x1,0x40},
{0xff,0x7f},
};
const uint8_t Graphic_icon_spectral_15x15[15][2] = {
{0xff,0x7f},
{0x1,0x40},
{0xf9,0x4b},
{0x1,0x48},
{0xf1,0x4b},
{0x1,0x48},
{0xe1,0x4b},
{0x1,0x48},
{0xc1,0x4b},
{0x1,0x48},
{0x81,0x4b},
{0x1,0x48},
{0x1,0x4b},
{0x1,0x40},
{0xff,0x7f},
};
const uint8_t Graphic_icon_arrow_right_9x9[9][2] = {
{0xfe,0x0},
{0x1,0x1},
{0x11,0x1},
{0x11,0x1},
{0x7d,0x1},
{0x39,0x1},
{0x11,0x1},
{0x1,0x1},
{0xfe,0x0},
};
const uint8_t Graphic_icon_arrow_left_9x9[9][2] = {
{0xfe,0x0},
{0x1,0x1},
{0x11,0x1},
{0x39,0x1},
{0x7d,0x1},
{0x11,0x1},
{0x11,0x1},
{0x1,0x1},
{0xfe,0x0},
};

const uint8_t Graphic_icon_arrow_down_9x9[9][2] = {
{0xfe,0x0},
{0x1,0x1},
{0x11,0x1},
{0x31,0x1},
{0x7d,0x1},
{0x31,0x1},
{0x11,0x1},
{0x1,0x1},
{0xfe,0x0},
};
const uint8_t Graphic_icon_arrow_up_9x9[9][2] = {
{0xfe,0x0},
{0x1,0x1},
{0x11,0x1},
{0x19,0x1},
{0x7d,0x1},
{0x19,0x1},
{0x11,0x1},
{0x1,0x1},
{0xfe,0x0},
};
const uint8_t Graphic_icon_wave_hump[15][2] = {
{0xff,0x7f},
{0x1,0x42},
{0x1,0x42},
{0x1,0x41},
{0x81,0x40},
{0x41,0x40},
{0x21,0x40},
{0x21,0x40},
{0x21,0x40},
{0x41,0x40},
{0x81,0x40},
{0x1,0x41},
{0x1,0x42},
{0x1,0x42},
{0xff,0x7f},
};
const uint8_t Graphic_icon_wave_triangle[15][2] = {
{0xff,0x7f},
{0x1,0x42},
{0x1,0x42},
{0x1,0x41},
{0x81,0x40},
{0x41,0x40},
{0x21,0x40},
{0x11,0x40},
{0x21,0x40},
{0x41,0x40},
{0x81,0x40},
{0x1,0x41},
{0x1,0x42},
{0x1,0x42},
{0xff,0x7f},
};
const uint8_t Graphic_icon_wave_ramp[15][2] = {
{0xff,0x7f},
{0x1,0x42},
{0x1,0x42},
{0x1,0x42},
{0x1,0x41},
{0x81,0x40},
{0x41,0x40},
{0x21,0x40},
{0x11,0x40},
{0xf9,0x43},
{0x1,0x42},
{0x1,0x42},
{0x1,0x42},
{0x1,0x42},
{0xff,0x7f},
};
const uint8_t Graphic_icon_wave_reverse_ramp[15][2] = {
{0xff,0x7f},
{0x1,0x42},
{0x1,0x42},
{0x1,0x42},
{0x1,0x42},
{0xf9,0x43},
{0x11,0x40},
{0x21,0x40},
{0x41,0x40},
{0x81,0x40},
{0x1,0x41},
{0x1,0x42},
{0x1,0x42},
{0x1,0x42},
{0xff,0x7f},
};
const uint8_t Graphic_icon_wave_spike[15][2] = {
{0xff,0x7f},
{0x1,0x42},
{0x1,0x42},
{0x1,0x42},
{0x1,0x42},
{0x1,0x41},
{0xc1,0x40},
{0x39,0x40},
{0xc1,0x40},
{0x1,0x41},
{0x1,0x42},
{0x1,0x42},
{0x1,0x42},
{0x1,0x42},
{0xff,0x7f},
};
const uint8_t Graphic_icon_wave_square[15][2] = {
{0xff,0x7f},
{0x1,0x42},
{0x1,0x42},
{0x1,0x42},
{0xf1,0x43},
{0x11,0x40},
{0x11,0x40},
{0x11,0x40},
{0x11,0x40},
{0x11,0x40},
{0xf1,0x43},
{0x1,0x42},
{0x1,0x42},
{0x1,0x42},
{0xff,0x7f},
};
