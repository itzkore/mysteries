#include "PluginEditor.h"

VoidTextureSynthAudioProcessorEditor::VoidTextureSynthAudioProcessorEditor(VoidTextureSynthAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    addAndMakeVisible(controlPane);
    addAndMakeVisible(displayArea);
    setSize(800, 400);
}

VoidTextureSynthAudioProcessorEditor::~VoidTextureSynthAudioProcessorEditor() {}

void VoidTextureSynthAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void VoidTextureSynthAudioProcessorEditor::resized()
{
    displayArea.setBounds(0, 0, getWidth(), getHeight() / 2);
    controlPane.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
}
