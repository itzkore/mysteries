private:
    WavetableEngine wavetableA;
    GranularEngine granularB;
    ReverbFX reverb;
    DelayFX delay;
    BitCrusherFX bitcrusher;
#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "DSP/FX/ReverbFX.h"
#include "DSP/FX/DelayFX.h"
#include "DSP/FX/BitCrusherFX.h"

class VoidTextureSynthAudioProcessor : public juce::AudioProcessor {

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "DSP/FX/ReverbFX.h"
#include "DSP/FX/DelayFX.h"
#include "DSP/FX/BitCrusherFX.h"

//==============================================================================
class VoidTextureSynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    VoidTextureSynthAudioProcessor();
    ~VoidTextureSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;


#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

//==============================================================================
class VoidTextureSynthAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    VoidTextureSynthAudioProcessor();
    ~VoidTextureSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    // DSP komponenty budou přidány později
    // WavetableEngine wavetableA;
    // GranularEngine granularB;
    // ReverbFX reverb;
    // DelayFX delay;
    // BitCrusherFX bitcrusher;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoidTextureSynthAudioProcessor)
};
    BitCrusherFX bitcrusher;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoidTextureSynthAudioProcessor)
};
