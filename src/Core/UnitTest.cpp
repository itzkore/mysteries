#include <JuceHeader.h>

#include <JuceHeader.h>
class VoidTextureSynthUnitTest : public juce::UnitTest {
public:
    VoidTextureSynthUnitTest() : juce::UnitTest("VoidTextureSynth", "DSP") {}
    void runTest() override {
        beginTest("Parameter Layout Validity");
        auto layout = createParameterLayout();
        expect(layout.size() > 0);
        // Add more DSP and thread safety tests here
    }
};

static VoidTextureSynthUnitTest testInstance;
