#include "SubLayer.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

SubLayer::SubLayer()
{
    // Sub oscillator: sine wave by default
    subOscillator.initialise([](float x) { return std::sin(x); });
    subOscillator.setFrequency(subFrequency);
}

SubLayer::~SubLayer() {}

void SubLayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = static_cast<double>(sampleRate);
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlockExpected);
    spec.numChannels = 1;

    subOscillator.prepare(spec);
}

void SubLayer::releaseResources()
{
    subOscillator.reset();
}

void SubLayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* left = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* right = bufferToFill.buffer->getNumChannels() > 1 ? bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample) : nullptr;
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        float sample = subOscillator.processSample(0.0f);
        left[i] = sample;
        if (right) right[i] = sample;
    }
}

void SubLayer::setFrequency(float frequency)
{
    subFrequency = frequency;
    subOscillator.setFrequency(frequency);
}
