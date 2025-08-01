#include "SubLayer.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

SubLayer::SubLayer()
{
    // Initialize dual sub oscillators with sine waves for clean low-end
    subOscillator1.initialise([](float x) { return std::sin(x); }, 128);
    subOscillator2.initialise([](float x) { return std::sin(x); }, 128);
    
    // Configure low-pass filter for sub-bass focus (40-120Hz range)
    lowpassFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    lowpassFilter.setCutoffFrequency(120.0f); // Sharp cutoff for sub-bass only
    lowpassFilter.setResonance(0.1f); // Very low resonance to avoid muddiness
    
    updateSubFrequencies();
}

SubLayer::~SubLayer() {}

void SubLayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    this->sampleRate = sampleRate;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlockExpected);
    spec.numChannels = 1;

    // Prepare both oscillators and filter
    subOscillator1.prepare(spec);
    subOscillator2.prepare(spec);
    lowpassFilter.prepare(spec);
}

void SubLayer::releaseResources()
{
    subOscillator1.reset();
    subOscillator2.reset();
    lowpassFilter.reset();
}

void SubLayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // If not active, clear buffer and return
    if (!isActive) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    auto* left = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* right = bufferToFill.buffer->getNumChannels() > 1 ? bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample) : nullptr;
    
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        // Generate dual sub oscillators for richness
        float subSample1 = subOscillator1.processSample(0.0f);
        float subSample2 = subOscillator2.processSample(0.0f) * harmonicsAmount;
        
        // Mix the sub oscillators
        float mixedSub = subSample1 + subSample2;
        
        // Apply low-pass filtering to focus on sub-bass frequencies
        float tempSample = mixedSub;
        float* channelPointer = &tempSample;
        auto block = juce::dsp::AudioBlock<float>(&channelPointer, 1, 1);
        auto context = juce::dsp::ProcessContextReplacing<float>(block);
        lowpassFilter.process(context);
        float filteredSub = tempSample;
        
        // Apply warmth and level control
        float finalSample = filteredSub * subLevel * warmthAmount;
        
        left[i] = finalSample;
        if (right) right[i] = finalSample;
    }
}

void SubLayer::setFrequency(float frequency)
{
    // Ensure frequency stays in sub-bass range (40-120Hz)
    subFrequency = juce::jlimit(40.0f, 120.0f, frequency);
    updateSubFrequencies();
}

void SubLayer::setLevel(float level)
{
    subLevel = juce::jlimit(0.0f, 1.0f, level);
}

void SubLayer::setSubHarmonics(float amount)
{
    harmonicsAmount = juce::jlimit(0.0f, 0.5f, amount); // Keep subtle
}

void SubLayer::setWarmth(float warmth)
{
    warmthAmount = juce::jlimit(0.0f, 1.0f, warmth);
}

void SubLayer::setActive(bool isActive)
{
    this->isActive = isActive;
}

void SubLayer::updateSubFrequencies()
{
    // Primary sub oscillator at fundamental frequency
    subOscillator1.setFrequency(subFrequency);
    
    // Secondary sub oscillator at octave down for extra depth
    subOscillator2.setFrequency(subFrequency * 0.5f);
}
