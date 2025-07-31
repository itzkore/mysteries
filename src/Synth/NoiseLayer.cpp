#include "NoiseLayer.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

NoiseLayer::NoiseLayer() {}

NoiseLayer::~NoiseLayer() {}

void NoiseLayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    juce::ignoreUnused(samplesPerBlockExpected, sampleRate);
}

void NoiseLayer::releaseResources()
{
}

void NoiseLayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* left = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* right = bufferToFill.buffer->getNumChannels() > 1 ? bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample) : nullptr;
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        float sample = 0.0f;
        switch (noiseType)
        {
            case 0: // White noise
                sample = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;
                break;
            // case 1: // Pink noise (to be implemented)
            //     break;
            default:
                sample = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;
                break;
        }
        left[i] = sample;
        if (right) right[i] = sample;
    }
}

void NoiseLayer::setNoiseType(int type)
{
    // Thread-safe setter for GUI control
    juce::SpinLock::ScopedLockType lock(noiseTypeLock);
    noiseType = type;
}

// Example usage for GUI:
// noiseLayerInstance->setNoiseType(guiComboBox.getSelectedId());
