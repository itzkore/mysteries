#include <JuceHeader.h>

class FFTAnalyzer {
public:
    FFTAnalyzer() : fft(11) {}
    void process(const float* buffer, int numSamples) {
        // FFT processing for GUI visualizer
    }
private:
    juce::dsp::FFT fft;
};
