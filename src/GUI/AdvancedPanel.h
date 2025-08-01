#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "../Engines/SynthEngine1.h"
#include "EngineDisplay.h"
#include "MacroPanel.h"
#include "VoidLookAndFeel.h"

/**
 * AdvancedPanel - Detailed SynthEngine1 configuration panel
 * Features: All 4 engine displays, macro panel, modulation matrix, detailed controls
 */
class AdvancedPanel : public juce::Component
{
public:
    AdvancedPanel(SynthEngine1& synthEngine, juce::AudioProcessorValueTreeState& apvts);
    ~AdvancedPanel() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    SynthEngine1& synthEngine;
    juce::AudioProcessorValueTreeState& apvts;

    // Engine displays - detailed synthesis control
    std::unique_ptr<EngineDisplay> oscDisplay;
    std::unique_ptr<EngineDisplay> samplerDisplay; 
    std::unique_ptr<EngineDisplay> noiseDisplay;
    std::unique_ptr<EngineDisplay> subDisplay;

    // Macro control panel
    std::unique_ptr<MacroPanel> macroPanel;

    // Modulation matrix visual
    std::unique_ptr<juce::Component> modMatrixDisplay;

    // Advanced synthesis controls
    std::unique_ptr<juce::Component> fxRackPanel;
    std::unique_ptr<juce::Component> routingPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdvancedPanel)
};
