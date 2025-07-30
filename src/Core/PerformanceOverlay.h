#pragma once
#include <JuceHeader.h>
class PerformanceOverlay : public juce::Component {
public:
    PerformanceOverlay();
    void paint(juce::Graphics& g) override;
};
