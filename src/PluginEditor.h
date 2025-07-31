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
private:
    VoidLookAndFeel voidLookAndFeel;
    std::unique_ptr<MacroPanel> macroPanel;
    // Only show actual engines: Oscillator, Sampler, Noise, Sub
    std::unique_ptr<EngineDisplay> oscDisplay;
    std::unique_ptr<EngineDisplay> samplerDisplay;
    std::unique_ptr<EngineDisplay> noiseDisplay;
    std::unique_ptr<EngineDisplay> subDisplay;
    VoidTextureSynthAudioProcessor& audioProcessor;
    juce::Label debugLabel;
    juce::Label childCountLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoidTextureSynthAudioProcessorEditor)
};
