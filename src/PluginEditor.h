
#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "GUI/ControlPane.h"
#include "GUI/DisplayArea.h"

class VoidTextureSynthAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    VoidTextureSynthAudioProcessorEditor(VoidTextureSynthAudioProcessor& p);
    ~VoidTextureSynthAudioProcessorEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    VoidTextureSynthAudioProcessor& processor;
    ControlPane controlPane;
    DisplayArea displayArea;
};
