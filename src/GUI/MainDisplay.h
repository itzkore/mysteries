#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "../Engines/SynthEngine1.h"
#include "VoidLookAndFeel.h"

/**
 * MainDisplay - The stunning main screen with simplified controls and graphics
 * Features: Large visual displays, essential synthesis controls, inspiring workflow
 */
class MainDisplay : public juce::Component
{
public:
    MainDisplay(SynthEngine1& synthEngine, juce::AudioProcessorValueTreeState& apvts);
    ~MainDisplay() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    SynthEngine1& synthEngine;
    juce::AudioProcessorValueTreeState& apvts;

    // Main visual elements
    std::unique_ptr<juce::Component> waveformVisualizer;
    std::unique_ptr<juce::Component> spectrumAnalyzer;
    std::unique_ptr<juce::Component> modMatrixVisual;

    // Essential controls - simplified and large
    std::unique_ptr<juce::Slider> masterVolume;
    std::unique_ptr<juce::Slider> filterCutoff;
    std::unique_ptr<juce::Slider> filterResonance;
    std::unique_ptr<juce::Slider> attackTime;
    std::unique_ptr<juce::Slider> releaseTime;
    
    // Quick preset selector
    std::unique_ptr<juce::ComboBox> presetSelector;
    
    // Performance controls
    std::unique_ptr<juce::Slider> performanceX;
    std::unique_ptr<juce::Slider> performanceY;

    // APVTS attachments for real-time parameter control
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainDisplay)
};
