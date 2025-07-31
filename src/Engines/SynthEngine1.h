#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "OscillatorLayer.h"
#include "SubLayer.h"
#include "NoiseLayer.h"
#include "SamplerLayer.h"

class SynthEngine1 : public juce::AudioSource {
public:
    SynthEngine1(juce::AudioProcessorValueTreeState& apvts);
    ~SynthEngine1() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    // Layer accessors
    OscillatorLayer& getOscillatorLayer();
    SubLayer& getSubLayer();
    NoiseLayer& getNoiseLayer();
    SamplerLayer& getSamplerLayer();

    // Parameter IDs
    static constexpr const char* macroParamIDs[4] = { "macro1", "macro2", "macro3", "macro4" };
    static constexpr const char* oscWaveformID = "oscWaveform";
    static constexpr const char* oscFreqID = "oscFreq";
    static constexpr const char* noiseTypeID = "noiseType";
    static constexpr const char* subFreqID = "subFreq";

private:
    juce::AudioProcessorValueTreeState& apvts;
    OscillatorLayer oscillatorLayer;
    SubLayer subLayer;
    NoiseLayer noiseLayer;
    SamplerLayer samplerLayer;
};
