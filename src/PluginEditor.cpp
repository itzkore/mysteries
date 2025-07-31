#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VoidTextureSynthAudioProcessorEditor::VoidTextureSynthAudioProcessorEditor (VoidTextureSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (1200, 700); // Large modern UI

    // Engine displays
    oscDisplay = std::make_unique<EngineDisplay>(p.synthEngine1, p.apvts);
    addAndMakeVisible(*oscDisplay);
    samplerDisplay = std::make_unique<EngineDisplay>(p.synthEngine1, p.apvts); // TODO: replace with sampler engine
    addAndMakeVisible(*samplerDisplay);
    noiseDisplay = std::make_unique<EngineDisplay>(p.synthEngine1, p.apvts); // TODO: replace with noise engine
    addAndMakeVisible(*noiseDisplay);
    subDisplay = std::make_unique<EngineDisplay>(p.synthEngine1, p.apvts); // TODO: replace with sub engine
    addAndMakeVisible(*subDisplay);

    // Macro panel
    macroPanel = std::make_unique<MacroPanel>(p.apvts);
    addAndMakeVisible(*macroPanel);

    // Group components for layout
    addAndMakeVisible(macroGroup);
    addAndMakeVisible(modMatrixGroup);
    addAndMakeVisible(oscGroup);
    addAndMakeVisible(samplerGroup);
    addAndMakeVisible(noiseGroup);
    addAndMakeVisible(subGroup);

    // Document: Custom UI logic for macro controls and mod matrix
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

    // Draw section borders for engine regions
    auto oscBounds = oscGroup.getBounds();
    auto samplerBounds = samplerGroup.getBounds();
    auto noiseBounds = noiseGroup.getBounds();
    auto subBounds = subGroup.getBounds();
    g.setColour(juce::Colour(0xFF23242A));
    g.drawRect(oscBounds, 2);
    g.drawRect(samplerBounds, 2);
    g.drawRect(noiseBounds, 2);
    g.drawRect(subBounds, 2);

    // SVG-ready placeholders for engine displays
    g.setColour(juce::Colours::white.withAlpha(0.08f));
    g.fillRect(oscBounds.reduced(10));
    g.fillRect(samplerBounds.reduced(10));
    g.fillRect(noiseBounds.reduced(10));
    g.fillRect(subBounds.reduced(10));

    // Macro controls and mod matrix backgrounds
    g.setColour(juce::Colour(0xFF23242A));
    g.fillRect(macroGroup.getBounds().reduced(10));
    g.fillRect(modMatrixGroup.getBounds().reduced(10));
    // ...existing code...

    g.setColour (juce::Colours::cyan);
    g.setFont (24.0f);
    g.drawFittedText ("VOID TEXTURE SYNTH", getLocalBounds().removeFromTop(60), juce::Justification::centred, 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawFittedText ("Development Build - Core Plugin Working", getLocalBounds().removeFromBottom(40), juce::Justification::centred, 1);
}

void VoidTextureSynthAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    int margin = 20;
    int engineHeight = 180;
    int macroHeight = 120;
    int modMatrixHeight = 180;


    // Engine region layout
    oscGroup.setBounds(area.removeFromTop(engineHeight).reduced(margin));
    samplerGroup.setBounds(area.removeFromTop(engineHeight).reduced(margin));
    noiseGroup.setBounds(area.removeFromTop(engineHeight).reduced(margin));
    subGroup.setBounds(area.removeFromTop(engineHeight).reduced(margin));
    // Engine displays inside groups
    if (oscDisplay != nullptr) oscDisplay->setBounds(oscGroup.getBounds().reduced(10));
    if (samplerDisplay != nullptr) samplerDisplay->setBounds(samplerGroup.getBounds().reduced(10));
    if (noiseDisplay != nullptr) noiseDisplay->setBounds(noiseGroup.getBounds().reduced(10));
    if (subDisplay != nullptr) subDisplay->setBounds(subGroup.getBounds().reduced(10));

    // Macro panel and group
    macroGroup.setBounds(area.removeFromTop(macroHeight).reduced(margin));
    if (macroPanel != nullptr) macroPanel->setBounds(macroGroup.getBounds().reduced(10));

    // Mod matrix panel and group
    modMatrixGroup.setBounds(area.removeFromTop(modMatrixHeight).reduced(margin));
    // TODO: add modMatrixPanel->setBounds(modMatrixGroup.getBounds().reduced(10));
}
