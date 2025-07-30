#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
class ControlPane : public juce::Component {
    #include <JuceHeader.h>
public:
    ControlPane();
    void paint(juce::Graphics& g) override;
};
