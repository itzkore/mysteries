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

    // Enhanced oscillator parameters for ambient pads
    void setFrequency(float frequency);
    void setWaveform(int type); // 0=sine, 1=saw, 2=triangle, 3=square
    void setDetune(float cents); // Detune amount in cents
    void setVoiceSpread(float spread); // Spread between multiple oscillators
    void setLevel(float level); // Layer level (0.0 - 1.0)
    void setActive(bool isActive); // Control if layer should produce sound
    
    // Ambient pad specific parameters
    void setAttack(float attackMs);
    void setRelease(float releaseMs);
    void setSustain(float sustainLevel);

private:
    // Multiple detuned oscillators for rich harmonic content
    juce::dsp::Oscillator<float> oscillator1;
    juce::dsp::Oscillator<float> oscillator2;
    juce::dsp::Oscillator<float> oscillator3;
    
    // ADSR envelope for pad-like behavior
    juce::dsp::StateVariableTPTFilter<float> lowpassFilter;
    
    // Parameters
    float currentFrequency = 440.0f;
    int waveformType = 2; // Default to triangle for smooth pads
    float detuneAmount = 5.0f; // Slight detune in cents
    float voiceSpread = 10.0f; // Spread between oscillators
    float layerLevel = 0.7f;
    
    // Envelope parameters (in samples)
    float attackTime = 2.0f; // 2 second attack
    float releaseTime = 4.0f; // 4 second release
    float sustainLevel = 0.8f;
    
    // Internal state
    double sampleRate = 44100.0;
    bool isActive = false; // Only play when note is active
    
    // Helper methods
    void updateOscillatorFrequencies();
    
    // Triangle wave function for smooth harmonic content
    static float triangleWave(float x) {
        return std::asin(std::sin(x)) * (2.0f / juce::MathConstants<float>::pi);
    }
};
