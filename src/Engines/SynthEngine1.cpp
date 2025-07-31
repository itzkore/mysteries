#include "SynthEngine1.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

SynthEngine1::SynthEngine1(juce::AudioProcessorValueTreeState& apvts)
    : apvts(apvts),
      oscillatorLayer(),
      subLayer(),
      noiseLayer(),
      samplerLayer()
{
    // Any additional initialization if needed
}
SynthEngine1::~SynthEngine1() {}

void SynthEngine1::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    oscillatorLayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
    subLayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
    noiseLayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
    samplerLayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void SynthEngine1::releaseResources()
{
    oscillatorLayer.releaseResources();
    subLayer.releaseResources();
    noiseLayer.releaseResources();
    samplerLayer.releaseResources();
}

void SynthEngine1::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    juce::AudioBuffer<float> tempBuffer(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
    tempBuffer.clear();

    juce::AudioSourceChannelInfo tempInfo(&tempBuffer, 0, bufferToFill.numSamples);
    oscillatorLayer.getNextAudioBlock(tempInfo);
    subLayer.getNextAudioBlock(tempInfo);
    noiseLayer.getNextAudioBlock(tempInfo);
    samplerLayer.getNextAudioBlock(tempInfo);

    for (int ch = 0; ch < bufferToFill.buffer->getNumChannels(); ++ch)
        bufferToFill.buffer->addFrom(ch, bufferToFill.startSample, tempBuffer, ch, 0, bufferToFill.numSamples);
}

OscillatorLayer& SynthEngine1::getOscillatorLayer() { return oscillatorLayer; }
SubLayer& SynthEngine1::getSubLayer() { return subLayer; }
NoiseLayer& SynthEngine1::getNoiseLayer() { return noiseLayer; }
SamplerLayer& SynthEngine1::getSamplerLayer() { return samplerLayer; }
