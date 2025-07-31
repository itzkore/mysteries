//==============================================================================
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"

void VoidTextureSynthAudioProcessor::setOscPreset(int idx)
{
    if (idx >= 0 && idx < (int)this->oscPresets.size())
        this->oscPresetIndex = idx;
}
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
    ),
#endif
    apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    // DSP engines will be initialized here once implemented
}

juce::AudioProcessorValueTreeState::ParameterLayout VoidTextureSynthAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    // Example macro parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>("MACRO1", "Macro 1", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("MACRO2", "Macro 2", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("MACRO3", "Macro 3", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("MACRO4", "Macro 4", 0.0f, 1.0f, 0.5f));
    // Add more parameters as needed
    return layout;
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
    juce::ignoreUnused (samplesPerBlock);
    currentSampleRate = static_cast<float>(sampleRate);
    oscPhase = 0.0f;
    oscFrequency = 220.0f;
    oscPhaseDelta = juce::MathConstants<float>::twoPi * oscFrequency / currentSampleRate;
    midiNote = -1;
    // 10 example oscillator presets
    oscPresets = {
        {"Init Sine", 0, 0.0f, 0.8f},
        {"Fat Saw", 1, 0.05f, 0.7f},
        {"Dark Pulse", 2, 0.0f, 0.6f},
        {"Triangle Soft", 3, 0.0f, 0.8f},
        {"Bright Saw", 1, 0.12f, 0.9f},
        {"Square Hollow", 2, 0.0f, 0.5f},
        {"Sub Sine", 0, 0.0f, 1.0f},
        {"Detuned Saw", 1, 0.18f, 0.7f},
        {"Pulse Wide", 2, 0.0f, 0.8f},
        {"Triangle Thin", 3, 0.0f, 0.6f}
    };
    oscPresetIndex = 0;
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
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // MIDI note handling
    for (const auto meta : midiMessages)
    {
        const auto msg = meta.getMessage();
        if (msg.isNoteOn())
        {
            midiNote = msg.getNoteNumber();
            oscPhase = 0.0f;
        }
        else if (msg.isNoteOff())
        {
            if (msg.getNoteNumber() == midiNote)
                midiNote = -1;
        }
    }

    // Clear any output channels that didn't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // --- Oscillator engine with presets and MIDI ---
    auto* left = buffer.getWritePointer(0);
    auto numSamples = buffer.getNumSamples();
    float gain = oscPresets[oscPresetIndex].gain;
    float detune = oscPresets[oscPresetIndex].detune;
    int waveform = (int)oscPresets[oscPresetIndex].waveform;
    float freq = midiNote >= 0 ? juce::MidiMessage::getMidiNoteInHertz(midiNote) * (1.0f + detune) : 0.0f;
    oscPhaseDelta = freq > 0.0f ? juce::MathConstants<float>::twoPi * freq / currentSampleRate : 0.0f;
    for (int i = 0; i < numSamples; ++i)
    {
        float sample = 0.0f;
        if (midiNote >= 0)
        {
            switch (waveform)
            {
                case 0: sample = std::sin(oscPhase); break; // Sine
                case 1: sample = oscPhase < juce::MathConstants<float>::pi ? -1.0f + 2.0f * oscPhase / juce::MathConstants<float>::pi : 3.0f - 2.0f * oscPhase / juce::MathConstants<float>::pi; break; // Saw
                case 2: sample = oscPhase < juce::MathConstants<float>::pi ? 1.0f : -1.0f; break; // Square
                case 3: sample = std::abs(oscPhase / juce::MathConstants<float>::pi - 1.0f) * 2.0f - 1.0f; break; // Triangle
                default: sample = std::sin(oscPhase); break;
            }
            sample *= gain;
            oscPhase += oscPhaseDelta;
            if (oscPhase > juce::MathConstants<float>::twoPi)
                oscPhase -= juce::MathConstants<float>::twoPi;
        }
        left[i] = sample;
    }
    // Copy to right channel if stereo
    if (buffer.getNumChannels() > 1)
        buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);
    else
        // If mono, copy left to right channel
        buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);
}

juce::AudioProcessorEditor* VoidTextureSynthAudioProcessor::createEditor() { return new VoidTextureSynthAudioProcessorEditor(*this); }
bool VoidTextureSynthAudioProcessor::hasEditor() const { return true; }

void VoidTextureSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData) {}
void VoidTextureSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {}
//==============================================================================
// VST3 entry point
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VoidTextureSynthAudioProcessor();
}