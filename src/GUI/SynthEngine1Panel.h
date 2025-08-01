#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "../Engines/SynthEngine1.h"

class SynthEngine1Panel : public juce::Component
{
public:
    SynthEngine1Panel(juce::AudioProcessorValueTreeState& apvts, SynthEngine1& engine);
    ~SynthEngine1Panel() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& apvts;
    SynthEngine1& synthEngine;
    
    // Layer Enable/Disable Toggles
    juce::ToggleButton oscillatorEnable;
    std::unique_ptr<juce::TextButton> oscillatorSettingsButton;
    std::unique_ptr<juce::TextButton> oscillatorRandomizeButton;
    juce::ToggleButton subEnable;
    std::unique_ptr<juce::TextButton> subSettingsButton;
    std::unique_ptr<juce::TextButton> subRandomizeButton;
    juce::ToggleButton noiseEnable;
    std::unique_ptr<juce::TextButton> noiseSettingsButton;
    std::unique_ptr<juce::TextButton> noiseRandomizeButton;
    juce::ToggleButton samplerEnable;
    std::unique_ptr<juce::TextButton> samplerSettingsButton;
    std::unique_ptr<juce::TextButton> samplerRandomizeButton;
    
    // Layer Level Controls
    juce::Slider oscillatorLevel;
    juce::Slider subLevel;
    juce::Slider noiseLevel;
    juce::Slider samplerLevel;
    
    // Layer Pan Controls
    juce::Slider oscillatorPan;
    juce::Slider subPan;
    juce::Slider noisePan;
    juce::Slider samplerPan;
    
    // Oscillator Specific Controls
    juce::ComboBox oscillatorWaveform;
    juce::Slider oscillatorDetune;
    juce::Slider oscillatorOctave;
    
    // Sub Oscillator Controls
    juce::Slider subOctaveOffset;
    juce::ComboBox subWaveform;
    
    // Noise Controls
    juce::ComboBox noiseType;
    juce::Slider noiseFilterCutoff;
    
    // Labels for clarity
    juce::Label sectionLabel;
    juce::Label oscillatorLabel;
    juce::Label subLabel;
    juce::Label noiseLabel;
    juce::Label samplerLabel;
    
    // Individual control labels - more specific for better organization
    // Oscillator section
    juce::Label oscLevelLabel;
    juce::Label oscPanLabel;
    juce::Label oscWaveformLabel;
    juce::Label oscDetuneLabel;
    juce::Label oscOctaveLabel;
    
    // Sub oscillator section
    juce::Label subLevelLabel;
    juce::Label subPanLabel;
    juce::Label subWaveformLabel;
    juce::Label subOctaveLabel;
    
    // Noise section
    juce::Label noiseLevelLabel;
    juce::Label noisePanLabel;
    juce::Label noiseTypeLabel;
    juce::Label noiseFilterLabel;
    
    // Sampler section
    juce::Label samplerLevelLabel;
    juce::Label samplerPanLabel;
    
    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscillatorLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> subLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> samplerLevelAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscillatorPanAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> subPanAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noisePanAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> samplerPanAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscillatorDetuneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscillatorOctaveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> subOctaveOffsetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseFilterCutoffAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscillatorWaveformAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> subWaveformAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> noiseTypeAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> oscillatorEnableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> subEnableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> noiseEnableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> samplerEnableAttachment;
    
    void setupLayerControls();
    void setupLabels();
    void setupAttachments();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthEngine1Panel)
};
