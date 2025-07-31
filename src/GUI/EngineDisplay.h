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
    juce::Slider macroKnobs[4] { juce::Slider(), juce::Slider(), juce::Slider(), juce::Slider() };

    // Modulation matrix (4x4 grid of buttons)
    juce::TextButton modMatrixButtons[4][4] {
        { juce::TextButton(), juce::TextButton(), juce::TextButton(), juce::TextButton() },
        { juce::TextButton(), juce::TextButton(), juce::TextButton(), juce::TextButton() },
        { juce::TextButton(), juce::TextButton(), juce::TextButton(), juce::TextButton() },
        { juce::TextButton(), juce::TextButton(), juce::TextButton(), juce::TextButton() }
    };

    // Envelope controls
    juce::Slider envAttackSlider, envDecaySlider, envSustainSlider, envReleaseSlider;

    // LFO controls
    juce::Slider lfoRateSlider, lfoDepthSlider;
    juce::ComboBox lfoWaveformBox;

    // Filter section
    juce::ComboBox filterTypeBox;
    juce::Slider filterCutoffSlider, filterResonanceSlider;

    // FX rack
    juce::ToggleButton reverbToggle, delayToggle, distortionToggle;
    juce::Slider reverbAmountSlider, delayTimeSlider, distortionDriveSlider;

    // Preset management
    juce::TextButton loadPresetButton, savePresetButton;

    // Output meter
    double outputMeterValue = 0.0;
    juce::ProgressBar outputMeter{outputMeterValue};

    // Visual waveform/spectrum display
    juce::Component waveformDisplay;

    // APVTS attachments (existing)
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveformAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> noiseTypeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> subFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> macroAttachments[4];

    // DEBUG label
    juce::Label debugLabel;

    void setupControls();
    void wireControls();
    void setupMacroControls();
    void setupModMatrix();
    void setupAttachments();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineDisplay)
};
