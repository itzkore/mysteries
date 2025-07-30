#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
class ControlPane : public juce::Component {
#include <juce_gui_basics/juce_gui_basics.h>
public:
    ControlPane();
    void paint(juce::Graphics& g) override;
};
