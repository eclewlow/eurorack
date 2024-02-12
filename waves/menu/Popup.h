/*
  ==============================================================================

    Popup.h
    Created: 28 Nov 2023 9:20:46pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once


namespace waves {

class Popup
{
public:
    Popup();
    ~Popup();
    bool handleKeyPress(int key) { return false; }
    bool handleKeyRelease(int key);
    bool handleKeyLongPress(int key) { return false; }
    void paint();
    void show(int32_t timer_length = 2000);
    void hide();
    void SetLine(int line_no, char* str);
private:
    uint32_t timer_;
    int32_t timer_length_;
    char lines_[3][20];
    bool is_showing_;
    DISALLOW_COPY_AND_ASSIGN (Popup);
};

}