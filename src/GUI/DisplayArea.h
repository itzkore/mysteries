#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
class DisplayArea : public juce::Component {
public:
    DisplayArea();
    void paint(juce::Graphics& g) override;
};
