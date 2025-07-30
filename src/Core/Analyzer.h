#pragma once
#include <JuceHeader.h>
class FFTAnalyzer {
public:
    FFTAnalyzer();
    void process(const float* buffer, int numSamples);
};
