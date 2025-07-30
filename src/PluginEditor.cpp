
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VoidTextureSynthAudioProcessorEditor::VoidTextureSynthAudioProcessorEditor (VoidTextureSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (1200, 700); // Large modern UI

    // Modular engine displays
    oscDisplay = std::make_unique<EngineDisplay>("Oscillator", &audioProcessor);
    addAndMakeVisible(*oscDisplay);
    samplerDisplay = std::make_unique<EngineDisplay>("Sampler");
    addAndMakeVisible(*samplerDisplay);
    noiseDisplay = std::make_unique<EngineDisplay>("Noise");
    addAndMakeVisible(*noiseDisplay);
    subDisplay = std::make_unique<EngineDisplay>("Sub");
    addAndMakeVisible(*subDisplay);

    // Preset selectors
    oscPresetBox.addItem("Init", 1);
    oscPresetBox.addItem("Fat Saw", 2);
    oscPresetBox.addItem("Dark Pulse", 3);
    oscPresetBox.setSelectedId(1);
    addAndMakeVisible(oscPresetBox);
    oscPresetBox.onChange = [this]() {
        audioProcessor.setOscPreset(oscPresetBox.getSelectedId() - 1);
    };

    samplerPresetBox.addItem("Init", 1);
    samplerPresetBox.addItem("Granular Pad", 2);
    samplerPresetBox.addItem("Ghost Texture", 3);
    samplerPresetBox.setSelectedId(1);
    addAndMakeVisible(samplerPresetBox);

    // Bypass button
    bypassButton.setButtonText("Bypass");
    addAndMakeVisible(bypassButton);

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

    // Document: Custom UI logic for macro controls and mod matrix
}

VoidTextureSynthAudioProcessorEditor::~VoidTextureSynthAudioProcessorEditor() {}

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
    // Layout macro sliders in macroGroup
    auto macroArea = macroGroup.getBounds().reduced(20, 40);
    int macroSliderHeight = 40;
    macroSlider1.setBounds(macroArea.removeFromTop(macroSliderHeight));
    macroSlider2.setBounds(macroArea.removeFromTop(macroSliderHeight));
    macroSlider3.setBounds(macroArea.removeFromTop(macroSliderHeight));
    macroSlider4.setBounds(macroArea.removeFromTop(macroSliderHeight));

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
    if (oscDisplay) oscDisplay->setBounds(engineADisplayArea);

    // Engine B (Granular) display
    juce::Rectangle<int> engineBDisplayArea(padding + displayW + padding, padding, displayW, displayH);
    if (samplerDisplay) samplerDisplay->setBounds(engineBDisplayArea);

    // Engine A controls (below display)
    juce::Rectangle<int> engineAControlsArea(engineADisplayArea.getX(), engineADisplayArea.getBottom() + padding, displayW, controlH);
    oscPresetBox.setBounds(engineAControlsArea.removeFromTop(30));
    // TODO: Add more controls for Engine A here

    // Engine B controls (below display)
    juce::Rectangle<int> engineBControlsArea(engineBDisplayArea.getX(), engineBDisplayArea.getBottom() + padding, displayW, controlH);
    samplerPresetBox.setBounds(engineBControlsArea.removeFromTop(30));
    // TODO: Add more controls for Engine B here

    // Macro panel (bottom, full width)
    juce::Rectangle<int> macroPanelArea(padding, bounds.getBottom() - macroPanelH - padding, bounds.getWidth() - 2 * padding, macroPanelH);
    int knobW = 100, knobH = 100, knobSpacing = (macroPanelArea.getWidth() - 4 * knobW) / 5;
    int knobY = macroPanelArea.getY() + (macroPanelH - knobH) / 2;
    macroSlider1.setBounds(macroPanelArea.getX() + knobSpacing, knobY, knobW, knobH);
    macroSlider2.setBounds(macroPanelArea.getX() + knobSpacing * 2 + knobW, knobY, knobW, knobH);
    macroSlider3.setBounds(macroPanelArea.getX() + knobSpacing * 3 + knobW * 2, knobY, knobW, knobH);
    macroSlider4.setBounds(macroPanelArea.getX() + knobSpacing * 4 + knobW * 3, knobY, knobW, knobH);

    // Bypass button (bottom right)
    bypassButton.setBounds(bounds.getRight() - 160, bounds.getBottom() - 60, 120, 40);
}
