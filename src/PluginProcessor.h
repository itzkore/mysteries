


#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

//==============================================================================
class VoidTextureSynthAudioProcessor : public juce::AudioProcessor
{
public:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts;
    // --- Minimal oscillator engine state ---
    float currentSampleRate = 44100.0f;
    float oscPhase = 0.0f;
    float oscFrequency = 220.0f;
    float oscPhaseDelta = 0.0f;
    VoidTextureSynthAudioProcessor();
    ~VoidTextureSynthAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool  acceptsMidi()   const override;
    bool  producesMidi()  const override;
    bool  isMidiEffect()  const override;
    double getTailLengthSeconds() const override;

    int  getNumPrograms() override;
    int  getCurrentProgram() override;
    void setCurrentProgram (int)   override;
    const juce::String getProgramName (int) override;
    void changeProgramName (int, const juce::String&) override;

    void getStateInformation (juce::MemoryBlock&) override;
    void setStateInformation (const void*, int) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoidTextureSynthAudioProcessor)
};
