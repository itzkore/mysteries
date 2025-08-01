#include "OscillatorLayer.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

OscillatorLayer::OscillatorLayer()
{
    // Initialize all three oscillators with triangle waves for ambient pads
    oscillator1.initialise(triangleWave, 128);
    oscillator2.initialise(triangleWave, 128);
    oscillator3.initialise(triangleWave, 128);
    
    // Set initial frequencies with slight detuning for rich harmonics
    updateOscillatorFrequencies();
    
    // Configure lowpass filter for smooth pad character
    lowpassFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    lowpassFilter.setCutoffFrequency(1200.0f);
    lowpassFilter.setResonance(0.3f);
}

OscillatorLayer::~OscillatorLayer() {}

void OscillatorLayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    this->sampleRate = sampleRate;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlockExpected);
    spec.numChannels = 1;
    
    // Prepare all oscillators and filter
    oscillator1.prepare(spec);
    oscillator2.prepare(spec);
    oscillator3.prepare(spec);
    lowpassFilter.prepare(spec);
}

void OscillatorLayer::releaseResources()
{
    oscillator1.reset();
    oscillator2.reset();
    oscillator3.reset();
    lowpassFilter.reset();
}

void OscillatorLayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* left = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* right = bufferToFill.buffer->getNumChannels() > 1 ? bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample) : nullptr;
    
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        float finalSample = 0.0f;
        
        // Only generate sound if layer is active (note is pressed)
        if (isActive)
        {
            // Mix multiple detuned oscillators for rich harmonic content
            float sample1 = oscillator1.processSample(0.0f);
            float sample2 = oscillator2.processSample(0.0f);
            float sample3 = oscillator3.processSample(0.0f);
            
            // Energy-based mixing to prevent level buildup
            float mixedSample = (sample1 + sample2 + sample3) * (1.0f / std::sqrt(3.0f));
            
            // Apply lowpass filtering for smooth pad character
            float tempSample = mixedSample;
            float* channelPointer = &tempSample;
            auto block = juce::dsp::AudioBlock<float>(&channelPointer, 1, 1);
            auto context = juce::dsp::ProcessContextReplacing<float>(block);
            lowpassFilter.process(context);
            float filteredSample = tempSample;
            
            // Apply layer level
            finalSample = filteredSample * layerLevel;
        }
        
        left[i] = finalSample;
        if (right) right[i] = finalSample;
    }
}

void OscillatorLayer::setFrequency(float frequency)
{
    currentFrequency = frequency;
    updateOscillatorFrequencies();
}

void OscillatorLayer::setWaveform(int type)
{
    waveformType = type;
    switch (type)
    {
        case 0: // Sine
            oscillator1.initialise([](float x) { return std::sin(x); }, 128);
            oscillator2.initialise([](float x) { return std::sin(x); }, 128);
            oscillator3.initialise([](float x) { return std::sin(x); }, 128);
            break;
        case 1: // Saw (detuned for ambient pads)
            oscillator1.initialise([](float x) { return x / juce::MathConstants<float>::pi; }, 128);
            oscillator2.initialise([](float x) { return x / juce::MathConstants<float>::pi; }, 128);
            oscillator3.initialise([](float x) { return x / juce::MathConstants<float>::pi; }, 128);
            break;
        case 2: // Triangle (recommended for ambient pads)
            oscillator1.initialise(triangleWave, 128);
            oscillator2.initialise(triangleWave, 128);
            oscillator3.initialise(triangleWave, 128);
            break;
        case 3: // Square
            oscillator1.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 128);
            oscillator2.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 128);
            oscillator3.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 128);
            break;
        default:
            oscillator1.initialise(triangleWave, 128);
            oscillator2.initialise(triangleWave, 128);
            oscillator3.initialise(triangleWave, 128);
            break;
    }
}

void OscillatorLayer::setDetune(float cents)
{
    detuneAmount = cents;
    updateOscillatorFrequencies();
}

void OscillatorLayer::setVoiceSpread(float spread)
{
    voiceSpread = spread;
    updateOscillatorFrequencies();
}

void OscillatorLayer::setLevel(float level)
{
    layerLevel = juce::jlimit(0.0f, 1.0f, level);
}

void OscillatorLayer::setActive(bool active)
{
    isActive = active;
}

void OscillatorLayer::setAttack(float attackMs)
{
    attackTime = attackMs / 1000.0f; // Convert to seconds
}

void OscillatorLayer::setRelease(float releaseMs)
{
    releaseTime = releaseMs / 1000.0f; // Convert to seconds
}

void OscillatorLayer::setSustain(float sustainLevel)
{
    this->sustainLevel = juce::jlimit(0.0f, 1.0f, sustainLevel);
}

void OscillatorLayer::updateOscillatorFrequencies()
{
    // Calculate detuned frequencies for rich harmonic content
    // Oscillator 1: Base frequency
    oscillator1.setFrequency(currentFrequency);
    
    // Oscillator 2: Slightly detuned up (+5-10 cents)
    float detuneFactor2 = std::pow(2.0f, (detuneAmount * 0.5f) / 1200.0f);
    oscillator2.setFrequency(currentFrequency * detuneFactor2);
    
    // Oscillator 3: Slightly detuned down (-3-7 cents)
    float detuneFactor3 = std::pow(2.0f, (-detuneAmount * 0.7f) / 1200.0f);
    oscillator3.setFrequency(currentFrequency * detuneFactor3);
}
