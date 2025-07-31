// MacroPanel.h - Modernized for Voidwalker
#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "VoidLookAndFeel.h"

class MacroPanel : public juce::Component {
public:
    MacroPanel(juce::AudioProcessorValueTreeState& apvts);
    ~MacroPanel() override;
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    VoidLookAndFeel voidLookAndFeel;
    juce::OwnedArray<juce::Slider> macroKnobs;
    juce::OwnedArray<juce::Label> macroLabels;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> macroAttachments;
    juce::AudioProcessorValueTreeState& apvts;
};