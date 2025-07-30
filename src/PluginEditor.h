

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoidTextureSynthAudioProcessorEditor)
};
