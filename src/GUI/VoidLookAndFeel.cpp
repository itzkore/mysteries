// Ensure header is included first for correct symbol linkage
#include "VoidLookAndFeel.h"

VoidLookAndFeel::VoidLookAndFeel() {
    setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    setColour(juce::Label::textColourId, juce::Colours::white);
    setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::cyan);
    setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF222222));
}

VoidLookAndFeel::~VoidLookAndFeel() {}

void VoidLookAndFeel::drawRotarySlider(juce::Graphics&, int, int, int, int, float, const float, float, juce::Slider&) {
    // Empty for now
}
