
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VoidTextureSynthAudioProcessorEditor::VoidTextureSynthAudioProcessorEditor (VoidTextureSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (800, 600);
}

VoidTextureSynthAudioProcessorEditor::~VoidTextureSynthAudioProcessorEditor() {}

void VoidTextureSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::cyan);
    g.setFont (24.0f);
    g.drawFittedText ("VOID TEXTURE SYNTH", getLocalBounds().removeFromTop(60), juce::Justification::centred, 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawFittedText ("Development Build - Core Plugin Working", getLocalBounds().removeFromBottom(40), juce::Justification::centred, 1);
}

void VoidTextureSynthAudioProcessorEditor::resized()
{
    // This is where you'll want to lay out the positions of any subcomponents in your editor..
}
