#include "SamplerLayer.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

SamplerLayer::SamplerLayer() {
    formatManager.registerBasicFormats();
}

SamplerLayer::~SamplerLayer() {}

void SamplerLayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    // Prepare sample playback (if loaded)
}

void SamplerLayer::releaseResources() {
    // Release sample resources
    readerSource.reset();
}

void SamplerLayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    if (readerSource)
        readerSource->getNextAudioBlock(bufferToFill);
    else
        bufferToFill.clearActiveBufferRegion();
}

void SamplerLayer::loadSample(const juce::File& file) {
    auto* reader = formatManager.createReaderFor(file);
    if (reader) {
        readerSource.reset(new juce::AudioFormatReaderSource(reader, true));
    }
}
