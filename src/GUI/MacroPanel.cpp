#include "MacroPanel.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include "VoidLookAndFeel.h"
#include "MacroPanel.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include "VoidLookAndFeel.h"

MacroPanel::MacroPanel()
{
    setLookAndFeel(&voidLookAndFeel);
    for (int i = 0; i < 4; ++i)
    {
        auto* knob = new juce::Slider();
        knob->setSliderStyle(juce::Slider::RotaryVerticalDrag);
        knob->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        knob->setName("Macro " + juce::String(i + 1));
        macroKnobs.add(knob);
        addAndMakeVisible(knob);
    }
}

MacroPanel::~MacroPanel()

{
    setLookAndFeel(nullptr);
}

void MacroPanel::paint(juce::Graphics& g)
{
    try {
        g.setColour(juce::Colours::darkgrey);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 16.0f);
        g.setColour(juce::Colours::cyan);
        g.setFont(18.0f);
        g.drawFittedText("MACRO PANEL", getLocalBounds().reduced(8), juce::Justification::centred, 1);
    } catch (...) {
        // Prevent plugin crash from GUI exceptions
    }
}

void MacroPanel::resized()
{
    auto area = getLocalBounds().reduced(10);
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignItems = juce::FlexBox::AlignItems::center;
    for (auto* knob : macroKnobs)
        fb.items.add(juce::FlexItem(*knob).withMinWidth(80).withMinHeight(120).withFlex(1));
    fb.performLayout(area);
}
