//==============================================================================
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"
#include "GUI/OrbVisualizer.h"

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
    apvts(*this, nullptr, "Parameters", createParameterLayout()),
    synthEngine1(apvts) // Initialize synthEngine1 with apvts
{
    // DSP engines will be initialized here once implemented
}

juce::AudioProcessorValueTreeState::ParameterLayout VoidTextureSynthAudioProcessor::createParameterLayout()
{
    return ::createParameterLayout(); // Use the comprehensive parameter layout from Parameters.cpp
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
    
    // Initialize the enhanced synthesis engine
    synthEngine1.prepareToPlay(samplesPerBlock, sampleRate);
    
    // Legacy oscillator initialization (can be removed later)
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
            
            // Update MIDI activity status
            isNoteActive = true;
            currentMidiVelocity = msg.getFloatVelocity(); // normalized 0-1
        }
        else if (msg.isNoteOff())
        {
            if (msg.getNoteNumber() == midiNote)
            {
                midiNote = -1;
                
                // Update MIDI activity status
                isNoteActive = false;
                // Don't reset velocity here - let it decay naturally in the visualizer
            }
        }
    }

    // Clear any output channels that didn't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // --- Enhanced Multi-Layer Ambient Pad Synthesis ---
    // Update layer frequencies and activation based on MIDI note
    if (midiNote >= 0)
    {
        float baseFreq = juce::MidiMessage::getMidiNoteInHertz(midiNote);
        
        // Update all layers with the base frequency and activate them
        synthEngine1.getOscillatorLayer().setFrequency(baseFreq);
        synthEngine1.getOscillatorLayer().setActive(true);
        
        synthEngine1.getSubLayer().setFrequency(baseFreq * 0.5f); // Sub octave
        synthEngine1.getSubLayer().setActive(true); // Activate sub layer
        
        synthEngine1.getNoiseLayer().setActive(true); // Activate noise layer
        
        // Process the enhanced synthesis engine
        juce::AudioSourceChannelInfo channelInfo(&buffer, 0, buffer.getNumSamples());
        synthEngine1.getNextAudioBlock(channelInfo);
        
        // Apply master volume to the final output
        float masterVolume = *apvts.getRawParameterValue("masterVolume");
        buffer.applyGain(masterVolume);
    }
    else
    {
        // No MIDI note active - deactivate layers and clear buffer
        synthEngine1.getOscillatorLayer().setActive(false);
        synthEngine1.getSubLayer().setActive(false); // Deactivate sub layer
        synthEngine1.getNoiseLayer().setActive(false); // Deactivate noise layer
        buffer.clear();
    }
    
    // Update waveform display if connected
    if (currentWaveformDisplay != nullptr)
    {
        // Send audio data to visualizer
        currentWaveformDisplay->pushAudioData(buffer);
        
        // Send MIDI activity status to visualizer
        currentWaveformDisplay->setMidiActivity(isNoteActive, currentMidiVelocity);
    }
}

juce::AudioProcessorEditor* VoidTextureSynthAudioProcessor::createEditor() { return new VoidTextureSynthAudioProcessorEditor(*this); }
bool VoidTextureSynthAudioProcessor::hasEditor() const { return true; }

void VoidTextureSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData) {}
void VoidTextureSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {}
//==============================================================================
// VST3 entry point
//==============================================================================
// This creates new instances of the plugin..
// Required JUCE global entry point for VST3
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VoidTextureSynthAudioProcessor();
}