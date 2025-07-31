// ControlPane.h - Modernized for Voidwalker
#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "MacroPanel.h"

class ControlPane : public juce::Component {
public:
    ControlPane(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    MacroPanel macroPanel;
    juce::Component fxPanel;         // Placeholder for FX rack
    juce::Component modMatrixPanel;  // Placeholder for modulation matrix
};
