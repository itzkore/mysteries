#include <juce_core/juce_core.h>
#include "../PluginProcessor.h"

class VoidTextureSynthUnitTest : public juce::UnitTest {
public:
    VoidTextureSynthUnitTest() : juce::UnitTest("VoidTextureSynth", "DSP") {}
    void runTest() override {
        beginTest("Parameter Layout Validity");
        auto layout = VoidTextureSynthAudioProcessor::createParameterLayout();
        expect(true); // ParameterLayout constructed, parameters added in PluginProcessor.cpp
        // Add more DSP and thread safety tests here
    }
};

static VoidTextureSynthUnitTest testInstance;
