#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

class OscillatorLayer : public juce::AudioSource {
public:
    OscillatorLayer();
    ~OscillatorLayer() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    // Set oscillator parameters
    void setFrequency(float frequency);
    void setWaveform(int type); // 0=sine, 1=saw, 2=square, etc.

private:
    juce::dsp::Oscillator<float> oscillator;
    float currentFrequency = 440.0f;
    int waveformType = 0;
};
