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
    // Get parameter values from APVTS
    auto oscEnable = apvts.getRawParameterValue("osc1Enable")->load();
    auto subEnable = apvts.getRawParameterValue("subEnable")->load();
    auto noiseEnable = apvts.getRawParameterValue("noiseEnable")->load();
    auto samplerEnable = apvts.getRawParameterValue("samplerEnable")->load();
    
    auto oscLevel = apvts.getRawParameterValue("osc1Level")->load();
    auto subLevel = apvts.getRawParameterValue("subLevel")->load();
    auto noiseLevel = apvts.getRawParameterValue("noiseLevel")->load();
    auto samplerLevel = apvts.getRawParameterValue("samplerLevel")->load();
    
    auto oscPan = apvts.getRawParameterValue("osc1Pan")->load();
    auto subPan = apvts.getRawParameterValue("subPan")->load();
    auto noisePan = apvts.getRawParameterValue("noisePan")->load();
    auto samplerPan = apvts.getRawParameterValue("samplerPan")->load();
    
    // Update enhanced layer parameters
    auto oscWaveform = static_cast<int>(apvts.getRawParameterValue("osc1Waveform")->load());
    auto oscDetune = apvts.getRawParameterValue("osc1Detune")->load();
    auto noiseType = static_cast<int>(apvts.getRawParameterValue("noiseType")->load());
    auto noiseFilterCutoff = apvts.getRawParameterValue("noiseFilterCutoff")->load();
    
    // Apply enhanced parameters to layers
    oscillatorLayer.setWaveform(oscWaveform);
    oscillatorLayer.setDetune(oscDetune);
    oscillatorLayer.setLevel(oscLevel);
    
    noiseLayer.setNoiseType(noiseType);
    noiseLayer.setFilterFrequency(noiseFilterCutoff);
    noiseLayer.setLevel(noiseLevel);
    
    subLayer.setLevel(subLevel);

    // Clear the output buffer
    bufferToFill.buffer->clear(bufferToFill.startSample, bufferToFill.numSamples);
    
    // Create temporary buffers for each layer
    juce::AudioBuffer<float> layerBuffer(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
    
    // Process Oscillator Layer
    if (oscEnable > 0.5f) {
        layerBuffer.clear();
        juce::AudioSourceChannelInfo layerInfo(&layerBuffer, 0, bufferToFill.numSamples);
        oscillatorLayer.getNextAudioBlock(layerInfo);
        
        // Apply level and pan
        for (int ch = 0; ch < bufferToFill.buffer->getNumChannels(); ++ch) {
            float panGain = 1.0f;
            if (bufferToFill.buffer->getNumChannels() == 2) {
                // Stereo panning: -1.0 = left, 0.0 = center, 1.0 = right
                panGain = (ch == 0) ? (1.0f - std::max(0.0f, oscPan)) : (1.0f + std::min(0.0f, oscPan));
            }
            bufferToFill.buffer->addFrom(ch, bufferToFill.startSample, 
                                       layerBuffer, ch, 0, bufferToFill.numSamples, 
                                       oscLevel * panGain);
        }
    }
    
    // Process Sub Oscillator Layer
    if (subEnable > 0.5f) {
        layerBuffer.clear();
        juce::AudioSourceChannelInfo layerInfo(&layerBuffer, 0, bufferToFill.numSamples);
        subLayer.getNextAudioBlock(layerInfo);
        
        // Apply level and pan
        for (int ch = 0; ch < bufferToFill.buffer->getNumChannels(); ++ch) {
            float panGain = 1.0f;
            if (bufferToFill.buffer->getNumChannels() == 2) {
                panGain = (ch == 0) ? (1.0f - std::max(0.0f, subPan)) : (1.0f + std::min(0.0f, subPan));
            }
            bufferToFill.buffer->addFrom(ch, bufferToFill.startSample, 
                                       layerBuffer, ch, 0, bufferToFill.numSamples, 
                                       subLevel * panGain);
        }
    }
    
    // Process Noise Layer
    if (noiseEnable > 0.5f) {
        layerBuffer.clear();
        juce::AudioSourceChannelInfo layerInfo(&layerBuffer, 0, bufferToFill.numSamples);
        noiseLayer.getNextAudioBlock(layerInfo);
        
        // Apply level and pan
        for (int ch = 0; ch < bufferToFill.buffer->getNumChannels(); ++ch) {
            float panGain = 1.0f;
            if (bufferToFill.buffer->getNumChannels() == 2) {
                panGain = (ch == 0) ? (1.0f - std::max(0.0f, noisePan)) : (1.0f + std::min(0.0f, noisePan));
            }
            bufferToFill.buffer->addFrom(ch, bufferToFill.startSample, 
                                       layerBuffer, ch, 0, bufferToFill.numSamples, 
                                       noiseLevel * panGain);
        }
    }
    
    // Process Sampler Layer
    if (samplerEnable > 0.5f) {
        layerBuffer.clear();
        juce::AudioSourceChannelInfo layerInfo(&layerBuffer, 0, bufferToFill.numSamples);
        samplerLayer.getNextAudioBlock(layerInfo);
        
        // Apply level and pan
        for (int ch = 0; ch < bufferToFill.buffer->getNumChannels(); ++ch) {
            float panGain = 1.0f;
            if (bufferToFill.buffer->getNumChannels() == 2) {
                panGain = (ch == 0) ? (1.0f - std::max(0.0f, samplerPan)) : (1.0f + std::min(0.0f, samplerPan));
            }
            bufferToFill.buffer->addFrom(ch, bufferToFill.startSample, 
                                       layerBuffer, ch, 0, bufferToFill.numSamples, 
                                       samplerLevel * panGain);
        }
    }
}

OscillatorLayer& SynthEngine1::getOscillatorLayer() { return oscillatorLayer; }
SubLayer& SynthEngine1::getSubLayer() { return subLayer; }
NoiseLayer& SynthEngine1::getNoiseLayer() { return noiseLayer; }
SamplerLayer& SynthEngine1::getSamplerLayer() { return samplerLayer; }
