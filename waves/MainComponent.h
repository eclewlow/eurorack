#pragma once

#include "waves/Context.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, juce::Component::MouseListener, juce::KeyListener, juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    void timerCallback() override;
    void mouseDown(const juce::MouseEvent &event) override;

    bool keyPressed(const juce::KeyPress &key, juce::Component *originatingComponent) override;
    bool handleKey(int key);
private:
    //==============================================================================
    // Your private member variables go here...
    float phase = 0.0f;
    bool key_map[128];
    DISALLOW_COPY_AND_ASSIGN (MainComponent)
};
