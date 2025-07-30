#pragma once
#include <juce_core/juce_core.h>
class GranularEngine {
public:
    GranularEngine();
    void setParameter(const juce::String& name, float value);
    void process(float* outBuffer, int numSamples);
};
