// VoidLookAndFeel.cpp - Modernized for Voidwalker
#include "VoidLookAndFeel.h"

VoidLookAndFeel::VoidLookAndFeel() {
    setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    setColour(juce::Label::textColourId, juce::Colours::white);
    setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::cyan);
    setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF222222));
    setColour(juce::ComboBox::backgroundColourId, juce::Colours::darkgrey);
    setColour(juce::ComboBox::textColourId, juce::Colours::white);
    setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);
}

VoidLookAndFeel::~VoidLookAndFeel() {}

void VoidLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, const float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float>(x, y, width, height).reduced(8);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto centre = bounds.getCentre();
    float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    g.setColour(slider.findColour(juce::Slider::rotarySliderOutlineColourId));
    g.drawEllipse(bounds, 2.0f);
    g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
    g.fillEllipse(bounds);
    g.setColour(juce::Colours::black);
    g.drawLine(centre.x, centre.y, centre.x + radius * std::cos(angle), centre.y + radius * std::sin(angle), 3.0f);
}
