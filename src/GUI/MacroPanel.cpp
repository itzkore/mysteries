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
    // Professional dark background with rounded corners  
    auto bounds = getLocalBounds().toFloat();
    juce::ColourGradient gradient(juce::Colour(0xFF2A2D34), 0.0f, 0.0f,
                                 juce::Colour(0xFF1F2127), 0.0f, bounds.getHeight(), false);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(bounds, 8.0f);
    
    // Subtle border
    g.setColour(juce::Colour(0xFF404858));
    g.drawRoundedRectangle(bounds.reduced(1.0f), 8.0f, 1.0f);
    
    // Title
    g.setColour(juce::Colours::cyan);
    g.setFont(juce::Font("Arial", 16.0f, juce::Font::bold));
    auto titleArea = getLocalBounds().removeFromTop(25).reduced(8, 4);
    g.drawText("MACRO CONTROLS", titleArea, juce::Justification::centredLeft);
}

void MacroPanel::resized()
{
    auto area = getLocalBounds().reduced(5);
    area.removeFromTop(25); // Account for title area
    
    if (area.getHeight() < 60 || area.getWidth() < 200) {
        return; // Too small to layout properly
    }
    
    // Simple horizontal layout for the 4 macro knobs
    int knobWidth = juce::jmin(area.getWidth() / 4 - 5, 80);
    int knobHeight = juce::jmin(area.getHeight() - 20, 70);
    int spacing = (area.getWidth() - (knobWidth * 4)) / 5;
    int x = area.getX() + spacing;
    int y = area.getY() + 5;
    
    for (int i = 0; i < 4; ++i)
    {
        if (i < macroKnobs.size()) {
            macroKnobs[i]->setBounds(x, y, knobWidth, knobHeight);
            if (i < macroLabels.size()) {
                macroLabels[i]->setBounds(x, y + knobHeight, knobWidth, 16);
            }
        }
        x += knobWidth + spacing;
    }
}
