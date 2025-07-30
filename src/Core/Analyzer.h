#pragma once
#include <juce_dsp/juce_dsp.h>
class FFTAnalyzer {
public:
    FFTAnalyzer();
    void process(const float* buffer, int numSamples);
};
