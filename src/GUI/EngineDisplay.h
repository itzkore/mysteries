#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "../Engines/SynthEngine1.h"

class EngineDisplay : public juce::Component {
public:
    EngineDisplay(SynthEngine1& engine, juce::AudioProcessorValueTreeState& apvts);
    ~EngineDisplay() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    SynthEngine1& synthEngine;
    juce::AudioProcessorValueTreeState& apvts;
    juce::ComboBox oscWaveformBox;
    juce::Slider oscFreqSlider;
    juce::ComboBox noiseTypeBox;
    juce::Slider subFreqSlider;

    // Macro controls (4 knobs)
    juce::Slider macroKnobs[4];

    // Modulation matrix placeholder (grid of buttons)
    juce::TextButton modMatrixButtons[4][4];

    // APVTS attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveformAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> noiseTypeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> subFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> macroAttachments[4];

    void setupControls();
    void wireControls();
    void setupMacroControls();
    void setupModMatrix();
    void setupAttachments();
};
