#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

class SubLayer : public juce::AudioSource {
public:
    SubLayer();
    ~SubLayer() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    // Set sub oscillator parameters
    void setFrequency(float frequency);

private:
    juce::dsp::Oscillator<float> subOscillator;
    float subFrequency = 60.0f;
};
