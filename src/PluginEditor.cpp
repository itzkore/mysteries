
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VoidTextureSynthAudioProcessorEditor::VoidTextureSynthAudioProcessorEditor (VoidTextureSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (800, 600);

    // Macro sliders setup
    macroSlider1.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    macroSlider1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(macroSlider1);
    macroAttachment1 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MACRO1", macroSlider1);

    macroSlider2.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    macroSlider2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(macroSlider2);
    macroAttachment2 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MACRO2", macroSlider2);

    macroSlider3.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    macroSlider3.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(macroSlider3);
    macroAttachment3 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MACRO3", macroSlider3);

    macroSlider4.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    macroSlider4.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(macroSlider4);
    macroAttachment4 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MACRO4", macroSlider4);

    // Mod matrix UI region (scaffold)
    modMatrixGroup.setText("Modulation Matrix");
    addAndMakeVisible(modMatrixGroup);

    // Document: Custom UI logic for macro controls and mod matrix
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
    // Layout macro sliders
    auto area = getLocalBounds().reduced(40);
    auto macroArea = area.removeFromTop(120);
    macroSlider1.setBounds(macroArea.removeFromLeft(120).reduced(10));
    macroSlider2.setBounds(macroArea.removeFromLeft(120).reduced(10));
    macroSlider3.setBounds(macroArea.removeFromLeft(120).reduced(10));
    macroSlider4.setBounds(macroArea.removeFromLeft(120).reduced(10));

    // Mod matrix group below macros
    modMatrixGroup.setBounds(area.removeFromTop(200).reduced(10));
}
