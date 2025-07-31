#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VoidTextureSynthAudioProcessorEditor::VoidTextureSynthAudioProcessorEditor (VoidTextureSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (1200, 700); // Large modern UI

    // Add EngineDisplays and MacroPanel
    oscDisplay = std::make_unique<EngineDisplay>(p.synthEngine1, p.apvts);
    oscDisplay->setName("Oscillator");
    addAndMakeVisible(*oscDisplay);
    samplerDisplay = std::make_unique<EngineDisplay>(p.synthEngine1, p.apvts);
    samplerDisplay->setName("Sampler");
    addAndMakeVisible(*samplerDisplay);
    noiseDisplay = std::make_unique<EngineDisplay>(p.synthEngine1, p.apvts);
    noiseDisplay->setName("Noise");
    addAndMakeVisible(*noiseDisplay);
    subDisplay = std::make_unique<EngineDisplay>(p.synthEngine1, p.apvts);
    subDisplay->setName("Sub");
    addAndMakeVisible(*subDisplay);
    macroPanel = std::make_unique<MacroPanel>(p.apvts);
    addAndMakeVisible(*macroPanel);
    addAndMakeVisible(childCountLabel);
    addAndMakeVisible(debugLabel);

    // DEBUG: Add a large visible label
    debugLabel.setText("DEBUG: PluginEditor visible", juce::dontSendNotification);
    debugLabel.setFont(juce::Font(32.0f, juce::Font::bold));
    debugLabel.setColour(juce::Label::textColourId, juce::Colours::red);
    debugLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(debugLabel);

    // Child count label
    childCountLabel.setText("Children: " + juce::String(getNumChildComponents()), juce::dontSendNotification);
    childCountLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    childCountLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    childCountLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(childCountLabel);

    // Document: Custom UI logic for macro controls and mod matrix

    // After all adds
    for (int i = 0; i < getNumChildComponents(); ++i) {
        auto* c = getChildComponent(i);
        DBG("Child[" + juce::String(i) + "]: " + juce::String(typeid(*c).name()) + ", bounds: " + c->getBounds().toString() + ", visible: " + juce::String(c->isVisible() ? "true" : "false") + ", showing: " + juce::String(c->isShowing() ? "true" : "false"));
    }
}

VoidTextureSynthAudioProcessorEditor::~VoidTextureSynthAudioProcessorEditor()
{
    DBG("=== Editor Destructor START ===");
    // ...existing code...
    macroPanel.reset();
    DBG("=== Editor Destructor END ===");
}

void VoidTextureSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Dark, layered background
    g.fillAll (juce::Colour(0xFF181A20));



    g.setColour (juce::Colours::cyan);
    g.setFont (24.0f);
    g.drawFittedText ("VOID TEXTURE SYNTH", getLocalBounds().removeFromTop(60), juce::Justification::centred, 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawFittedText ("Development Build - Core Plugin Working", getLocalBounds().removeFromBottom(40), juce::Justification::centred, 1);

    // DEBUG: Draw a thick yellow border to confirm paint is called
    g.setColour(juce::Colours::yellow);
    g.drawRect(getLocalBounds(), 8);
}

void VoidTextureSynthAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    int displayHeight = 80, spacing = 8;
    int top = area.getY();
    int left = area.getX();
    int width = area.getWidth();

    if (oscDisplay) { oscDisplay->setBounds(left, top, width, displayHeight); top += displayHeight + spacing; }
    if (samplerDisplay) { samplerDisplay->setBounds(left, top, width, displayHeight); top += displayHeight + spacing; }
    if (noiseDisplay) { noiseDisplay->setBounds(left, top, width, displayHeight); top += displayHeight + spacing; }
    if (subDisplay) { subDisplay->setBounds(left, top, width, displayHeight); top += displayHeight + spacing; }
    if (macroPanel) { macroPanel->setBounds(left, top, width, 60); top += 60 + spacing; }

    debugLabel.setBounds(10, 10, getWidth() - 20, 32);
    childCountLabel.setBounds(10, 42, getWidth() - 20, 24);
}
