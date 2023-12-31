/*
 ==============================================================================
 
 Display.cpp
 Created: 7 Nov 2023 2:29:42pm
 Author:  Eugene Clewlow
 
 ==============================================================================
 */

#include "waves/Display.h"
#include "waves/Globals.h"
#include "string.h"
#include <stdlib.h>
#include <algorithm>

uint8_t Display::framebuffer[8][128];

// void Display::paint(juce::Graphics& g)
// {
//     g.setColour(juce::Colour(0xFF, 0xFF, 0xFF));
//     g.fillRect(0, 0, 128, 64);
    
//     if(user_settings.getInvert()) {
//         for(int page=0;page<8;page++)
//         {
//             for(int column=0;column<128;column++)
//             {
//                 framebuffer[page][column] = ~framebuffer[page][column];
//             }
//         }
//     }
    
//     for(int page=0;page<8;page++)
//     {
//         for(int column=0;column<128;column++)
//         {
//             for (int row = 0; row < 8; row++) {
//                 bool is_set = (Display::framebuffer[page][column] >> row) & 0x1;
//                 if(is_set) {
//                     //                    g.setColour(Colour newColour)
//                     g.setColour(juce::Colour(0x00, 0x00, 0x00));
//                     g.drawRect(column, page * 8 + row, 1, 1);
//                 }
//             }
//         }
//     }
// }

void Display::clear_screen() {
    memset(framebuffer, 0, 128*8);
}

void Display::Put_Pixel(uint8_t x, uint8_t y, uint8_t set, bool toggle) {
    //2.48uS ~ 4.70uS
    if(x >= 128 || y >= 64)
        return;
    
    if(toggle) {
        Display::framebuffer[y>>3][x]^=(0x01 << (y&0x07));
        return;
    }
    
    if(0 != set)
    {
        //Setting the bit
        Display::framebuffer[y>>3][x]|=(0x01 << (y&0x07));
    }
    else
    {
        //Clearing the bit
        Display::framebuffer[y>>3][x]&=~(0x01 << (y&0x07));
    }
}

void Display::Draw_Wave(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int16_t* waveform_data, bool shade) {
    uint8_t last_x = 0;
    uint8_t last_y = 0;
    for(int i = 0; i < width; i++) {
        
        float average = 0;
        
        for(int j = i * 2048 / width; j < i * 2048 / width + 2048 / width; j++) {
            average += waveform_data[j];
        }
        average /= (2048.0f / width);
        
        average = CLAMP<float>(0 - average, -32768.0f, 32767.0f);
        
        int col = i + x;
        
        int center = y + height / 2;
        int row = center + static_cast<int>((average / 32768.0) * (height / 2) - (average < 0 ? 1:0));
        if(i == 0) {
            if(row < 64 && col < 128)
                Display::framebuffer[row>>3][col]|=(0x01 << (row&0x07));
        } else {
            Display::LCD_Line(last_x, last_y, i + x, row, true);
        }
        last_x = i + x;
        last_y = row;
        
        if(shade) {
            while(row > center) {
                row--;
                if(row < 64 && col < 128)
                    Display::framebuffer[row>>3][col]|=(0x01 << (row&0x07));
            }
            while(row < center) {
                row++;
                if(row < 64 && col < 128)
                    Display::framebuffer[row>>3][col]|=(0x01 << (row&0x07));
            }
        }
    }
}

void Display::Draw_NWave(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t* waveform_data, int16_t n) {
    uint8_t last_x = 0;
    uint8_t last_y = 0;
    for(int i = 0; i < width; i++) {
        
        float sample = CLAMP<float>(2047 - waveform_data[i], -2048.0f, 2047.0f);
        
        int col = i + x;
        
        int center = y + height / 2;
        int row = center + static_cast<int>((sample / 2048.0f) * (height / 2) - (sample < 0 ? 1:0));
        if(i == 0) {
            if(row < 64 && col < 128)
                Display::framebuffer[row>>3][col]|=(0x01 << (row&0x07));
        } else {
            Display::LCD_Line(last_x, last_y, i + x, row, true);
        }
        last_x = i + x;
        last_y = row;
    }
}

void Display::LCD_Line(int16_t x0, int16_t y0,
                       int16_t x1, int16_t y1,
                       uint8_t set, bool toggle) {
    int16_t dx;
    int16_t sx;
    int16_t dy;
    int16_t sy;
    int16_t err;
    int16_t e2;
    
    dx = abs((int16_t )x1 - (int16_t )x0);
    sx = x0 < x1 ? 1 : -1;
    dy = abs((int16_t )y1 - (int16_t )y0);
    sy = y0 < y1 ? 1 : -1;
    err = (dx > dy ? dx : -dy) / 2;
    
    for (;;)
    {
        Display::Put_Pixel(x0, y0, set, toggle);
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

void Display::LCD_DottedLine(uint8_t x0, uint8_t y0,
                             uint8_t x1, uint8_t y1, int16_t dash_width, int16_t gap_width,
                             uint8_t set) {
    int8_t
    dx;
    int8_t
    sx;
    int8_t
    dy;
    int8_t
    sy;
    int8_t
    err;
    int8_t
    e2;
    dx = abs((int16_t )x1 - (int16_t )x0);
    sx = x0 < x1 ? 1 : -1;
    dy = abs((int16_t )y1 - (int16_t )y0);
    sy = y0 < y1 ? 1 : -1;
    err = (dx > dy ? dx : -dy) / 2;
    
    int counter = 0;
    bool isDash = true;
    for (;;)
    {
        if(isDash)
            Display::Put_Pixel(x0, y0, set);
        
        counter++;
        
        if(isDash && counter >= dash_width) {
            isDash = !isDash;
            counter = 0;
        }
        else if(!isDash && counter >= gap_width) {
            isDash = !isDash;
            counter = 0;
        }
        
        if ((x0 == x1) && (y0 == y1))
            break;
        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x0 = (uint16_t)((int16_t) x0 + sx);
        }
        if (e2 < dy)
        {
            err += dx;
            y0 = (uint16_t)((int16_t) y0 + sy);
        }
    }
}

void Display::put_string(uint8_t x, uint8_t y, uint8_t Field_Width, const char *input)
{
    uint8_t
    Terminator_Found;
    uint8_t
    Characters_Placed;
    uint8_t
    this_character;
    uint8_t
    *LCD_Memory;
    uint8_t
    column;
    uint8_t
    row;
    WORD_UNION
    Clearing_Mask;
    WORD_UNION
    Pixel_Data;
    
    //Get the first row of the display character.
    row=y>>3;
    //Calculate the address of the first uint8_t in display in LCD_Memory
    LCD_Memory=&Display::framebuffer[row][x];
    
    //Calculate Clearing_Mask, the vertical mask that we will and with
    //LCD_Memory to clear the space before we or in the data from the
    //font. It is 9 pixels.
    Clearing_Mask.as_word=~(0x01FF<<(y&0x07));
    
    //Clear the first col to the left of the string.
    LCD_Memory[0]&=Clearing_Mask.as_bytes[0];
    if(row<7)
        LCD_Memory[128]&=Clearing_Mask.as_bytes[1];
    LCD_Memory++;
    
    //Initialize Terminator_Found.
    Terminator_Found=0;
    //Move across the field. We will either put the character or a blank
    //in every position of Field_Width.
    for(Characters_Placed=0;Characters_Placed<Field_Width;Characters_Placed++)
    {
        //If we have not passed the terminator, then get the next
        //character in the string. If we find the terminator,
        //remember that we are out of characters.
        if(!Terminator_Found)
        {
            this_character=*input++;
            if(!this_character)
            {
                Terminator_Found=1;
                this_character=' ';
            }
        }
        else
            this_character=' ';
        //Get a pointer into the font information for this
        //character.
        
        //Write the eight columns of this character.
        for(column=0;column<=7;column++)
        {
            //Clear the correct bits in this row and the next row down.
            LCD_Memory[0]&=Clearing_Mask.as_bytes[0];
            if(row<7)
            {
                LCD_Memory[128]&=Clearing_Mask.as_bytes[1];
            }
            //Get the font data, convert it to a word and shift it down. Leave
            //one blank row of pixels above as a spacer.
            Pixel_Data.as_word=((uint16_t)Font_08x08[(this_character-FONT_08X08_BASE)][column])<<((y&0x07));
            
            //Set the correct bits in this row and the next row down.
            LCD_Memory[0]|=Pixel_Data.as_bytes[0];
            if(row<7)
            {
                LCD_Memory[128]|=Pixel_Data.as_bytes[1];
            }
            LCD_Memory++;
        }
    }
}

void Display::put_string_5x5(uint8_t x, int8_t y, uint8_t Field_Width, const char *input, bool inverted)
{
    uint8_t Terminator_Found;
    uint8_t Characters_Placed;
    uint8_t this_character;
    uint8_t *LCD_Memory;
    uint8_t column;
    uint8_t row;
    WORD_UNION Pixel_Data;
    
    
    int minus = 0;
    
    if(y < 0) {
        minus = -y;
        y = 0;
    }

    //Get the first row of the display character.
    row=y>>3;
    //Calculate the address of the first uint8_t in display in LCD_Memory
    LCD_Memory=&Display::framebuffer[row][x];
    
    //Initialize Terminator_Found.
    Terminator_Found=0;
    //Move across the field. We will either put the character or a blank
    //in every position of Field_Width.
    for(Characters_Placed=0;Characters_Placed<Field_Width;Characters_Placed++)
    {
        //If we have not passed the terminator, then get the next
        //character in the string. If we find the terminator,
        //remember that we are out of characters.
        if(!Terminator_Found)
        {
            this_character=*input++;
            if(!this_character)
            {
                Terminator_Found=1;
                this_character=' ';
            }
        }
        else
            this_character=' ';
        //Get a pointer into the font information for this
        //character.
        
        //Write the eight columns of this character.
        for(column=0;column<5;column++)
        {
            if(row > 7) continue;
            //Get the font data, convert it to a word and shift it down. Leave
            //one blank row of pixels above as a spacer.
            Pixel_Data.as_word=((uint16_t)Font_05x05[(this_character-FONT_05X05_BASE)][column])<<(y&0x07);
            Pixel_Data.as_word >>= 3;
            Pixel_Data.as_word >>= minus;
            
            //Set the correct bits in this row and the next row down.
            LCD_Memory[0]|=Pixel_Data.as_bytes[0];
            if(row<7)
            {
                LCD_Memory[128]|=Pixel_Data.as_bytes[1];
            }
            LCD_Memory++;
        }
        LCD_Memory++;
    }
    if(inverted)
        Display::invert_rectangle(x - 1, y - 1, 6 * Field_Width + 1, 7);
}

void Display::put_string_3x5(uint8_t x, int8_t y, uint8_t Field_Width, const char *input, bool inverted)
{
    uint8_t Terminator_Found;
    uint8_t Characters_Placed;
    uint8_t this_character;
    uint8_t *LCD_Memory;
    uint8_t column;
    uint8_t row;
    WORD_UNION Pixel_Data;
    
    
    int minus = 0;
    
    if(y < 0) {
        minus = -y;
        y = 0;
    }

    //Get the first row of the display character.
    row=y>>3;
    //Calculate the address of the first uint8_t in display in LCD_Memory
    LCD_Memory=&Display::framebuffer[row][x];
    
    //Initialize Terminator_Found.
    Terminator_Found=0;
    //Move across the field. We will either put the character or a blank
    //in every position of Field_Width.
    for(Characters_Placed=0;Characters_Placed<Field_Width;Characters_Placed++)
    {
        //If we have not passed the terminator, then get the next
        //character in the string. If we find the terminator,
        //remember that we are out of characters.
        if(!Terminator_Found)
        {
            this_character=*input++;
            if(!this_character)
            {
                Terminator_Found=1;
                this_character=' ';
            }
        }
        else
            this_character=' ';
        //Get a pointer into the font information for this
        //character.
        
        //Write the eight columns of this character.
        for(column=0;column<3;column++)
        {
            if(row > 7) continue;
            //Get the font data, convert it to a word and shift it down. Leave
            //one blank row of pixels above as a spacer.
            Pixel_Data.as_word=((uint16_t)Font_03x05[(this_character-FONT_05X05_BASE)][column])<<(y&0x07);
            Pixel_Data.as_word >>= 3;
            Pixel_Data.as_word >>= minus;
            
            //Set the correct bits in this row and the next row down.
            LCD_Memory[0]|=Pixel_Data.as_bytes[0];
            if(row<7)
            {
                LCD_Memory[128]|=Pixel_Data.as_bytes[1];
            }
            LCD_Memory++;
        }
        LCD_Memory++;
    }
    if(inverted)
        Display::invert_rectangle(x - 1, y - 1, 6 * Field_Width + 1, 7);
}

int16_t Display::get_string_9x9_width(const char *input, int8_t gap)
{
    return strlen(input) * 9 + (strlen(input) - 1) * gap;
}

void Display::put_string_9x9(uint8_t x, uint8_t y, uint8_t Field_Width, const char *input, bool inverted, int8_t gap)
{
    uint8_t Terminator_Found;
    uint8_t Characters_Placed;
    uint8_t this_character;
    uint8_t *LCD_Memory;
    // uint8_t column; // unused
    uint8_t row;
    WORD_UNION Pixel_Data;
    const char * save_input = input;
    
    //Get the first row of the display character.
    row=y>>3;
    //Calculate the address of the first uint8_t in display in LCD_Memory
    LCD_Memory=&Display::framebuffer[row][x];
    
    //Initialize Terminator_Found.
    Terminator_Found=0;
    //Move across the field. We will either put the character or a blank
    //in every position of Field_Width.
    for(Characters_Placed=0;Characters_Placed<Field_Width;Characters_Placed++)
    {
        //If we have not passed the terminator, then get the next
        //character in the string. If we find the terminator,
        //remember that we are out of characters.
        if(!Terminator_Found)
        {
            this_character=*input++;
            if(!this_character)
            {
                Terminator_Found=1;
                this_character=' ';
            }
        }
        else
            this_character=' ';
        
        for(uint8_t column = 0; column<9; column++)
        {
            Pixel_Data.as_word=((uint16_t)Font_09x09[(this_character-FONT_09X09_BASE)][column]);
            
            Pixel_Data.as_word <<= (y & 0x07);
            
            LCD_Memory[0]|=Pixel_Data.as_bytes[0];
            if (row < 7)
                LCD_Memory[128]|=Pixel_Data.as_bytes[1];

            if(column == 8)
                LCD_Memory += gap;
            else
                LCD_Memory ++;
        }
        LCD_Memory++;
    }
    if(inverted) {
        uint8_t gap_round_up = (gap + 2 - 1) / 2;
        Display::invert_rectangle(x - gap_round_up, y - gap_round_up, get_string_9x9_width(save_input, gap) + gap_round_up * 2, 9 + gap_round_up * 2);
    }
    
}

void Display::put_string_9x9_loop(uint8_t x, uint8_t y, uint8_t Field_Width, const char *input, bool inverted, int8_t gap, int8_t num_chars, int8_t position)
{
    uint8_t Terminator_Found;
    uint8_t Characters_Placed;
    uint8_t this_character;
    uint8_t *LCD_Memory;
    uint8_t counter = 0;
    uint8_t row;
    WORD_UNION Pixel_Data;
    const char * save_input = input;
    
    // num_chars will be 7
    
    // string width is     return strlen(input) * 9 + (strlen(input) - 1) * gap;
    // we want to draw n pixel columns. where for(n=offset, n <
    // position is from 0 to string width
    
    //Get the first row of the display character.
    row=y>>3;
    //Calculate the address of the first uint8_t in display in LCD_Memory
    LCD_Memory=&Display::framebuffer[row][x];
    
    //Initialize Terminator_Found.
    Terminator_Found=0;
    //Move across the field. We will either put the character or a blank
    //in every position of Field_Width.
    for(Characters_Placed=0;Characters_Placed<Field_Width;Characters_Placed++)
    {
        //If we have not passed the terminator, then get the next
        //character in the string. If we find the terminator,
        //remember that we are out of characters.
        if(!Terminator_Found)
        {
            this_character=*input++;
            if(!this_character)
            {
                Terminator_Found=1;
                this_character=' ';
            }
        }
        else
            this_character=' ';
        
        for(uint8_t column = 0; column<9; column++)
        {
            if(counter >= position && counter < position + (num_chars * 9 + (num_chars - 1) * gap)) {
                Pixel_Data.as_word=((uint16_t)Font_09x09[(this_character-FONT_09X09_BASE)][column]);
                
                Pixel_Data.as_word <<= (y & 0x07);
                
                LCD_Memory[0]|=Pixel_Data.as_bytes[0];
                if (row < 7)
                    LCD_Memory[128]|=Pixel_Data.as_bytes[1];

                LCD_Memory ++;
                counter++;
            } else {
                counter++;
            }
        }
        if(counter == position) {
            counter += gap;
            LCD_Memory += gap;
        } else if (counter + 1 == position) {
            counter++;
            LCD_Memory++;
        }
        else if(counter >= position && counter < position + (num_chars * 9 + (num_chars) * gap)) {
            counter += gap;
            LCD_Memory += gap;
        } else {
            counter += gap;
        }

//        counter+=gap;
//        LCD_Memory+=gap;
    }
    if(inverted) {
        uint8_t gap_round_up = (gap + 2 - 1) / 2;
        uint8_t str_len = std::min<uint8_t>(strlen(save_input), num_chars);
        uint8_t str_width = str_len * 9 + (str_len - 1) * gap;
        Display::invert_rectangle(x - gap_round_up, y - gap_round_up, str_width + gap_round_up * 2, 9 + gap_round_up * 2);
    }
    
}

void Display::put_string_5x5_loop(uint8_t x, int8_t y, uint8_t Field_Width, const char *input, bool inverted, int8_t num_chars, int8_t position)
{
    uint8_t Terminator_Found;
    uint8_t Characters_Placed;
    uint8_t this_character;
    uint8_t *LCD_Memory;
    uint8_t column;
    uint8_t counter = 0;
    uint8_t row;
    WORD_UNION Pixel_Data;
    
    const char * save_input = input;

    int minus = 0;
    
    if(y < 0) {
        minus = -y;
        y = 0;
    }

    //Get the first row of the display character.
    row=y>>3;
    //Calculate the address of the first uint8_t in display in LCD_Memory
    LCD_Memory=&Display::framebuffer[row][x];
    
    //Initialize Terminator_Found.
    Terminator_Found=0;
    //Move across the field. We will either put the character or a blank
    //in every position of Field_Width.
    for(Characters_Placed=0;Characters_Placed<Field_Width;Characters_Placed++)
    {
        //If we have not passed the terminator, then get the next
        //character in the string. If we find the terminator,
        //remember that we are out of characters.
        if(!Terminator_Found)
        {
            this_character=*input++;
            if(!this_character)
            {
                Terminator_Found=1;
                this_character=' ';
            }
        }
        else
            this_character=' ';
        //Get a pointer into the font information for this
        //character.
        
        //Write the eight columns of this character.
        for(column=0;column<5;column++)
        {
            if(row > 7) continue;

            if(counter >= position && counter < position + (num_chars * 6)) {
                
                //Get the font data, convert it to a word and shift it down. Leave
                //one blank row of pixels above as a spacer.
                Pixel_Data.as_word=((uint16_t)Font_05x05[(this_character-FONT_05X05_BASE)][column])<<(y&0x07);
                Pixel_Data.as_word >>= 3;
                Pixel_Data.as_word >>= minus;
                
                //Set the correct bits in this row and the next row down.
                LCD_Memory[0]|=Pixel_Data.as_bytes[0];
                if(row<7)
                {
                    LCD_Memory[128]|=Pixel_Data.as_bytes[1];
                }
                LCD_Memory++;
                counter++;
            } else {
                counter++;
            }
        }
        if(counter == position) {
            counter ++;
            LCD_Memory ++;
        } else if(counter >= position && counter < position + (num_chars * 6)) {
            counter ++;
            LCD_Memory ++;
        } else {
            counter ++;
        }
    }

    if(inverted) {
        uint8_t str_len = std::min<uint8_t>(strlen(save_input), num_chars);
        uint8_t str_width = str_len * 6;

        Display::invert_rectangle(x - 1, y - 1, str_width + 1, 7);
    }
}

void Display::put_image_16bit(int16_t x, int16_t y, const uint8_t image[][2], uint8_t width)
{
    uint8_t* LCD_Memory;
    DBLWORD_UNION Pixel_Data;
    
    int minus = 0;
    
    if(y < 0) {
        minus = -y;
        y = 0;
    }

    int row = y >> 3;
    
    LCD_Memory=&Display::framebuffer[row][x];
    
    //    printf("%d %d %d\n", sizeof(image), sizeof(image[0]), sizeof(image[0][0]));
    for(uint8_t column = 0; column < width; column++)
    {
        if(x + column >= 128) {
            LCD_Memory++;
            continue;
        }
        Pixel_Data.as_word=(((uint32_t)image[column][1])<<8) & 0xFF00;
        Pixel_Data.as_word|=((uint32_t)image[column][0]) & 0xFF;
        
        Pixel_Data.as_word <<= (y & 0x07);
        Pixel_Data.as_word >>= minus;
        
        LCD_Memory[0]|=Pixel_Data.as_bytes[0];
        if (row < 7)
            LCD_Memory[128]|=Pixel_Data.as_bytes[1];
        if (row < 6)
            LCD_Memory[128*2]|=Pixel_Data.as_bytes[2];
        LCD_Memory++;
    }
}

void Display::put_image_22x23(uint8_t x, uint8_t y, const uint8_t image[3][23])
{
    uint8_t* LCD_Memory;
    DBLWORD_UNION Pixel_Data;
    
    int row = y >> 3;
    // int col = x;
    
    LCD_Memory=&Display::framebuffer[row][x];
    
    //        LCD_Memory++;
    for(uint8_t column = 0; column<23; column++)
    {
        Pixel_Data.as_word=(((uint32_t)image[2][column])<<16) & 0xFF0000;
        Pixel_Data.as_word|=(((uint32_t)image[1][column])<<8) & 0xFF00;
        Pixel_Data.as_word|=((uint32_t)image[0][column]) & 0xFF;
        
        Pixel_Data.as_word <<= (y & 0x07);
        
        LCD_Memory[0]|=Pixel_Data.as_bytes[0];
        if (row < 7)
            LCD_Memory[128]|=Pixel_Data.as_bytes[1];
        if (row < 6)
            LCD_Memory[128*2]|=Pixel_Data.as_bytes[2];
        if (row < 5)
            LCD_Memory[128*3]|=Pixel_Data.as_bytes[3];
        LCD_Memory++;
    }
}

void Display::invert_rectangle(int8_t x, int8_t y,uint8_t width,uint8_t height)
{
    uint8_t
    *LCD_Memory;
    uint8_t
    *LCD_Memory_1;
    uint8_t
    *LCD_Memory_2;
    uint8_t
    mask;
    uint8_t
    column;
    
    int8_t x1 = x;
    int8_t y1 = y;
    
    int8_t x2 = x+width-1;
    int8_t y2 = y+height;
    
    if(y2 > 63) y2 = 63;
    if(y1 > 63) y1 = 63;
    if(x2 > 127) x2 = 127;
    if(x1 > 127) x1 = 127;
    if(y1 < 0) y1 = 0;
    if(x1 < 0) x1 = 0;
    if(y2 < 0) y2 = 0;
    if(x2 < 0) x2 = 0;
    //Draw the last pixel too.
    //      y2++;
    //Bail for bogus parametrers.
    if((x2<x1)||
       (y2<y1)||
       (127<x1)||
       (127<x2)||
       (63<y1)||
       (63<y2))
        return;
    //Calculate the address of the first ubyte in display in LCD_Memory
    LCD_Memory_1=&Display::framebuffer[y1>>3][x1];
    //Calculate the address of the last ubyte in display in LCD_Memory
    LCD_Memory_2=&Display::framebuffer[y2>>3][x1];
    //If they are the same, then this is a special case.
    if(LCD_Memory_1==LCD_Memory_2)
    {
        //The rectangle fits in one row.
        mask=((0xFF<<(y1&0x07))&(~(0xFF<<(y2&0x07))));
        for(column=x1;column<=x2;column++)
        {
            *LCD_Memory_1 ^= mask;
            LCD_Memory_1++;
        }
    }
    else
    {
        //This is the case where we need to loop over multiple
        //(possibly 0) lines rows. Do the top ubyte.
        mask=(0xFF<<(y1&0x07));
        LCD_Memory=LCD_Memory_1;
        for(column=x1;column<=x2;column++)
        {
            *LCD_Memory ^= mask;
            LCD_Memory++;
        }
        //Do the bytes in the middle.
        for(LCD_Memory_1+=128;LCD_Memory_1!=LCD_Memory_2;LCD_Memory_1+=128)
        {
            LCD_Memory=LCD_Memory_1;
            for(column=x1;column<=x2;column++)
            {
                *LCD_Memory ^= 0xFF;
                LCD_Memory++;
            }
        }
        //Do the bottom ubyte.
        mask=~(0xFF<<(y2&0x07));
        for(column=x1;column<=x2;column++)
        {
            *LCD_Memory_2 ^= mask;
            LCD_Memory_2++;
        }
    }
}

void Display::clear_rectangle_simple(uint8_t x,uint8_t y,uint8_t width,uint8_t height, bool set) {
    for(int i = x; i < x + width; i++) {
        for(int j = y; j < y + height; j++) {
            Put_Pixel(i, j, set);
        }
    }
}

void Display::clear_rectangle_grey(uint8_t x,uint8_t y,uint8_t width,uint8_t height) {
    for(int i = x; i < x + width; i++) {
        for(int j = y; j < y + height; j++) {
            if(i == x || j == y || i == x + width - 1 || j == y + height - 1)
                Put_Pixel(i, j, true);
            else if ( (i + j) % 2 == 0)
                Put_Pixel(i, j, false);
            else
                Put_Pixel(i, j, true);
        }
    }
}

void Display::clear_rectangle(uint8_t x,uint8_t y,uint8_t width,uint8_t height)
{
    uint8_t
    *LCD_Memory;
    uint8_t
    *LCD_Memory_1;
    uint8_t
    *LCD_Memory_2;
    uint8_t
    mask;
    uint8_t
    column;
    
    uint8_t x1 = x;
    uint8_t y1 = y;
    
    uint8_t x2 = x+width-1;
    uint8_t y2 = y+height;
    
    //Bail for bogus parametrers.
    if((x2<x1)||
       (y2<y1)||
       (127<x1)||
       (127<x2)||
       (64<y1)||
       (64<y2))
        return;
    //Calculate the address of the first ubyte in display in LCD_Memory
    LCD_Memory_1=&framebuffer[y1>>3][x1];
    //Calculate the address of the last ubyte in display in LCD_Memory
    LCD_Memory_2=&framebuffer[y2>>3][x1];
    //If they are the same, then this is a special case.
    if(LCD_Memory_1==LCD_Memory_2)
    {
        //The rectangle fits in one row.
        mask=~((0xFF<<(y1&0x07))&(~(0xFF<<(y2&0x07))));
        for(column=x1;column<=x2;column++)
        {
            *LCD_Memory_1&=mask;
            LCD_Memory_1++;
        }
    }
    else
    {
        //This is the case where we need to loop over multiple
        //(possibly 0) lines rows. Do the top ubyte.
        mask=~(0xFF<<(y1&0x07));
        LCD_Memory=LCD_Memory_1;
        for(column=x1;column<=x2;column++)
        {
            *LCD_Memory&=mask;
            LCD_Memory++;
        }
        //Do the bytes in the middle.
        for(LCD_Memory_1+=128;LCD_Memory_1!=LCD_Memory_2;LCD_Memory_1+=128)
        {
            LCD_Memory=LCD_Memory_1;
            for(column=x1;column<=x2;column++)
            {
                *LCD_Memory=0;
                LCD_Memory++;
            }
        }
        //Do the bottom ubyte.
        mask=0xFF<<(y2&0x07);
        for(column=x1;column<=x2;column++)
        {
            *LCD_Memory_2&=mask;
            LCD_Memory_2++;
        }
    }
}

//============================================================================
void Display::outline_rectangle(int16_t x, int16_t y, int16_t width, int16_t height)
{
    int16_t x1 = x;
    int16_t y1 = y;
    int16_t x2 = x + width - 1;
    int16_t y2 = y + height - 1;
    Display::LCD_Line(x1,y1,x2,y1,1);
    Display::LCD_Line(x1,y2,x2,y2,1);
    Display::LCD_Line(x1,y1,x1,y2,1);
    Display::LCD_Line(x2,y1,x2,y2,1);
}

//============================================================================
// From: http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void Display::LCD_Circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t set)
{
    uint8_t x;
    uint8_t y;
    int8_t radiusError;
    
    x = radius;
    y = 0;
    radiusError = 1 - (int8_t) x;
    
    while (x >= y)
    {
        //11 O'Clock
        Put_Pixel(x0 - y, y0 + x, set);
        //1 O'Clock
        Put_Pixel(x0 + y, y0 + x, set);
        //10 O'Clock
        Put_Pixel(x0 - x, y0 + y, set);
        //2 O'Clock
        Put_Pixel(x0 + x, y0 + y, set);
        //8 O'Clock
        Put_Pixel(x0 - x, y0 - y, set);
        //4 O'Clock
        Put_Pixel(x0 + x, y0 - y, set);
        //7 O'Clock
        Put_Pixel(x0 - y, y0 - x, set);
        //5 O'Clock
        Put_Pixel(x0 + y, y0 - x, set);
        
        y++;
        if (radiusError < 0)
        {
            radiusError += (int16_t)(2 * y + 1);
        }
        else
        {
            x--;
            radiusError += 2 * (((int16_t) y - (int16_t) x) + 1);
        }
    }
}

//============================================================================
// From: http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void Display::LCD_FillCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t set)
{
    uint8_t x;
    uint8_t y;
    int8_t radiusError;
    
    x = radius;
    y = 0;
    radiusError = 1 - (int8_t) x;
    
    while (x >= y)
    {
        //11 O'Clock
        LCD_Line(x0 - y, y0 + x, x0 - y, y0, true);
//        Put_Pixel(x0 - y, y0 + x, set);
        //1 O'Clock
        LCD_Line(x0 + y, y0 + x, x0 + y, y0, true);
//        Put_Pixel(x0 + y, y0 + x, set);
        //10 O'Clock
        LCD_Line(x0 - x, y0 + y, x0 - x, y0, true);
//        Put_Pixel(x0 - x, y0 + y, set);
        //2 O'Clock
        LCD_Line(x0 + x, y0 + y, x0 + x, y0, true);
//        Put_Pixel(x0 + x, y0 + y, set);
        //8 O'Clock
        LCD_Line(x0 - x, y0 - y, x0 - x, y0, true);
//        Put_Pixel(x0 - x, y0 - y, set);
        //4 O'Clock
        LCD_Line(x0 + x, y0 - y, x0 + x, y0, true);
//        Put_Pixel(x0 + x, y0 - y, set);
        //7 O'Clock
        LCD_Line(x0 - y, y0 - x, x0 - y, y0, true);
//        Put_Pixel(x0 - y, y0 - x, set);
        //5 O'Clock
        LCD_Line(x0 + y, y0 - x, x0 + y, y0, true);
//        Put_Pixel(x0 + y, y0 - x, set);
        
        y++;
        if (radiusError < 0)
        {
            radiusError += (int16_t)(2 * y + 1);
        }
        else
        {
            x--;
            radiusError += 2 * (((int16_t) y - (int16_t) x) + 1);
        }
    }
}
