

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

//==============================================================================
class VoidTextureSynthAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    VoidTextureSynthAudioProcessorEditor (VoidTextureSynthAudioProcessor&);
    ~VoidTextureSynthAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    VoidTextureSynthAudioProcessor& audioProcessor;

    // Macro parameter sliders
    juce::Slider macroSlider1;
    juce::Slider macroSlider2;
    juce::Slider macroSlider3;
    juce::Slider macroSlider4;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> macroAttachment1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> macroAttachment2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> macroAttachment3;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> macroAttachment4;

    // Mod matrix UI region (scaffold)
    juce::GroupComponent modMatrixGroup;

    // Document: Custom UI logic for macro controls and mod matrix

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoidTextureSynthAudioProcessorEditor)
};
