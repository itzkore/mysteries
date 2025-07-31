#include "OscillatorLayer.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

OscillatorLayer::OscillatorLayer()
{
    // Default to sine
    oscillator.initialise([](float x) { return std::sin(x); });
    oscillator.setFrequency(currentFrequency);
}

OscillatorLayer::~OscillatorLayer() {}

void OscillatorLayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    juce::dsp::ProcessSpec spec;
spec.sampleRate = static_cast<double>(sampleRate);
spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlockExpected);
spec.numChannels = 1;
oscillator.prepare(spec);}

void OscillatorLayer::releaseResources()
{
    oscillator.reset();
}

void OscillatorLayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* left = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* right = bufferToFill.buffer->getNumChannels() > 1 ? bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample) : nullptr;
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        float sample = oscillator.processSample(0.0f);
        left[i] = sample;
        if (right) right[i] = sample;
    }
}

void OscillatorLayer::setFrequency(float frequency)
{
    currentFrequency = frequency;
    oscillator.setFrequency(frequency);
}

void OscillatorLayer::setWaveform(int type)
{
    waveformType = type;
    switch (type)
    {
        case 0: // Sine
            oscillator.initialise([](float x) { return std::sin(x); });
            break;
        case 1: // Saw
            oscillator.initialise([](float x) { return x / juce::MathConstants<float>::pi; });
            break;
        case 2: // Square
            oscillator.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
            break;
        default:
            oscillator.initialise([](float x) { return std::sin(x); });
            break;
    }
}
