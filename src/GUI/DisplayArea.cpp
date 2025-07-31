// DisplayArea.cpp - Modernized for Voidwalker
#include "DisplayArea.h"
#include <juce_gui_basics/juce_gui_basics.h>

DisplayArea::DisplayArea()
{
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(spectrumDisplay);
    addAndMakeVisible(granularCloudDisplay);
}

void DisplayArea::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::darkgrey);
    g.drawRect(getLocalBounds().reduced(8), 2);
}

void DisplayArea::resized()
{
    auto area = getLocalBounds().reduced(16);
    // Waveform at top
    auto waveArea = area.removeFromTop(60);
    waveformDisplay.setBounds(waveArea);
    // Spectrum in middle
    auto specArea = area.removeFromTop(60);
    spectrumDisplay.setBounds(specArea);
    // Granular cloud at bottom
    granularCloudDisplay.setBounds(area);
}
