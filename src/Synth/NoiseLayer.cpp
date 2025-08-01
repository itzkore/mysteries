#include "NoiseLayer.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

NoiseLayer::NoiseLayer() 
    : gen(rd()), dist(-1.0f, 1.0f)
{
    // Initialize pink noise state
    for (int i = 0; i < 7; ++i)
        pinkState[i] = 0.0f;
    
    // Configure bandpass filter for atmospheric texture (2-8kHz range)
    bandpassFilter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
    bandpassFilter.setCutoffFrequency(filterFrequency);
    bandpassFilter.setResonance(0.5f); // Moderate Q for character
    
    // Setup slow LFO for filter modulation
    filterLFO.initialise([](float x) { return std::sin(x); }, 128);
    filterLFO.setFrequency(0.1f); // Very slow (0.1 Hz) for ambient movement
}

NoiseLayer::~NoiseLayer() {}

void NoiseLayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    this->sampleRate = sampleRate;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlockExpected);
    spec.numChannels = 1;
    
    // Prepare filter and LFO
    bandpassFilter.prepare(spec);
    filterLFO.prepare(spec);
}

void NoiseLayer::releaseResources()
{
    bandpassFilter.reset();
    filterLFO.reset();
}

void NoiseLayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
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
        // Generate noise based on type
        float noiseSample = 0.0f;
        {
            juce::SpinLock::ScopedLockType lock(noiseTypeLock);
            switch (noiseType)
            {
                case 0: // White noise
                    noiseSample = generateWhiteNoise();
                    break;
                case 1: // Pink noise (recommended for ambient)
                    noiseSample = generatePinkNoise();
                    break;
                case 2: // Brown noise
                    noiseSample = generateBrownNoise();
                    break;
                default:
                    noiseSample = generatePinkNoise();
                    break;
            }
        }
        
        // Apply slow filter modulation for atmospheric movement
        float lfoValue = filterLFO.processSample(0.0f);
        float modulatedFrequency = filterFrequency + (lfoValue * filterModDepth * 1000.0f);
        bandpassFilter.setCutoffFrequency(juce::jlimit(500.0f, 8000.0f, modulatedFrequency));
        
        // Apply bandpass filtering for atmospheric character
        float tempSample = noiseSample;
        float* channelPointer = &tempSample;
        auto block = juce::dsp::AudioBlock<float>(&channelPointer, 1, 1);
        auto context = juce::dsp::ProcessContextReplacing<float>(block);
        bandpassFilter.process(context);
        float filteredSample = tempSample;
        
        // Apply level and atmosphere control
        float finalSample = filteredSample * noiseLevel * atmosphereAmount;
        
        left[i] = finalSample;
        if (right) right[i] = finalSample;
    }
}

void NoiseLayer::setNoiseType(int type)
{
    juce::SpinLock::ScopedLockType lock(noiseTypeLock);
    noiseType = type;
}

void NoiseLayer::setFilterFrequency(float frequency)
{
    filterFrequency = juce::jlimit(500.0f, 8000.0f, frequency);
    bandpassFilter.setCutoffFrequency(filterFrequency);
}

void NoiseLayer::setFilterModulation(float depth)
{
    filterModDepth = juce::jlimit(0.0f, 1.0f, depth);
}

void NoiseLayer::setLevel(float level)
{
    noiseLevel = juce::jlimit(0.0f, 1.0f, level);
}

void NoiseLayer::setAtmosphere(float amount)
{
    atmosphereAmount = juce::jlimit(0.0f, 1.0f, amount);
}

void NoiseLayer::setActive(bool isActive)
{
    this->isActive = isActive;
}

float NoiseLayer::generateWhiteNoise()
{
    return dist(gen);
}

float NoiseLayer::generatePinkNoise()
{
    // Paul Kellet's pink noise algorithm
    float white = dist(gen);
    pinkState[0] = 0.99886f * pinkState[0] + white * 0.0555179f;
    pinkState[1] = 0.99332f * pinkState[1] + white * 0.0750759f;
    pinkState[2] = 0.96900f * pinkState[2] + white * 0.1538520f;
    pinkState[3] = 0.86650f * pinkState[3] + white * 0.3104856f;
    pinkState[4] = 0.55000f * pinkState[4] + white * 0.5329522f;
    pinkState[5] = -0.7616f * pinkState[5] - white * 0.0168980f;
    float pink = pinkState[0] + pinkState[1] + pinkState[2] + pinkState[3] + pinkState[4] + pinkState[5] + pinkState[6] + white * 0.5362f;
    pinkState[6] = white * 0.115926f;
    return pink * 0.11f; // Scale down
}

float NoiseLayer::generateBrownNoise()
{
    static float lastOutput = 0.0f;
    float white = dist(gen);
    float brown = (lastOutput + (0.02f * white)) / 1.02f;
    lastOutput = brown;
    brown *= 3.5f; // Compensate for attenuation
    return juce::jlimit(-1.0f, 1.0f, brown);
}
