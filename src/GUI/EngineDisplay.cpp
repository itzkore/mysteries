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

    // DEBUG: Add a large visible label
    debugLabel.setText("DEBUG: EngineDisplay visible", juce::dontSendNotification);
    debugLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    debugLabel.setColour(juce::Label::textColourId, juce::Colours::blue);
    debugLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(debugLabel);

    // Attachments
    oscWaveformAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(apvts, SynthEngine1::oscWaveformID, oscWaveformBox));
    oscFreqAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, SynthEngine1::oscFreqID, oscFreqSlider));
    noiseTypeAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(apvts, SynthEngine1::noiseTypeID, noiseTypeBox));
    subFreqAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, SynthEngine1::subFreqID, subFreqSlider));
    for (int i = 0; i < 4; ++i)
        macroAttachments[i].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, SynthEngine1::macroParamIDs[i], macroKnobs[i]));

    DBG("EngineDisplay constructed: " + juce::String::toHexString((uintptr_t)this));
}

EngineDisplay::~EngineDisplay() {}

void EngineDisplay::paint(juce::Graphics& g)
{
    // Only draw a subtle background, do not obscure children
    g.fillAll(juce::Colour(0xFF23242A));
    g.setColour(juce::Colours::darkgrey);
    g.drawRect(getLocalBounds().reduced(2), 1);
    // Draw engine name at top left
    g.setColour(juce::Colours::orange);
    g.setFont(16.0f);
    g.drawText(getName(), 8, 4, 120, 20, juce::Justification::left);
}

void EngineDisplay::resized()
{
    auto area = getLocalBounds().reduced(8);
    int rowH = 36, spacing = 8;
    int y = area.getY();
    int x = area.getX();
    int w = area.getWidth();

    // Top row: Oscillator, Sub, Noise
    oscWaveformBox.setBounds(x, y, 80, rowH); x += 88;
    oscFreqSlider.setBounds(x, y, 120, rowH); x += 128;
    subFreqSlider.setBounds(x, y, 120, rowH); x += 128;
    noiseTypeBox.setBounds(x, y, 80, rowH);

    y += rowH + spacing; x = area.getX();

    // Macro row
    for (int i = 0; i < 4; ++i) {
        macroKnobs[i].setBounds(x, y, 60, rowH); x += 68;
    }

    y += rowH + spacing; x = area.getX();

    // Mod matrix grid
    int cell = 32;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            modMatrixButtons[r][c].setBounds(x + c * (cell + 4), y + r * (cell + 4), cell, cell);
        }
    }

    y += 4 * (cell + 4) + spacing; x = area.getX();

    // FX row
    reverbToggle.setBounds(x, y, 60, rowH); x += 68;
    reverbAmountSlider.setBounds(x, y, 100, rowH); x += 108;
    delayToggle.setBounds(x, y, 60, rowH); x += 68;
    delayTimeSlider.setBounds(x, y, 100, rowH); x += 108;
    distortionToggle.setBounds(x, y, 80, rowH); x += 88;
    distortionDriveSlider.setBounds(x, y, 100, rowH);

    // DEBUG: Place debug label at bottom
    debugLabel.setBounds(10, getHeight() - 28, getWidth() - 20, 24);
}
