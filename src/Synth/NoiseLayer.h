#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include <random>

class NoiseLayer : public juce::AudioSource {
public:
    NoiseLayer();
    ~NoiseLayer() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    // Enhanced noise parameters for ambient pads
    void setNoiseType(int type); // 0=white, 1=pink, 2=brown
    void setFilterFrequency(float frequency); // Center frequency for bandpass
    void setFilterModulation(float depth); // LFO modulation depth
    void setLevel(float level); // Noise layer level (0.0 - 1.0)
    void setAtmosphere(float amount); // Overall atmospheric character
    void setActive(bool isActive); // Control if layer should produce sound

private:
    // Noise generation
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist;
    
    // Pink noise filter coefficients
    float pinkState[7] = {0};
    
    // Filtering for atmospheric texture
    juce::dsp::StateVariableTPTFilter<float> bandpassFilter;
    juce::dsp::Oscillator<float> filterLFO;
    
    // Parameters
    int noiseType = 1; // Default to pink noise for natural texture
    float filterFrequency = 3000.0f; // Upper mid-range for air
    float filterModDepth = 0.3f; // Subtle modulation
    float noiseLevel = 0.2f; // Subtle level (10-30% as recommended)
    float atmosphereAmount = 0.5f;
    
    // Thread safety
    juce::SpinLock noiseTypeLock;
    
    // Internal state
    double sampleRate = 44100.0;
    bool isActive = false; // Only play when note is active
    
    // Helper methods
    float generateWhiteNoise();
    float generatePinkNoise();
    float generateBrownNoise();
};
