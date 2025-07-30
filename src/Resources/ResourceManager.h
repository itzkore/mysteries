#pragma once
#include <JuceHeader.h>
class ResourceManager {
public:
    ResourceManager();
    void loadWavetable(const juce::File& file);
    void loadSample(const juce::File& file);
};
