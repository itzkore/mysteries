

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

//==============================================================================
// EngineDisplay: Top-level component for engine visualization
class EngineDisplay : public juce::Component, private juce::Timer {
public:
    EngineDisplay(const juce::String& name, VoidTextureSynthAudioProcessor* proc = nullptr)
        : engineName(name), processor(proc) {
        setName(name);
        if (name == "Oscillator" && processor) startTimerHz(30);
    }
    void paint(juce::Graphics& g) override {
        g.setColour(juce::Colours::white.withAlpha(0.12f));
        g.fillRect(getLocalBounds().reduced(10));
        g.setColour(juce::Colours::white);
        g.drawText(engineName, getLocalBounds(), juce::Justification::centredTop);
        if (engineName == "Oscillator" && processor) {
            auto area = getLocalBounds().reduced(20, 40);
            g.setColour(juce::Colours::aqua);
            juce::Path wave;
            int N = area.getWidth();
            float amp = area.getHeight() / 2.0f;
            float phase = processor->oscPhase;
            float delta = processor->oscPhaseDelta;
            wave.startNewSubPath(area.getX(), area.getCentreY());
            for (int x = 0; x < N; ++x) {
                float y = std::sin(phase + x * delta) * amp;
                wave.lineTo(area.getX() + x, area.getCentreY() - y);
            }
            g.strokePath(wave, juce::PathStrokeType(2.0f));
        }
    }
    void timerCallback() override { repaint(); }
    juce::String engineName;
    VoidTextureSynthAudioProcessor* processor = nullptr;
};

//==============================================================================
class VoidTextureSynthAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    VoidTextureSynthAudioProcessorEditor (VoidTextureSynthAudioProcessor&);
    ~VoidTextureSynthAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;


private:
    VoidTextureSynthAudioProcessor& audioProcessor;

    // Macro parameter sliders
    juce::Slider macroSlider1;
    juce::Slider macroSlider2;
    juce::Slider macroSlider3;
    juce::Slider macroSlider4;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> macroAttachment1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> macroAttachment2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> macroAttachment3;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> macroAttachment4;

    // Macro group
    juce::GroupComponent macroGroup;

    // Mod matrix UI region (scaffold)
    juce::GroupComponent modMatrixGroup;

    // --- Engine region group components ---
    juce::GroupComponent oscGroup;
    juce::GroupComponent samplerGroup;
    juce::GroupComponent noiseGroup;
    juce::GroupComponent subGroup;

    // --- Engine display components ---
    std::unique_ptr<EngineDisplay> oscDisplay;
    std::unique_ptr<EngineDisplay> samplerDisplay;
    std::unique_ptr<EngineDisplay> noiseDisplay;
    std::unique_ptr<EngineDisplay> subDisplay;

    // Preset selectors
    juce::ComboBox oscPresetBox;
    juce::ComboBox samplerPresetBox;

    // Bypass button
    juce::ToggleButton bypassButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoidTextureSynthAudioProcessorEditor)
};
