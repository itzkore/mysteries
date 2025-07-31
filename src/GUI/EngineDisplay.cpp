#include "EngineDisplay.h"
#include <juce_gui_basics/juce_gui_basics.h>

EngineDisplay::EngineDisplay(SynthEngine1& engine, juce::AudioProcessorValueTreeState& apvts)
    : synthEngine(engine), apvts(apvts)
{
    setupControls();
    wireControls();
    setupMacroControls();
    setupModMatrix();
    setupAttachments();
    addAndMakeVisible(oscWaveformBox);
    addAndMakeVisible(oscFreqSlider);
    addAndMakeVisible(noiseTypeBox);
    addAndMakeVisible(subFreqSlider);
    for (int i = 0; i < 4; ++i)
        addAndMakeVisible(macroKnobs[i]);
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            addAndMakeVisible(modMatrixButtons[r][c]);
}

EngineDisplay::~EngineDisplay() {}

void EngineDisplay::setupControls()
{
    oscWaveformBox.addItem("Sine", 1);
    oscWaveformBox.addItem("Saw", 2);
    oscWaveformBox.addItem("Square", 3);
    oscWaveformBox.setSelectedId(1);

    oscFreqSlider.setRange(20.0, 20000.0, 0.01);
    oscFreqSlider.setValue(440.0);
    oscFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);

    noiseTypeBox.addItem("White", 1);
    // noiseTypeBox.addItem("Pink", 2); // Extend as needed
    noiseTypeBox.setSelectedId(1);

    subFreqSlider.setRange(20.0, 200.0, 0.01);
    subFreqSlider.setValue(60.0);
    subFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
}

void EngineDisplay::wireControls()
{
    // All controls now use APVTS attachments for automation/preset support
}

void EngineDisplay::setupAttachments()
{
    oscWaveformAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(
        apvts, SynthEngine1::oscWaveformID, oscWaveformBox));
    oscFreqAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        apvts, SynthEngine1::oscFreqID, oscFreqSlider));
    noiseTypeAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(
        apvts, SynthEngine1::noiseTypeID, noiseTypeBox));
    subFreqAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        apvts, SynthEngine1::subFreqID, subFreqSlider));
    for (int i = 0; i < 4; ++i)
        macroAttachments[i].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
            apvts, SynthEngine1::macroParamIDs[i], macroKnobs[i]));
}

void EngineDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawText("Voidwalker Synth Engine 1", getLocalBounds().removeFromTop(30), juce::Justification::centred, true);
    g.setColour(juce::Colours::grey);
    g.drawLine(0, 80, getWidth(), 80, 2.0f); // Visual separation
    g.drawText("Macro Controls", 10, 90, 200, 20, juce::Justification::left);
    g.drawText("Mod Matrix", 10, 220, 200, 20, juce::Justification::left);
}

void EngineDisplay::resized()
{
    auto area = getLocalBounds().reduced(10);
    // Top row: Oscillator and Sub controls
    auto topRow = area.removeFromTop(60);
    oscWaveformBox.setBounds(topRow.removeFromLeft(120).reduced(5));
    oscFreqSlider.setBounds(topRow.removeFromLeft(180).reduced(5));
    noiseTypeBox.setBounds(topRow.removeFromLeft(120).reduced(5));
    subFreqSlider.setBounds(topRow.removeFromLeft(180).reduced(5));

    // Macro knobs row
    auto macroRow = area.removeFromTop(80);
    for (int i = 0; i < 4; ++i)
        macroKnobs[i].setBounds(macroRow.removeFromLeft(80).reduced(5));

    // Mod matrix grid
    auto modMatrixArea = area.removeFromTop(120).reduced(5);
    int btnW = modMatrixArea.getWidth() / 4;
    int btnH = modMatrixArea.getHeight() / 4;
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            modMatrixButtons[r][c].setBounds(
                modMatrixArea.getX() + c * btnW,
                modMatrixArea.getY() + r * btnH,
                btnW - 6, btnH - 6);
}

void EngineDisplay::setupMacroControls()
{
    for (int i = 0; i < 4; ++i)
    {
        macroKnobs[i].setSliderStyle(juce::Slider::Rotary);
        macroKnobs[i].setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        macroKnobs[i].setRange(0.0, 1.0, 0.001);
        macroKnobs[i].setValue(0.5);
        macroKnobs[i].setName("Macro " + juce::String(i + 1));
    }
}

void EngineDisplay::setupModMatrix()
{
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
        {
            modMatrixButtons[r][c].setButtonText("M" + juce::String(r + 1) + "," + juce::String(c + 1));
            modMatrixButtons[r][c].setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        }
}
