#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "GUI/SynthEngine1Panel.h"
#include "GUI/OrbVisualizer.h"
#include "GUI/VoidLookAndFeel.h"

//==============================================================================
class VoidTextureSynthAudioProcessorEditor : public juce::AudioProcessorEditor,
                                              public juce::Button::Listener
{
public:
    VoidTextureSynthAudioProcessorEditor (VoidTextureSynthAudioProcessor&);
    ~VoidTextureSynthAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    
private:
    void updateTabVisibility();
    void setupFXControls();
    void setFXControlsVisible(bool visible);
    
    // Professional layout methods for clean organization
    void layoutMainTab(juce::Rectangle<int> bounds);
    void layoutSynth1Section(juce::Rectangle<int> area);
    void layoutVisualSection(juce::Rectangle<int> area);
    void layoutBottomControls(juce::Rectangle<int> area);
    void layoutFXSection(juce::Rectangle<int> area);
    void layoutMacroSection(juce::Rectangle<int> area);
    void layoutFXModule(juce::Rectangle<int> bounds, 
                        juce::Label& headerLabel,
                        juce::Slider& slider1, juce::Slider& slider2,
                        juce::Label& label1, juce::Label& label2,
                        const juce::String& title);
    void layoutFXModule6Controls(juce::Rectangle<int> bounds, 
                                juce::Label& headerLabel,
                                std::vector<juce::Slider*> sliders,
                                std::vector<juce::Label*> labels,
                                const juce::String& title);
    void layoutMacroControl(juce::Rectangle<int> bounds, 
                           juce::Slider& slider, 
                           juce::Label& label);
    // Simple tab buttons - no lambdas, no complex setup
    juce::TextButton mainTabButton;
    juce::TextButton synth1TabButton;
    juce::TextButton synth2TabButton;
    
    // Single content area - we'll just change what's displayed
    juce::Component contentArea;
    
    // All controls as direct members
    juce::Label titleLabel;
    juce::Slider volumeSlider;
    juce::Label volumeLabel;
    juce::Label synth2Label;
    
    // FX section labels
    juce::Label fxLabel;
    // FX Module Controls - 6 controllers each for professional control
    // Reverb FX (6 controls)
    juce::Slider reverbSizeSlider, reverbDampSlider, reverbWidthSlider;
    juce::Slider reverbPreDelaySlider, reverbLowCutSlider, reverbHighCutSlider;
    juce::Label reverbSizeLabel, reverbDampLabel, reverbWidthLabel;
    juce::Label reverbPreDelayLabel, reverbLowCutLabel, reverbHighCutLabel;
    juce::Label reverbLabel;
    
    // Delay FX (6 controls)
    juce::Slider delayTimeSlider, delayFeedbackSlider, delayMixSlider;
    juce::Slider delayLowCutSlider, delayHighCutSlider, delayStereoSlider;
    juce::Label delayTimeLabel, delayFeedbackLabel, delayMixLabel;
    juce::Label delayLowCutLabel, delayHighCutLabel, delayStereoLabel;
    juce::Label delayLabel;
    
    // Distortion FX (6 controls)
    juce::Slider distortionDriveSlider, distortionMixSlider, distortionToneSlider;
    juce::Slider distortionGainSlider, distortionBiasSlider, distortionTypeSlider;
    juce::Label distortionDriveLabel, distortionMixLabel, distortionToneLabel;
    juce::Label distortionGainLabel, distortionBiasLabel, distortionTypeLabel;
    juce::Label distortionLabel;
    
    // Filter FX (6 controls)
    juce::Slider filterCutoffSlider, filterResonanceSlider, filterDriveSlider;
    juce::Slider filterEnvSlider, filterLFOSlider, filterTypeSlider;
    juce::Label filterCutoffLabel, filterResonanceLabel, filterDriveLabel;
    juce::Label filterEnvLabel, filterLFOLabel, filterTypeLabel;
    juce::Label filterLabel;
    
    // FX section header
    juce::Label fxHeaderLabel;
    
    // Macro Controls for Synth 1 (on Main tab)
    juce::Slider macro1Slider, macro2Slider, macro3Slider, macro4Slider;
    juce::Label macro1Label, macro2Label, macro3Label, macro4Label;
    juce::Label macroHeaderLabel;
    
    // Additional UI Elements for Professional Layout
    // Performance controls
    juce::Slider polyCountSlider, unisonSlider, unisonDetuneSlider;
    juce::Label polyCountLabel, unisonLabel, unisonDetuneLabel;
    juce::Label performanceLabel;
    
    // Modulation section
    juce::Slider modWheelSlider, pitchBendSlider, velocitySlider;
    juce::Label modWheelLabel, pitchBendLabel, velocityLabel;
    juce::Label modulationLabel;
    
    // Output section
    juce::Slider panSlider, widthSlider;
    juce::Label panLabel, widthLabel;
    juce::Label outputLabel;
    
    // Preset controls
    juce::TextButton presetPrevButton, presetNextButton;
    juce::ComboBox presetComboBox;
    juce::Label presetLabel;
    
    // Main panel components
    std::unique_ptr<OrbVisualizer> waveformDisplay;
    juce::TextButton displayModeButton;
    
    // Advanced panels
    std::unique_ptr<SynthEngine1Panel> synthEngine1Panel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    
    // Macro parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> macro1Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> macro2Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> macro3Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> macro4Attachment;
    
    // Custom look and feel for cosmic aesthetic
    VoidLookAndFeel voidLookAndFeel;
    
    int currentTab = 0;
    
    // Button handlers
    void displayModeButtonClicked();
    
    VoidTextureSynthAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoidTextureSynthAudioProcessorEditor)
};
