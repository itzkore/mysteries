#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>
#include "VoidLookAndFeel.h"

class MacroPanel : public juce::Component {
public:
    MacroPanel();
    ~MacroPanel() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    juce::OwnedArray<juce::Slider> macroKnobs;
    VoidLookAndFeel voidLookAndFeel;
};
