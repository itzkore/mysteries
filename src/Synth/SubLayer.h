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

    // Enhanced sub parameters for ambient pads
    void setFrequency(float frequency);
    void setLevel(float level); // Sub layer level (0.0 - 1.0)
    void setSubHarmonics(float amount); // Add subtle harmonics
    void setWarmth(float warmth); // Overall warmth character
    void setActive(bool isActive); // Control if layer should produce sound

private:
    // Dual sub oscillators for richness
    juce::dsp::Oscillator<float> subOscillator1; // Primary sub
    juce::dsp::Oscillator<float> subOscillator2; // Octave down
    
    // Low-pass filtering for sub-bass focus
    juce::dsp::StateVariableTPTFilter<float> lowpassFilter;
    
    // Parameters
    float subFrequency = 60.0f; // Sub-bass range (40-120Hz)
    float subLevel = 0.3f; // Subtle but present
    float harmonicsAmount = 0.2f; // Subtle harmonics
    float warmthAmount = 0.5f;
    
    // Internal state
    double sampleRate = 44100.0;
    bool isActive = false; // Only play when note is active
    
    // Helper methods
    void updateSubFrequencies();
};
