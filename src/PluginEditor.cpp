#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VoidTextureSynthAudioProcessorEditor::VoidTextureSynthAudioProcessorEditor (VoidTextureSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    DBG("=== Editor Constructor ENTRY ===");
    try {
        DBG("=== Editor Constructor START ===");
    setSize (1200, 700); // Large modern UI

    // Engine displays (OpenGL)
        wavetableDisplay = std::make_unique<EngineDisplay>("WAVETABLE");
        if (wavetableDisplay) addAndMakeVisible(*wavetableDisplay);
        granularDisplay = std::make_unique<EngineDisplay>("GRANULAR");
        if (granularDisplay) addAndMakeVisible(*granularDisplay);
    
    // Macro panel
        macroPanel = std::make_unique<MacroPanel>();
        if (macroPanel) addAndMakeVisible(*macroPanel);

    // Preset selectors
    oscPresetBox.addItem("Init", 1);
    oscPresetBox.addItem("Fat Saw", 2);
    oscPresetBox.addItem("Dark Pulse", 3);
    oscPresetBox.setSelectedId(1);
        addAndMakeVisible(oscPresetBox);
    oscPresetBox.onChange = [this]() {
        audioProcessor.setOscPreset(oscPresetBox.getSelectedId() - 1);
    };
        addAndMakeVisible(samplerPresetBox);

    // Bypass button
    bypassButton.setButtonText("Bypass");
        addAndMakeVisible(bypassButton);

    // Macro group
        macroGroup.setText("Macro Controls");
        addAndMakeVisible(macroGroup);

    // Mod matrix UI region (scaffold)
        modMatrixGroup.setText("Modulation Matrix");
        addAndMakeVisible(modMatrixGroup);

    // Engine region group components
        oscGroup.setText("Oscillator");
        addAndMakeVisible(oscGroup);
        samplerGroup.setText("Sampler");
        addAndMakeVisible(samplerGroup);
        noiseGroup.setText("Noise");
        addAndMakeVisible(noiseGroup);
        subGroup.setText("Sub");
        addAndMakeVisible(subGroup);
        DBG("=== Editor Constructor END ===");
    } catch (std::exception& e) {
        DBG("Exception in Editor constructor: " << e.what());
    } catch (...) {
        DBG("Unknown exception in Editor constructor");
    }

    // Document: Custom UI logic for macro controls and mod matrix
}

VoidTextureSynthAudioProcessorEditor::~VoidTextureSynthAudioProcessorEditor()
{
    DBG("=== Editor Destructor START ===");
    wavetableDisplay.reset();
    granularDisplay.reset();
    macroPanel.reset();
    DBG("=== Editor Destructor END ===");
}

void VoidTextureSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    try {
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
    } catch (...) {
        // Prevent plugin crash from GUI exceptions
    }

    // Macro controls and mod matrix backgrounds
    g.setColour(juce::Colour(0xFF23242A));
    g.fillRect(macroGroup.getBounds().reduced(10));
    g.fillRect(modMatrixGroup.getBounds().reduced(10));
    // Layout macro sliders in macroGroup
    auto macroArea = macroGroup.getBounds().reduced(20, 40);

    // Layout mod matrix cells in modMatrixGroup (scaffold)
    auto modArea = modMatrixGroup.getBounds().reduced(20, 40);
    int cellSize = 40;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            g.drawRect(modArea.getX() + i * cellSize, modArea.getY() + j * cellSize, cellSize, cellSize, 1);

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
    auto area = getLocalBounds().reduced(20);
    auto topRow = area.removeFromTop(area.getHeight() / 2);
    auto bottomRow = area;

    // --- Precise, non-overlapping layout ---
    auto bounds = getLocalBounds();
    const int padding = 20;
    const int displayW = 350, displayH = 250;
    const int controlH = 100;
    const int macroPanelH = 120;

    // Engine A (Wavetable) display
    juce::Rectangle<int> engineADisplayArea(padding, padding, displayW, displayH);
    if (wavetableDisplay)
        wavetableDisplay->setBounds(engineADisplayArea);

    juce::Rectangle<int> engineBDisplayArea(padding + displayW + padding, padding, displayW, displayH);
    if (granularDisplay)
        granularDisplay->setBounds(engineBDisplayArea);

    juce::Rectangle<int> engineAControlsArea(engineADisplayArea.getX(), engineADisplayArea.getBottom() + padding, displayW, controlH);
    oscPresetBox.setBounds(engineAControlsArea.removeFromTop(30));
    // TODO: Add more controls for Engine A here

    juce::Rectangle<int> engineBControlsArea(engineBDisplayArea.getX(), engineBDisplayArea.getBottom() + padding, displayW, controlH);
    samplerPresetBox.setBounds(engineBControlsArea.removeFromTop(30));
    // TODO: Add more controls for Engine B here

    if (macroPanel)
        macroPanel->setBounds(area.removeFromTop(macroPanelH).reduced(20, 0));
    oscPresetBox.setBounds(area.removeFromTop(40).removeFromLeft(400).reduced(20, 0));
    samplerPresetBox.setBounds(area.removeFromTop(40).removeFromLeft(400).reduced(20, 0));
    if (wavetableDisplay)
        wavetableDisplay->setBounds(20, 120, 200, 200);
    if (granularDisplay)
        granularDisplay->setBounds(240, 120, 200, 200);
    juce::Rectangle<int> macroPanelArea(padding, bounds.getBottom() - macroPanelH - padding, bounds.getWidth() - 2 * padding, macroPanelH);
    int knobW = 100, knobH = 100, knobSpacing = (macroPanelArea.getWidth() - 4 * knobW) / 5;
    int knobY = macroPanelArea.getY() + (macroPanelH - knobH) / 2;

    bypassButton.setBounds(bounds.getRight() - 160, bounds.getBottom() - 60, 120, 40);
}
