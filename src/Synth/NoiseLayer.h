#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

class NoiseLayer : public juce::AudioSource {
public:
    NoiseLayer();
    ~NoiseLayer() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    // Set noise type from GUI (thread-safe)
    void setNoiseType(int type); // 0=white, 1=pink, etc.

private:
    int noiseType = 0;
    juce::SpinLock noiseTypeLock;
};
