/*
  ==============================================================================

    Popup.h
    Created: 28 Nov 2023 9:20:46pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class Popup
{
public:
    Popup();
    ~Popup();
    bool handleKeyPress(int key) { return false; }
    bool handleKeyRelease(int key);
    bool handleKeyLongPress(int key) { return false; }
    void paint(juce::Graphics& g);
    void show(int32_t timer_length = 2000);
    void hide();
    void SetLine(int line_no, char* str);
private:
    uint32_t timer_;
    int32_t timer_length_;
    char lines_[3][20];
    bool is_showing_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Popup);
};
