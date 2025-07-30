#pragma once
// This line is already present, so we will not add it again.
#include <juce_core/juce_core.h>
class ResourceManager {
public:
    ResourceManager();
    void loadWavetable(const juce::File& file);
    void loadSample(const juce::File& file);
};
