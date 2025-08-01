#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "GUI/SynthEngine1Panel.h"
#include "GUI/OrbVisualizer.h"
#include "GUI/VoidLookAndFeel.h"

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
    
    // FX section labels
    juce::Label fxLabel;
    juce::Label reverbLabel;
    juce::Label delayLabel;
    
    // Main panel components
    std::unique_ptr<OrbVisualizer> waveformDisplay;
    juce::TextButton displayModeButton;
    
    // Advanced panels
    std::unique_ptr<SynthEngine1Panel> synthEngine1Panel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    
    // Custom look and feel for cosmic aesthetic
    VoidLookAndFeel voidLookAndFeel;
    
    int currentTab = 0;
    
    // Button handlers
    void displayModeButtonClicked();
    
    VoidTextureSynthAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoidTextureSynthAudioProcessorEditor)
};
