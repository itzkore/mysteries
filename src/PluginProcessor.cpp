



#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VoidTextureSynthAudioProcessor::VoidTextureSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                   #if ! JucePlugin_IsMidiEffect
                    #if ! JucePlugin_IsSynth
                     .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                    #endif
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                   #endif
                     )
#endif
{
    // DSP engines will be initialized here once implemented
}

VoidTextureSynthAudioProcessor::~VoidTextureSynthAudioProcessor() {}

//==============================================================================
const juce::String VoidTextureSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VoidTextureSynthAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool VoidTextureSynthAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool VoidTextureSynthAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double VoidTextureSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VoidTextureSynthAudioProcessor::getNumPrograms()
{
    return 1;
}

int VoidTextureSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VoidTextureSynthAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String VoidTextureSynthAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void VoidTextureSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void VoidTextureSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void VoidTextureSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VoidTextureSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void VoidTextureSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that didn't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    // TODO: Add DSP processing here
}

juce::AudioProcessorEditor* VoidTextureSynthAudioProcessor::createEditor() { return new VoidTextureSynthAudioProcessorEditor(*this); }
bool VoidTextureSynthAudioProcessor::hasEditor() const { return true; }

void VoidTextureSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData) {}
void VoidTextureSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {}
void VoidTextureSynthAudioProcessor::setStateInformation(const void*, int) {}

extern "C" juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VoidTextureSynthAudioProcessor();
}
