#include "AdvancedPanel.h"

AdvancedPanel::AdvancedPanel(SynthEngine1& synthEngine, juce::AudioProcessorValueTreeState& apvts)
    : synthEngine(synthEngine), apvts(apvts)
{
    // Create all 4 EngineDisplay components - same as before but in advanced tab
    oscDisplay = std::make_unique<EngineDisplay>(synthEngine, apvts);
    oscDisplay->setName("Oscillator ENGINE");
    addAndMakeVisible(*oscDisplay);

    samplerDisplay = std::make_unique<EngineDisplay>(synthEngine, apvts);
    samplerDisplay->setName("Sampler ENGINE");
    addAndMakeVisible(*samplerDisplay);

    noiseDisplay = std::make_unique<EngineDisplay>(synthEngine, apvts);
    noiseDisplay->setName("Noise ENGINE");
    addAndMakeVisible(*noiseDisplay);

    subDisplay = std::make_unique<EngineDisplay>(synthEngine, apvts);
    subDisplay->setName("Sub ENGINE");
    addAndMakeVisible(*subDisplay);

    // Create MacroPanel for advanced macro control
    macroPanel = std::make_unique<MacroPanel>(apvts);
    addAndMakeVisible(*macroPanel);
}

AdvancedPanel::~AdvancedPanel() = default;

void AdvancedPanel::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    // Professional dark gradient background
    juce::ColourGradient gradient(
        juce::Colour(0xFF2A2D34),  // Darker than main screen
        bounds.getTopLeft().toFloat(),
        juce::Colour(0xFF1F2127),  // Even darker bottom
        bounds.getBottomRight().toFloat(),
        false
    );
    g.setGradientFill(gradient);
    g.fillAll();

    // Title
    g.setColour(juce::Colour(0xFF00FFFF));
    g.setFont(juce::Font("Arial", 24.0f, juce::Font::bold));
    g.drawText("ADVANCED SYNTHESIS CONFIGURATION", bounds.removeFromTop(50), juce::Justification::centred);

    // Section dividers
    g.setColour(juce::Colour(0x40FFFFFF));
    g.drawLine(20, 270, bounds.getWidth() - 20, 270, 1.0f); // Between engine rows
    g.drawLine(20, 410, bounds.getWidth() - 20, 410, 1.0f); // Above macro panel
}

void AdvancedPanel::resized()
{
    auto bounds = getLocalBounds();
    
    // Title space
    bounds.removeFromTop(60);
    
    // Engine displays in 2x2 grid layout (same as before)
    int engineWidth = 570;
    int engineHeight = 120;
    int margin = 20;
    int centerGap = 20;
    
    // Top row - Oscillator and Sampler
    oscDisplay->setBounds(margin, bounds.getY(), engineWidth, engineHeight);
    samplerDisplay->setBounds(margin + engineWidth + centerGap, bounds.getY(), engineWidth, engineHeight);
    
    // Bottom row - Noise and Sub  
    noiseDisplay->setBounds(margin, bounds.getY() + engineHeight + 20, engineWidth, engineHeight);
    subDisplay->setBounds(margin + engineWidth + centerGap, bounds.getY() + engineHeight + 20, engineWidth, engineHeight);
    
    // Macro panel at bottom - full width
    int macroPanelY = bounds.getY() + (2 * engineHeight) + 60;
    macroPanel->setBounds(margin, macroPanelY, bounds.getWidth() - (2 * margin), 120);
}
