

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "GUI/MacroPanel.h"
#include "GUI/EngineDisplay.h"
#include "GUI/VoidLookAndFeel.h"


//==============================================================================
class VoidTextureSynthAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    VoidTextureSynthAudioProcessorEditor (VoidTextureSynthAudioProcessor&);
    ~VoidTextureSynthAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;


private:
    // Macro panel and engine display components
    std::unique_ptr<MacroPanel> macroPanel;
    std::unique_ptr<EngineDisplay> engineDisplay;
    // Custom LookAndFeel
    VoidLookAndFeel voidLookAndFeel;
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

    // Macro group
    juce::GroupComponent macroGroup;

    // Mod matrix UI region (scaffold)
    juce::GroupComponent modMatrixGroup;

    // --- Engine region group components ---
    juce::GroupComponent oscGroup;
    juce::GroupComponent samplerGroup;
    juce::GroupComponent noiseGroup;
    juce::GroupComponent subGroup;

    // --- Engine display components ---
    std::unique_ptr<EngineDisplay> oscDisplay;
    std::unique_ptr<EngineDisplay> samplerDisplay;
    std::unique_ptr<EngineDisplay> noiseDisplay;
    std::unique_ptr<EngineDisplay> subDisplay;

    // Preset selectors
    juce::ComboBox oscPresetBox;
    juce::ComboBox samplerPresetBox;

    // Bypass button
    juce::ToggleButton bypassButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoidTextureSynthAudioProcessorEditor)
};
