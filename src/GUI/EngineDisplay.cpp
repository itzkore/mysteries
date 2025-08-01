// EngineDisplay.cpp - Add real controls to visible UI
#include "EngineDisplay.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <cstdint>

EngineDisplay::EngineDisplay(SynthEngine1& engine, juce::AudioProcessorValueTreeState& apvts)
    : synthEngine(engine), apvts(apvts)
{
    // Oscillator controls
    oscWaveformBox.addItem("Sine", 1);
    oscWaveformBox.addItem("Saw", 2);
    oscWaveformBox.addItem("Square", 3);
    oscWaveformBox.setSelectedId(1);
    addAndMakeVisible(oscWaveformBox);

    oscFreqSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    oscFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    oscFreqSlider.setRange(20.0, 20000.0, 0.01);
    oscFreqSlider.setValue(440.0);
    addAndMakeVisible(oscFreqSlider);

    noiseTypeBox.addItem("White", 1);
    noiseTypeBox.addItem("Pink", 2);
    noiseTypeBox.setSelectedId(1);
    addAndMakeVisible(noiseTypeBox);

    subFreqSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    subFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    subFreqSlider.setRange(20.0, 200.0, 0.01);
    subFreqSlider.setValue(60.0);
    addAndMakeVisible(subFreqSlider);

    // Macro controls
    for (int i = 0; i < 4; ++i)
    {
        macroKnobs[i].setSliderStyle(juce::Slider::RotaryVerticalDrag);
        macroKnobs[i].setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        macroKnobs[i].setRange(0.0, 1.0, 0.001);
        macroKnobs[i].setValue(0.5);
        macroKnobs[i].setName("Macro " + juce::String(i + 1));
        addAndMakeVisible(macroKnobs[i]);
    }

    // Mod matrix
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
        {
            modMatrixButtons[r][c].setButtonText("M" + juce::String(r + 1) + "," + juce::String(c + 1));
            addAndMakeVisible(modMatrixButtons[r][c]);
        }

    // FX
    addAndMakeVisible(reverbToggle);
    addAndMakeVisible(reverbAmountSlider);
    addAndMakeVisible(delayToggle);
    addAndMakeVisible(delayTimeSlider);
    addAndMakeVisible(distortionToggle);
    addAndMakeVisible(distortionDriveSlider);

    // Set up parameter attachments
    oscWaveformAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(apvts, SynthEngine1::oscWaveformID, oscWaveformBox));
    oscFreqAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, SynthEngine1::oscFreqID, oscFreqSlider));
    noiseTypeAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(apvts, SynthEngine1::noiseTypeID, noiseTypeBox));
    subFreqAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, SynthEngine1::subFreqID, subFreqSlider));
    for (int i = 0; i < 4; ++i)
        macroAttachments[i].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, SynthEngine1::macroParamIDs[i], macroKnobs[i]));
}

EngineDisplay::~EngineDisplay() {}

void EngineDisplay::paint(juce::Graphics& g)
{
    // Professional dark background with subtle gradient
    auto bounds = getLocalBounds().toFloat();
    juce::ColourGradient gradient(juce::Colour(0xFF2A2D34), 0.0f, 0.0f,
                                 juce::Colour(0xFF1F2127), 0.0f, bounds.getHeight(), false);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(bounds, 8.0f);
    
    // Subtle border
    g.setColour(juce::Colour(0xFF404858));
    g.drawRoundedRectangle(bounds.reduced(1.0f), 8.0f, 1.0f);
    
    // Engine name with professional styling
    g.setColour(juce::Colours::cyan);
    g.setFont(juce::Font("Arial", 16.0f, juce::Font::bold));
    auto titleArea = getLocalBounds().removeFromTop(25).reduced(8, 4);
    g.drawText(getName() + " ENGINE", titleArea, juce::Justification::centredLeft);
}

void EngineDisplay::resized()
{
    auto area = getLocalBounds().reduced(5);
    area.removeFromTop(25); // Account for title area
    
    // If area is too small, don't try to layout
    if (area.getWidth() < 50 || area.getHeight() < 30) {
        return;
    }
    
    int controlHeight = 30;
    int spacing = 5;
    
    // Just place a few key controls in a simple horizontal layout
    if (area.getHeight() >= controlHeight) {
        auto controlRow = area.removeFromTop(controlHeight);
        
        // Oscillator waveform
        if (controlRow.getWidth() >= 100) {
            oscWaveformBox.setBounds(controlRow.removeFromLeft(100));
            controlRow.removeFromLeft(spacing);
        }
        
        // Frequency slider
        if (controlRow.getWidth() >= 80) {
            oscFreqSlider.setBounds(controlRow.removeFromLeft(80));
            controlRow.removeFromLeft(spacing);
        }
        
        // Sub frequency
        if (controlRow.getWidth() >= 80) {
            subFreqSlider.setBounds(controlRow.removeFromLeft(80));
        }
    }
}
