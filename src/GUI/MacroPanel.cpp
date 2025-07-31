#include "MacroPanel.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "VoidLookAndFeel.h"

MacroPanel::MacroPanel(juce::AudioProcessorValueTreeState& apvts)
    : apvts(apvts)
{
    setLookAndFeel(&voidLookAndFeel);
    for (int i = 0; i < 4; ++i)
    {
        auto* knob = new juce::Slider();
        knob->setSliderStyle(juce::Slider::RotaryVerticalDrag);
        knob->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        knob->setRange(0.0, 1.0, 0.001);
        knob->setValue(0.5);
        knob->setName("Macro " + juce::String(i + 1));
        macroKnobs.add(knob);
        addAndMakeVisible(knob);
        macroAttachments.add(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, "macro" + juce::String(i + 1), *knob));
        auto* label = new juce::Label();
        label->setText("M" + juce::String(i + 1), juce::dontSendNotification);
        label->setJustificationType(juce::Justification::centred);
        macroLabels.add(label);
        addAndMakeVisible(label);
    }
}

MacroPanel::~MacroPanel()
{
    for (auto* knob : macroKnobs)
        knob->setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

void MacroPanel::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::darkgrey);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 16.0f);
    g.setColour(juce::Colours::cyan);
    g.setFont(18.0f);
    g.drawFittedText("MACRO PANEL", getLocalBounds().reduced(8), juce::Justification::centred, 1);
}

void MacroPanel::resized()
{
    auto area = getLocalBounds().reduced(16);
    int knobWidth = juce::jmin(area.getWidth() / 4, 120);
    int knobHeight = 100;
    int labelHeight = 24;
    int spacing = (area.getWidth() - (knobWidth * 4)) / 5;
    int x = area.getX() + spacing;
    int y = area.getY() + 16;
    for (int i = 0; i < 4; ++i)
    {
        macroKnobs[i]->setBounds(x, y, knobWidth, knobHeight);
        macroLabels[i]->setBounds(x, y + knobHeight + 4, knobWidth, labelHeight);
        x += knobWidth + spacing;
    }
}
