// #include "waves/MainComponent.h"
// #include "waves/Display.h"
// #include "waves/Globals.h"

// //==============================================================================
// MainComponent::MainComponent()
// {
//     // Make sure you set the size of the component after
//     // you add any child components.
//     setSize (800, 600);
    
//     storage.Init();
//     memset(key_map, 0, 128);

//     abEngine.Init();
//     wavetableEngine.Init();
//     matrixEngine.Init();
//     drumEngine.Init();

//     context.setState(&mainMenu);
//     context.setEngine(Context::ENGINE_TYPE_AB);

    
//     // Some platforms require permissions to open input channels so request that here
//     if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
//         && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
//     {
//         juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
//                                            [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
//     }
//     else
//     {
//         // Specify the number of input and output channels that we want to open
//         setAudioChannels (2, 2);
// //        setAudioChannels(1, 1);
//     }
    
// //    context.setState(&mainMenu);
// //    context.setState(&wavetableModeMenu);
//     adc.setChannel(Adc::ADC_CHANNEL_PITCH_CV, 2048);
//     adc.setChannel(Adc::ADC_CHANNEL_FX_AMOUNT_CV, 2048);
//     adc.setChannel(Adc::ADC_CHANNEL_FX_CV, 2048);
//     adc.setChannel(Adc::ADC_CHANNEL_MORPH_CV, 2048);

//     system_clock.Init();

//     effect_manager.Init();
//     effect_manager.setEffect(EffectManager::EFFECT_TYPE_BYPASS);
// //    fx_engine.Init();
    
//     startTimer(16); // 60 / second.  1000/60
// //    startTimer(10.666);
    
//     setWantsKeyboardFocus(true);
//     getTopLevelComponent()->addKeyListener(this);
// }

// MainComponent::~MainComponent()
// {
//     // This shuts down the audio device and clears the audio source.
//     shutdownAudio();
// }

// //==============================================================================
// void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
// {
//     // This function will be called when the audio device is started, or when
//     // its settings (i.e. sample rate, block size, etc) are changed.
    
//     // You can use this function to initialise any resources you might need,
//     // but be careful - it will be called on the audio thread, not the GUI thread.
    
//     // For more details, see the help for AudioProcessor::prepareToPlay()
// }

// void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
// {
//     // Your audio-processing code goes here!
    
//     // For more details, see the help for AudioProcessor::getNextAudioBlock()
    
//     // Right now we are not producing any data, in which case we need to clear the buffer
//     // (to prevent the output of random noise)
//     //    bufferToFill.clearActiveBufferRegion();
//     //    if (listener == nullptr) return;
//     int outputChannelsNumber = bufferToFill.buffer->getNumChannels();
    
//     // get the audio samples to redirect to the audio output (numSamples represents the total number of frames needed to be played):
//     //    auto* samplesInline = listener->getNextAudioSamples(bufferToFill.numSamples, outputChannelsNumber);
//     //    get
    
//     // map the block of audio frames stored in samplesInline to the audio output:
//     int size = bufferToFill.numSamples;
//     float out[size];
//     float subosc_out[size];

//     adc.handleKeyPress();
//     handleKey(LEFT_ENCODER_CW);
//     handleKey(LEFT_ENCODER_CCW);
//     handleKey(LEFT_ENCODER_CLICK);
//     handleKey(RIGHT_ENCODER_CW);
//     handleKey(RIGHT_ENCODER_CCW);
//     handleKey(RIGHT_ENCODER_CLICK);
//     handleKey(BACK_BUTTON);

//     uint16_t tune = adc.getChannelProcessed(0);
//     uint16_t fx_amount = adc.getChannelProcessed(1);
//     uint16_t fx = adc.getChannelProcessed(2);
//     uint16_t morph = adc.getChannelProcessed(3);
    
//     context.getEngine()->Render(out, out, size, tune, fx_amount, fx, morph);
//     if(context.getLastEngine()) {
//         float last_engine_out[size];
//         context.getLastEngine()->Render(last_engine_out, last_engine_out, size, tune, fx_amount, fx, morph);
        
//         for(int i = 0; i < size; i++) {
//             float fraction = (float) i / (float) size;
//             out[i] = fraction * out[i] + (1.0f - fraction) * last_engine_out[i];
//         }
        
//         context.setLastEngine(-1);
//     }
//     suboscillator.Render(subosc_out, subosc_out, size, tune, fx_amount, fx, morph);
    
//     int16_t sample_data = CLAMP<int16_t>(context.getEngine()->GetSine(phase) * 100.0f + 2048.0f, 0, 4095);
// //    adc.setChannel(Adc::ADC_CHANNEL_PITCH_CV, sample_data);
// //    adc.setChannel(Adc::ADC_CHANNEL_FX_AMOUNT_CV, 2048);
// //    adc.setChannel(Adc::ADC_CHANNEL_FX_CV, 2048);
// //    adc.setChannel(Adc::ADC_CHANNEL_MORPH_CV, 2048);
           
//     for (auto channel = 0 ; channel < outputChannelsNumber ; channel++)
//     {
//         // get a pointer to the start sample in the buffer for this audio output channel :
//         auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

//         for (auto a = 0 ; a < bufferToFill.numSamples ; a++)
//         {
//             if(channel == 0) {
//                 float phase_increment = 0.125f / 48000.0f;
// //                float phase_increment = 1.0f / 48000.0f;
//                 phase += phase_increment;
//                 if(phase >= 1.0f)
//                     phase -= 1.0f;
//             }
//             buffer[a] = (user_settings.getSubOscMix() / 100.0f) * out[a] + (1.0f - user_settings.getSubOscMix() / 100.0f) * subosc_out[a];
//         }
//     }
// }

// void MainComponent::releaseResources()
// {
//     // This will be called when the audio device stops, or when it is being
//     // restarted due to a setting change.
    
//     // For more details, see the help for AudioProcessor::releaseResources()
// }

// //==============================================================================
// void MainComponent::paint (juce::Graphics& g)
// {
//     // (Our component is opaque, so we must completely fill the background with a solid colour)
//     g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//     g.drawRect(getWidth()/2.0f, getHeight()/2.0f, 10.0f, 10.0f);
//     // You can add your drawing code here!
// //    display.paint(g);
//     context.paint(g);
//     popup.paint(g);
//     Display::paint(g);
// }

// bool MainComponent::handleKey(int key) {
//     if(key_map[key] || juce::KeyPress::isKeyCurrentlyDown(key)) {
//         if(!juce::KeyPress::isKeyCurrentlyDown(key)) {
//             // key was released!  call keyrelease
//             key_map[key] = false;
//             bool pass = popup.handleKeyRelease(key);
//             if(!pass) {
//                 pass = context.handleKeyRelease(key);
//             }
//             return pass;

//         } else if (!key_map[key]) {
//             // key was pressed!  call keypress. start timer for managemenu
//             key_map[key] = true;
//             bool pass = popup.handleKeyPress(key);
//             if(!pass) {
//                 pass = context.handleKeyPress(key);
//             }
//             return pass;
//         } else {
//             // key is being held. call keyheld.
//             key_map[key] = true;
//             bool pass = popup.handleKeyLongPress(key);
//             if(!pass) {
//                 pass = context.handleKeyLongPress(key);
//             }
//             if(pass) {
//                 key_map[key] = false;
//             }
//             return pass;
//         }
//     }
//     return false;
// }

// void MainComponent::timerCallback()
// {
//     for(int i = 0; i < 16; i++)
//         system_clock.Tick();

//     repaint();
// };

// void MainComponent::mouseDown(const juce::MouseEvent &event) {
//     //    DBG("print"+event.getMouseDownScreenX());
//     printf("x=%d, y=%d\n", event.getMouseDownX(), event.getMouseDownY());
    
// }

// bool MainComponent::keyPressed(const juce::KeyPress &key, juce::Component *originatingComponent) {
// //    printf("%d\n", key.getKeyCode());
//     if(key.getKeyCode() == FX_AMOUNT_CV_TRIGGER) {
// //        printf("%d\n", adc.getChannel(0));
// //        if(adc.getChannelProcessed(Adc::ADC_CHANNEL_FX_AMOUNT_CV) == 2059) {
// //            adc.setChannel(Adc::ADC_CHANNEL_FX_AMOUNT_CV, 4022);
// //        } else {
// //            adc.setChannel(Adc::ADC_CHANNEL_FX_AMOUNT_CV, 2059);
// //        }
//     }
    
// //    handleKey(LEFT_ENCODER_CW);
// //    handleKey(LEFT_ENCODER_CCW);
// //    handleKey(LEFT_ENCODER_CLICK);
// //    handleKey(RIGHT_ENCODER_CW);
// //    handleKey(RIGHT_ENCODER_CCW);
// //    handleKey(RIGHT_ENCODER_CLICK);
// //    handleKey(BACK_BUTTON);
// //    bool pass = popup.handleKeyPress(key);
// //    if(!pass) {
// //        pass = context.handleKeyPress(key);
// //    }
// //    return pass;
//     return true;
// }

// void MainComponent::resized()
// {
//     // This is called when the MainContentComponent is resized.
//     // If you add any child components, this is where you should
//     // update their positions.
// }
