#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "GUI/SynthEngine1Panel.h"
#include "GUI/WaveformDisplay.h"

//==============================================================================
class VoidTextureSynthAudioProcessorEditor : public juce::AudioProcessorEditor,
                                              public juce::Button::Listener
{
public:
    VoidTextureSynthAudioProcessorEditor (VoidTextureSynthAudioProcessor&);
    ~VoidTextureSynthAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    
private:
    void updateTabVisibility();
    // Simple tab buttons - no lambdas, no complex setup
    juce::TextButton mainTabButton;
    juce::TextButton synth1TabButton;
    juce::TextButton synth2TabButton;
    
    // Single content area - we'll just change what's displayed
    juce::Component contentArea;
    
    // All controls as direct members
    juce::Label titleLabel;
    juce::Slider volumeSlider;
    juce::Label volumeLabel;
    juce::Label synth2Label;
    
    // Main panel components
    std::unique_ptr<WaveformDisplay> waveformDisplay;
    juce::TextButton displayModeButton;
    
    // Advanced panels
    std::unique_ptr<SynthEngine1Panel> synthEngine1Panel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    
    int currentTab = 0;
    
    // Button handlers
    void displayModeButtonClicked();
    
    VoidTextureSynthAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoidTextureSynthAudioProcessorEditor)
};
