// ControlPane.cpp - Modernized for Voidwalker
#include "ControlPane.h"
#include <juce_gui_basics/juce_gui_basics.h>

ControlPane::ControlPane(juce::AudioProcessorValueTreeState& apvts)
    : macroPanel(apvts)
{
    addAndMakeVisible(macroPanel);
    addAndMakeVisible(fxPanel);
    addAndMakeVisible(modMatrixPanel);
}

void ControlPane::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::darkgrey);
    g.drawRect(getLocalBounds().reduced(8), 2);
}

void ControlPane::resized()
{
    auto area = getLocalBounds().reduced(16);
    // Macro panel at top
    auto macroArea = area.removeFromTop(120);
    macroPanel.setBounds(macroArea);
    // FX panel below
    auto fxArea = area.removeFromTop(80);
    fxPanel.setBounds(fxArea);
    // Mod matrix at bottom
    modMatrixPanel.setBounds(area);
}
