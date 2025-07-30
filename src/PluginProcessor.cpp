

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

VoidTextureSynthAudioProcessor::VoidTextureSynthAudioProcessor() {
    // Init DSP engines
}
VoidTextureSynthAudioProcessor::~VoidTextureSynthAudioProcessor() {}

const juce::String VoidTextureSynthAudioProcessor::getName() const { return "VoidTextureSynth"; }
void VoidTextureSynthAudioProcessor::prepareToPlay(double, int) {}
void VoidTextureSynthAudioProcessor::releaseResources() {}
void VoidTextureSynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    // Example: process both engines and sum output
    float tempA[buffer.getNumSamples()];
    float tempB[buffer.getNumSamples()];
    wavetableA.process(tempA, buffer.getNumSamples());
    granularB.process(tempB, buffer.getNumSamples());
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            buffer.setSample(ch, i, 0.5f * (tempA[i] + tempB[i]));
        }
    }
    // FX rack processing
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        reverb.process(buffer.getWritePointer(ch), buffer.getNumSamples());
        delay.process(buffer.getWritePointer(ch), buffer.getNumSamples());
        bitcrusher.process(buffer.getWritePointer(ch), buffer.getNumSamples());
    }
}
juce::AudioProcessorEditor* VoidTextureSynthAudioProcessor::createEditor() { return new VoidTextureSynthAudioProcessorEditor(*this); }
bool VoidTextureSynthAudioProcessor::hasEditor() const { return true; }
double VoidTextureSynthAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int VoidTextureSynthAudioProcessor::getNumPrograms() { return 1; }
int VoidTextureSynthAudioProcessor::getCurrentProgram() { return 0; }
void VoidTextureSynthAudioProcessor::setCurrentProgram(int) {}
const juce::String VoidTextureSynthAudioProcessor::getProgramName(int) { return {}; }
void VoidTextureSynthAudioProcessor::changeProgramName(int, const juce::String&) {}
void VoidTextureSynthAudioProcessor::getStateInformation(juce::MemoryBlock&) {}
void VoidTextureSynthAudioProcessor::setStateInformation(const void*, int) {}

extern "C" juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VoidTextureSynthAudioProcessor();
}
