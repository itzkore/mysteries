#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
class PerformanceOverlay : public juce::Component {
public:
    PerformanceOverlay();
    void paint(juce::Graphics& g) override;
};
