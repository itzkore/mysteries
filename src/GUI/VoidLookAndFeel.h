#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class VoidLookAndFeel : public juce::LookAndFeel_V4 {
public:
    VoidLookAndFeel();
    virtual ~VoidLookAndFeel();
    void drawRotarySlider(juce::Graphics&, int, int, int, int, float, const float, float, juce::Slider&) override;
};
